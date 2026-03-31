#ifdef WINDOWS_WINRT_ENABLED

#include "../include/ble.h"
#include <fmt/core.h>
#include <thread>

void run_ble_scan() {
    try {
        winrt::init_apartment();

        fmt::print("Starting BLE scan (will run for 3 seconds)...\n");

        BluetoothLEAdvertisementWatcher watcher;
        std::promise<void> scan_completed_promise;
        auto scan_completed_future = scan_completed_promise.get_future();

        watcher.ScanningMode(BluetoothLEScanningMode::Active);

        watcher.Received([&](const auto & /*sender*/, const BluetoothLEAdvertisementReceivedEventArgs &args) {
            fmt::print("\nBLE Device Found:\n");
            fmt::print("  Address: {:016X}\n", args.BluetoothAddress());
            fmt::print("  RSSI: {} dBm\n", args.RawSignalStrengthInDBm());

            auto advertisement = args.Advertisement();
            if (!advertisement.LocalName().empty()) {
                fmt::print("  Name: {}\n", winrt::to_string(advertisement.LocalName()));
            } else {
                auto get_device_name = [args]() -> winrt::fire_and_forget {
                    try {
                        auto device = co_await BluetoothLEDevice::FromBluetoothAddressAsync(args.BluetoothAddress());
                        if (device != nullptr && !device.Name().empty()) {
                            fmt::print("  Name: {}\n", winrt::to_string(device.Name()));
                        }
                    } catch (...) {
                    } // Ignore errors in async name fetching
                };
                get_device_name();
            }
        });

        watcher.Stopped([&](const auto & /*sender*/, const BluetoothLEAdvertisementWatcherStoppedEventArgs &args) {
            fmt::print("BLE scan stopped. Reason: {}\n", static_cast<int>(args.Error()));
            scan_completed_promise.set_value();
        });

        watcher.Start();
        fmt::print("BLE scan started...\n");

        // Set up a timer to stop after 3 seconds
        std::jthread stop_timer([&watcher, &scan_completed_promise] {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            if (watcher.Status() == BluetoothLEAdvertisementWatcherStatus::Started) {
                watcher.Stop();
            }
        });

        scan_completed_future.wait();
        fmt::print("BLE scan completed after 3 seconds.\n");
    } catch (const winrt::hresult_error &ex) {
        fmt::println(stderr, "BLE scan error: {}", winrt::to_string(ex.message()));
    } catch (const std::exception &ex) {
        fmt::println(stderr, "Error: {}", ex.what());
    }
}

#endif // WINDOWS_WINRT_ENABLED