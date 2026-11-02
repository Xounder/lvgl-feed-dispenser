#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/domain/dosing_controller.h"
#include "../src/hardware/dispenser.h"
#include "../src/hardware/weight_sensor.h"

static DosingConfig config;
static int grams;
static int active;
static int starts;
static int stops;
static int last_step;
static int weight_after_add;
static DispenserFlow flow;

static void check(int condition, int line)
{
    if (!condition) {
        fprintf(stderr, "Falha na linha %d\n", line);
        exit(EXIT_FAILURE);
    }
}

#define CHECK(condition) check((condition), __LINE__)

static int read_grams(void)
{
    return grams;
}

static void add_grams(int step)
{
    last_step = step;
    if (weight_after_add != INT_MIN) {
        grams = weight_after_add;
    }
}

static void reset_weight(void)
{
    grams = 0;
}

static void start(DispenserFlow requested_flow)
{
    flow = requested_flow;
    active = 1;
    starts++;
}

static void stop(void)
{
    active = 0;
    stops++;
}

static int is_active(void)
{
    return active;
}

WeightSensor weight_sensor = {read_grams, add_grams, reset_weight};
Dispenser dispenser = {start, stop, is_active};

static void reset(int target, int initial_weight)
{
    config.mode = DOSING_MODE_GRAMS;
    config.target_grams = target;
    config.target_money_cents = 600;
    config.price_per_kg_cents = 1200;
    grams = initial_weight;
    active = 0;
    starts = 0;
    stops = 0;
    last_step = 0;
    weight_after_add = INT_MIN;
    dosing_controller_init(&config);
    CHECK(!active);
    CHECK(stops == 1);
}

static void test_phases_and_noise(void)
{
    reset(100, 0);
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FAST);
    CHECK(active && starts == 1);
    dosing_controller_start();
    CHECK(starts == 1);
    grams = 69;
    dosing_controller_update();
    CHECK(flow == DISPENSER_FLOW_FAST && last_step == 20);
    grams = 70;
    dosing_controller_update();
    CHECK(flow == DISPENSER_FLOW_FINE && last_step == 2);
    CHECK(dosing_controller_get_phase() == DOSING_PHASE_FINE);
    CHECK(starts == 2);
    grams = 68;
    dosing_controller_update();
    CHECK(flow == DISPENSER_FLOW_FINE && starts == 2);
    CHECK(last_step == 2);
    grams = 100;
    dosing_controller_update();
    CHECK(!active);
    CHECK(dosing_controller_get_state() == DOSING_STATE_COMPLETED);
    dosing_controller_update();
    CHECK(starts == 2);
}

static void test_initial_phase_and_existing_weight(void)
{
    reset(30, 0);
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FINE && starts == 1);
    reset(31, 0);
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FAST);
    reset(100, 80);
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FINE);
    reset(100, 100);
    dosing_controller_start();
    CHECK(!active && starts == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_COMPLETED);
    reset(100, 110);
    dosing_controller_start();
    CHECK(!active && starts == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_COMPLETED);
}

static void test_stop_and_manual_priority(void)
{
    reset(100, 0);
    dosing_controller_manual_release_start();
    CHECK(flow == DISPENSER_FLOW_MANUAL && active);
    CHECK(dosing_controller_manual_release_is_active());
    dosing_controller_manual_release_stop();
    CHECK(!active && !dosing_controller_manual_release_is_active());
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FAST && active);
    int previous_starts = starts;
    int previous_stops = stops;
    dosing_controller_manual_release_start();
    dosing_controller_manual_release_stop();
    CHECK(active && starts == previous_starts && stops == previous_stops);
    CHECK(!dosing_controller_manual_release_is_active());
    dosing_controller_cancel();
    CHECK(!active && dosing_controller_get_state() == DOSING_STATE_INTERRUPTED);
    grams = 75;
    dosing_controller_update();
    CHECK(starts == previous_starts);
    dosing_controller_manual_release_start();
    CHECK(active && flow == DISPENSER_FLOW_MANUAL);
    dosing_controller_new_dosing();
    CHECK(!active && grams == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_IDLE);
    CHECK(!dosing_controller_manual_release_is_active());
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FAST && active);
    grams = 100;
    dosing_controller_update();
    dosing_controller_manual_release_start();
    CHECK(flow == DISPENSER_FLOW_MANUAL && active);
    dosing_controller_manual_release_stop();
    CHECK(!active);
}

static void test_invalid_inputs_and_reads(void)
{
    const int targets[] = {0, -1, 40001};
    const int weights[] = {-1, 40001};
    for (unsigned i = 0; i < sizeof(targets) / sizeof(targets[0]); i++) {
        reset(targets[i], 0);
        dosing_controller_start();
        CHECK(!active && starts == 0);
        CHECK(dosing_controller_get_state() == DOSING_STATE_INTERRUPTED);
    }
    for (unsigned i = 0; i < sizeof(weights) / sizeof(weights[0]); i++) {
        reset(100, weights[i]);
        dosing_controller_start();
        CHECK(!active && starts == 0);
        CHECK(dosing_controller_get_state() == DOSING_STATE_INTERRUPTED);
    }
    reset(100, 0);
    dosing_controller_start();
    grams = 40001;
    dosing_controller_update();
    CHECK(dosing_controller_get_state() == DOSING_STATE_DOSING);
    CHECK(starts == 1 && flow == DISPENSER_FLOW_FAST);
    grams = 80;
    weight_after_add = 40001;
    dosing_controller_update();
    CHECK(dosing_controller_get_state() == DOSING_STATE_DOSING);
    CHECK(flow == DISPENSER_FLOW_FINE);
    weight_after_add = 101;
    grams = 99;
    dosing_controller_update();
    CHECK(!active && dosing_controller_get_state() == DOSING_STATE_COMPLETED);
}

static void test_currency(void)
{
    reset(100, 469);
    config.mode = DOSING_MODE_CURRENCY;
    CHECK(dosing_controller_get_target_grams() == 500);
    dosing_controller_start();
    CHECK(flow == DISPENSER_FLOW_FAST);
    grams = 470;
    dosing_controller_update();
    CHECK(flow == DISPENSER_FLOW_FINE);
    grams = 501;
    dosing_controller_update();
    CHECK(!active && dosing_controller_get_state() == DOSING_STATE_COMPLETED);
    reset(100, 0);
    config.mode = DOSING_MODE_CURRENCY;
    config.price_per_kg_cents = 0;
    dosing_controller_start();
    CHECK(!active && starts == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_INTERRUPTED);
}

static void test_maximum_weight(void)
{
    reset(40000, 39969);
    dosing_controller_start();
    CHECK(active && flow == DISPENSER_FLOW_FAST);
    grams = 39970;
    dosing_controller_update();
    CHECK(active && flow == DISPENSER_FLOW_FINE);
    grams = 40000;
    dosing_controller_update();
    CHECK(!active && dosing_controller_get_state() == DOSING_STATE_COMPLETED);
    reset(40000, 40000);
    dosing_controller_start();
    CHECK(!active && starts == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_COMPLETED);
    reset(100, 40000);
    dosing_controller_start();
    CHECK(!active && starts == 0);
    CHECK(dosing_controller_get_state() == DOSING_STATE_COMPLETED);
}

int main(void)
{
    test_phases_and_noise();
    test_initial_phase_and_existing_weight();
    test_stop_and_manual_priority();
    test_invalid_inputs_and_reads();
    test_currency();
    test_maximum_weight();
    puts("Testes do controlador: OK");
    return EXIT_SUCCESS;
}
