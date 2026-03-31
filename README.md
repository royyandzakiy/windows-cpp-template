# CMake C++ Project Template for Windows

A professional boilerplate template for Windows C++23 development using CMake, vcpkg, and Windows-specific APIs including WinRT and Windows.h. This template demonstrates modern C++ best practices with Windows desktop/console application development.

## Features

- **Modern C++23** with standard library features
- **Windows-specific APIs** including WinRT for BLE and Windows.h for COM ports
- **vcpkg integration** for dependency management
- **CMake Presets** for simplified build configuration
- **Modular architecture** with separated concerns
- **Unit testing** with Google Test
- **Documentation** support with Doxygen

## Prerequisites

### Required Software
- **Visual Studio 2022** with C++ development tools and v143 build tools
- **Git** for version control and vcpkg installation
- **CMake** (3.20 or newer)

### Recommended Extensions
- **Visual Studio Code** with CMake Tools extension
- **C++ Extension Pack** for enhanced C++ development experience

## Quick Start

### 1. Clone and Setup
```bash
git clone <repository-url>
cd cmake-project-template
```

### 2. Configure Environment
Create `CMakeUserPresets.json` in the project root:
```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "default",
      "inherits": "windows-base",
      "environment": {
        "VCPKG_ROOT": "C:/vcpkg"
      }
    }
  ]
}
```

### 3. Build and Run
**Using Visual Studio Code:**
- Run Task: `Configure Project` - This automatically installs and sets up all dependencies
- Run Task: `Build Project` 
- Run Task: `Run Application`

**Using Command Line:**
```bash
cmake --preset default
cmake --build build/default
./build/default/cmake-project-template
```

**What Happens During Configuration:**

When you run Configure Project, the CMake setup script automatically:

1. Checks for required tools (Git, Doxygen, vcpkg)
2. Installs missing dependencies if needed
3. Sets up vcpkg with all project dependencies
4. Configures the build system with proper toolchain

## Project Structure

```
cmake-project-template/
├── include/                 # Header files
│   ├── calculator.h
│   ├── mqtt.h
│   ├── ble.h
│   └── ports.h
├── src/                    # Source implementation
├── test/                   # Unit tests
├── docs/                   # Documentation
├── cmake/                  # CMake utilities
├── CMakePresets.json       # Build configurations
├── vcpkg.json             # Dependency management
└── README.md
```

## Windows-Specific Features

### WinRT Integration
This template includes Windows Runtime (WinRT) support for modern Windows APIs:

- **BLE Scanning** using `Windows.Devices.Bluetooth.Advertisement`
- **C++/WinRT** for type-safe Windows API access
- **Async operations** with coroutines

Example BLE usage:
```cpp
#ifdef WINDOWS_WINRT_ENABLED
#include "../include/ble.h"
run_ble_scan();
#endif
```

### COM Port Management
Traditional Windows API usage for hardware access:

- **Device enumeration** using SetupAPI
- **Registry access** for COM port detection
- **Windows.h integration** for system-level operations

### MQTT Communication
Cross-platform MQTT client implementation with Windows-specific optimizations.

## Using This Template for New Projects

### 1. Initial Setup
```bash
# Clone the template
git clone <template-repository> my-new-project
cd my-new-project

# Update project information
# Modify CMakeLists.txt project name and version
# Update vcpkg.json with your dependencies
```

### 2. Customize Dependencies
Edit `vcpkg.json` to include your project's specific dependencies:
```json
{
  "name": "your-project-name",
  "dependencies": [
    "paho-mqttpp3"
  ],
  "features": {
    "tests": {
      "dependencies": ["gtest"]
    }
  }
}
```

### 3. Add Your Source Code
- Place headers in `include/`
- Implementations in `src/`
- Tests in `test/`

### 4. Configure Windows Features
Enable/disable Windows-specific features in `CMakeLists.txt`:
```cmake
# Enable WinRT for modern Windows APIs
set(CMAKE_ENABLE_WINDOWS_WINRT ON)

# Platform-specific source files
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE windows_specific.cpp)
endif()
```

## Dependency Management

### vcpkg Integration
This template uses vcpkg for seamless dependency management:

- **Automatic installation** of specified packages
- **CMake integration** through toolchain file
- **Version control** via vcpkg.json manifest

### Core Dependencies
- **paho-mqttpp3**: MQTT client implementation
- **gtest**: Unit testing framework (optional)
- **cppwinrt**: Windows Runtime support (Windows only)

## Build Configurations

### Available Presets
- **windows-base**: Debug build with WinRT support
- **windows-release**: Optimized release build

### Custom Presets
Add new presets to `CMakePresets.json` for different configurations:
```json
{
  "name": "windows-custom",
  "inherits": "windows-base",
  "cacheVariables": {
    "CMAKE_BUILD_TYPE": "RelWithDebInfo"
  }
}
```

## Development Workflow

### Testing
```bash
# Run all tests
ctest --preset windows-test

# Run specific test
./build/default/test/unit_tests
```

### Documentation
```bash
# Generate documentation (requires Doxygen)
cmake --build build/default --target docs
```

### Code Quality
```bash
# Enable clang-tidy (if configured)
cmake --preset default -DCLANGTIDY=clang-tidy
```

## Windows API Usage Examples

This template demonstrates several Windows development patterns:

1. **Modern C++ with WinRT** for BLE device scanning
2. **Traditional Windows API** for COM port enumeration  
3. **Mixed-mode development** combining standard C++ with Windows-specific features

## Troubleshooting

### Common Issues

**vcpkg not found:**
- Ensure `VCPKG_ROOT` environment variable is set
- Verify vcpkg installation path in `CMakeUserPresets.json`

**WinRT compilation errors:**
- Confirm Visual Studio 2022 with C++/WinRT components installed
- Check `CMAKE_ENABLE_WINDOWS_WINRT` is set to ON

**Missing dependencies:**
- Run `vcpkg install` from the vcpkg directory
- Verify internet connection for first-time setup

## License

MIT License - see LICENSE file for details.

## Contributing

This template is designed as a starting point for Windows C++ projects. Feel free to adapt it to your specific needs while maintaining the core structure and best practices demonstrated.