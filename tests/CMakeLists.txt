cmake_minimum_required(VERSION 3.12.4)
project(dosing_tests C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(dosing_controller_tests
    dosing_controller_tests.c
    ../src/domain/dosing_controller.c
)
add_test(NAME dosing_controller COMMAND dosing_controller_tests)

add_executable(real_dispenser_tests
    real_dispenser_tests.cpp
    ../src/hardware/esp32/real_dispenser.cpp
)
target_include_directories(real_dispenser_tests PRIVATE stubs)
add_test(NAME real_dispenser COMMAND real_dispenser_tests)
