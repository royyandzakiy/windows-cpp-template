#include "ports.h"

#ifdef _WIN32

#include <windows.h>

#include <cfgmgr32.h>
#include <devguid.h>
#include <fmt/core.h>
#include <format>
#include <iomanip>
#include <setupapi.h>

#pragma comment(lib, "setupapi.lib")

namespace com_ports {

[[nodiscard]] std::vector<ComPortInfo> list_com_ports() {
    std::vector<ComPortInfo> ports;

    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return ports;
    }

    SP_DEVINFO_DATA devInfoData{};
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++) {
        ComPortInfo port_info{};
        port_info.is_available = true;

        // Get device description (friendly name)
        CHAR device_desc[256]{};
        if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                                              reinterpret_cast<PBYTE>(device_desc), sizeof(device_desc), nullptr)) {
            port_info.description = device_desc;

            // Extract COM port number from description
            const std::string desc_str = device_desc;
            const size_t com_pos = desc_str.find("(COM");
            if (com_pos != std::string::npos) {
                const size_t end_pos = desc_str.find(")", com_pos);
                if (end_pos != std::string::npos) {
                    port_info.name = desc_str.substr(com_pos + 1, end_pos - com_pos - 1);
                }
            }
        }

        // If we couldn't extract COM port from friendly name, try device instance ID
        if (port_info.name.empty()) {
            CHAR instance_id[256]{};
            if (SetupDiGetDeviceInstanceIdA(hDevInfo, &devInfoData, instance_id, sizeof(instance_id), nullptr)) {
                const std::string instance_str = instance_id;
                // Look for COM port in instance ID
                const size_t com_pos = instance_str.find("COM");
                if (com_pos != std::string::npos) {
                    port_info.name = "COM" + instance_str.substr(com_pos + 3);
                    // Take only the COM port part
                    const size_t end_pos = port_info.name.find_first_of("&\\");
                    if (end_pos != std::string::npos) {
                        port_info.name = port_info.name.substr(0, end_pos);
                    }
                }
            }
        }

        // Get device manufacturer
        CHAR manufacturer[256]{};
        if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_MFG, nullptr,
                                              reinterpret_cast<PBYTE>(manufacturer), sizeof(manufacturer), nullptr)) {
            if (!port_info.description.empty()) {
                port_info.description += " - " + std::string(manufacturer);
            } else {
                port_info.description = manufacturer;
            }
        }

        // Only add if we found a valid COM port name
        if (!port_info.name.empty() && port_info.name.find("COM") != std::string::npos) {
            ports.push_back(port_info);
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return ports;
}

// Alternative method using registry to enumerate COM ports
[[nodiscard]] std::vector<ComPortInfo> list_com_ports_registry() {
    std::vector<ComPortInfo> ports;

    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD index = 0;
        CHAR value_name[256]{};
        CHAR data[256]{};
        DWORD value_name_size, data_size, type;

        while (true) {
            value_name_size = sizeof(value_name);
            data_size = sizeof(data);

            const LONG result = RegEnumValueA(hKey, index, value_name, &value_name_size, nullptr, &type,
                                              reinterpret_cast<LPBYTE>(data), &data_size);

            if (result == ERROR_NO_MORE_ITEMS) {
                break;
            }

            if (result == ERROR_SUCCESS && type == REG_SZ) {
                ComPortInfo port_info{};
                port_info.name = data;
                port_info.description = value_name;
                port_info.is_available = true;
                ports.push_back(port_info);
            }

            index++;
        }

        RegCloseKey(hKey);
    }

    return ports;
}

void print_com_ports() {
    fmt::print("=== Available COM Ports (PnP Enumeration) ===\n");

    auto ports = list_com_ports();

    if (ports.empty()) {
        fmt::print("No COM ports found via PnP. Trying registry method...\n");
        ports = list_com_ports_registry();
    }

    if (ports.empty()) {
        fmt::print("No COM ports found.\n");
        return;
    }

    fmt::print("{:<10} {:<50} {:<12}\n", "Port", "Description", "Status");
    fmt::print("{:-<72}\n", "");

    for (const auto &port : ports) {
        const std::string status = port.is_available ? "Available" : "Unavailable";
        fmt::print("{:<10} {:<50} {:<12}\n", port.name, port.description, status);
    }
}

int run_com_ports() {
    try {
        print_com_ports();
        return 0;
    } catch (const std::exception &e) {
        fmt::println(stderr, "Error listing COM ports: {}", e.what());
        return 1;
    }
}

} // namespace com_ports

#endif