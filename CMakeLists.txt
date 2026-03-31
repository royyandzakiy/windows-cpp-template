#==============================================================================
# Pre-Project Checks
#==============================================================================
cmake_minimum_required(VERSION 3.5..4.2)

include(cmake/local_path.cmake)
include(cmake/config_vcpkg.cmake)

#==============================================================================
# Project Configuration
#==============================================================================
project(cmake_project_template 
    LANGUAGES CXX
    VERSION 1.0.0
)

#==============================================================================
# Version Management
#==============================================================================
include(cmake/version.cmake)
generate_version_header()
source_group("Generated" FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/version.h")
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")

#==============================================================================
# WinRT Configuration (OPTIONAL - Easy to remove completely)
#==============================================================================
option(CMAKE_ENABLE_WINDOWS_WINRT "Enable Windows WinRT support" ON) # Optional: Turn OFF if not using WinRT (or remove)
message(STATUS "Windows WinRT support: ${CMAKE_ENABLE_WINDOWS_WINRT}")

if(CMAKE_ENABLE_WINDOWS_WINRT)
    find_package(cppwinrt CONFIG REQUIRED)
    add_compile_definitions(WINDOWS_WINRT_ENABLED=1)
    message(STATUS "Building with Windows WinRT BLE support")
endif()

#==============================================================================
# Compiler Configuration
#==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: Enable Clang-Tidy if defined
if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    message(STATUS "Clang-Tidy enabled: ${CLANGTIDY}")
endif()

#==============================================================================
# Dependencies
#==============================================================================
find_package(GTest CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)

#==============================================================================
# Project Structure
#==============================================================================
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)

#==============================================================================
# Main Executable
#==============================================================================
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Core dependencies (always included)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        run_calc 
        my_mqtt
)

# Platform-specific dependencies
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_com_ports)
endif()

# OPTIONAL: WinRT dependencies (remove if not using WinRT)
if(CMAKE_ENABLE_WINDOWS_WINRT)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_ble)
endif()