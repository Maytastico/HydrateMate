#include <unity.h>
#include "HydrateMonitor.hpp"

void setUp(void) {
    // set stuff up here
}

void test_moisture_evaluation(void) {
    HydrateMonitor hydrate_monitor;
    hydrate_monitor::MoistureLevel moisture_level = hydrate_monitor.evaluteMoistureLevel(0);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    UNITY_END();
}