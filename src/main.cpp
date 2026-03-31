#include "../include/calculator.h"
#include "../include/mqtt.h"
#include "../include/ports.h"
#include <fmt/core.h>

#ifdef WINDOWS_WINRT_ENABLED
#include "../include/ble.h"
#endif

int run_calc(int agc, char **argv);

int main(int argc, char *argv[]) {
    run_calc(argc, argv);
    run_mqtt();

#ifdef _WIN32
    com_ports::run_com_ports();
#endif

#ifdef WINDOWS_WINRT_ENABLED
    run_ble_scan();
#endif

    return 0;
}