#include "real_weight_sensor.h"

#include <Arduino.h>
#include "HX711.h"

#include "../weight_sensor.h"
#include "board_config.h"

#define WEIGHT_LOG_PERIOD_MS 1000
#define WEIGHT_FILTER_SIZE 8
#define WEIGHT_MAX_GRAMS 30000
#define WEIGHT_SPIKE_GRAMS 300
#define WEIGHT_READY_TIMEOUT_MS 300

static HX711 scale;
static int ready = 0;

static long filter_buf[WEIGHT_FILTER_SIZE];
static int filter_pos = 0;
static int filter_count = 0;
static long filter_sum = 0;
static int last_grams = 0;
static long last_raw = 0;

static unsigned long reject_notready = 0;
static unsigned long reject_range = 0;
static unsigned long reject_spike = 0;

static void filter_reset(void)
{
    filter_pos = 0;
    filter_count = 0;
    filter_sum = 0;

    for (int i = 0; i < WEIGHT_FILTER_SIZE; i++) {
        filter_buf[i] = 0;
    }
}

static int to_grams(long raw)
{
    return (int)((raw - scale.get_offset()) / scale.get_scale());
}

static int sample_raw_average(int times, long *out)
{
    long sum = 0;
    int got = 0;

    for (int i = 0; i < times; i++) {
        if (!scale.wait_ready_timeout(WEIGHT_READY_TIMEOUT_MS, 2)) {
            break;
        }

        sum += scale.read();
        got++;
    }

    if (got == 0) {
        return 0;
    }

    *out = sum / got;
    return 1;
}

static int real_read_grams(void)
{
    if (!ready || !scale.is_ready()) {
        reject_notready++;
        return last_grams;
    }

    long raw = scale.read();
    last_raw = raw;

    int raw_grams = to_grams(raw);

    if (raw_grams < 0 || raw_grams > WEIGHT_MAX_GRAMS) {
        reject_range++;
        return last_grams;
    }

    if (filter_count >= 2) {
        int avg_grams = to_grams(filter_sum / filter_count);
        int delta = raw_grams - avg_grams;

        if (delta > WEIGHT_SPIKE_GRAMS || delta < -WEIGHT_SPIKE_GRAMS) {
            reject_spike++;
            return last_grams;
        }
    }

    filter_sum -= filter_buf[filter_pos];
    filter_buf[filter_pos] = raw;
    filter_sum += raw;
    filter_pos = (filter_pos + 1) % WEIGHT_FILTER_SIZE;

    if (filter_count < WEIGHT_FILTER_SIZE) {
        filter_count++;
    }

    last_grams = to_grams(filter_sum / filter_count);
    return last_grams;
}

static void real_add_grams(int grams)
{
    (void)grams;
}

static void real_reset(void)
{
    if (!ready) {
        return;
    }

    long offset_raw;

    if (!sample_raw_average(10, &offset_raw)) {
        Serial.println("[PESAGEM] tara ignorada (HX711 sem resposta)");
        return;
    }

    scale.set_offset(offset_raw);
    filter_reset();
    last_grams = 0;
    Serial.println("[PESAGEM] tara executada");
}

WeightSensor weight_sensor = {
    real_read_grams,
    real_add_grams,
    real_reset
};

void real_weight_sensor_init(void)
{
    Serial.printf(
        "[PESAGEM] init HX711 DT=%d SCK=%d\n",
        BOARD_HX711_DATA_PIN,
        BOARD_HX711_SCK_PIN
    );

    scale.begin(BOARD_HX711_DATA_PIN, BOARD_HX711_SCK_PIN);

    if (!scale.wait_ready_timeout(3000, 10)) {
        Serial.println(
            "[PESAGEM] ERRO: HX711 nao detectado "
            "(verifique DT/SCK/alimentacao); dosagem sem peso real"
        );
        return;
    }

    scale.set_scale(BOARD_HX711_CALIBRATION_FACTOR);

    long offset_raw;

    if (sample_raw_average(10, &offset_raw)) {
        scale.set_offset(offset_raw);
    }

    filter_reset();
    last_grams = 0;
    ready = 1;

    Serial.printf(
        "[PESAGEM] OK escala=%.2f offset=%ld\n",
        scale.get_scale(),
        scale.get_offset()
    );
}

void real_weight_sensor_probe(const char *tag)
{
    int dout = digitalRead(BOARD_HX711_DATA_PIN);
    int sck = digitalRead(BOARD_HX711_SCK_PIN);

    if (!ready) {
        Serial.printf(
            "[PESAGEM] probe %s dout=%d sck=%d (nao pronto)\n",
            tag,
            dout,
            sck
        );
        return;
    }

    int ready_now = scale.is_ready() ? 1 : 0;
    long raw = 0;
    uint32_t dt = 0;

    if (ready_now) {
        uint32_t t0 = micros();
        raw = scale.read();
        dt = micros() - t0;
    }

    Serial.printf(
        "[PESAGEM] probe %s dout=%d sck=%d ready=%d dt=%luus raw=%ld\n",
        tag,
        dout,
        sck,
        ready_now,
        (unsigned long)dt,
        raw
    );
}

void real_weight_sensor_log(void)
{
    static uint32_t last_ms = 0;
    uint32_t now = millis();

    if (now - last_ms < WEIGHT_LOG_PERIOD_MS) {
        return;
    }
    last_ms = now;

    if (!ready) {
        Serial.println("[PESAGEM] valor indisponivel (HX711 ausente)");
        return;
    }

    int ready_now = scale.is_ready() ? 1 : 0;
    uint32_t t0 = micros();
    int grams = real_read_grams();
    uint32_t dt = micros() - t0;

    static uint32_t max_dt = 0;
    if (dt > max_dt) {
        max_dt = dt;
    }

    Serial.printf(
        "[PESAGEM] ready=%d dt=%luus maxdt=%luus raw=%ld g=%d "
        "rej(notready=%lu range=%lu spike=%lu)\n",
        ready_now,
        (unsigned long)dt,
        (unsigned long)max_dt,
        last_raw,
        grams,
        reject_notready,
        reject_range,
        reject_spike
    );
}
