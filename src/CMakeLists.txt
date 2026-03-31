# =============================================================================
# Core Libraries
# =============================================================================

# Calculator library
add_library(calculator 
    calculator.cpp
)
target_include_directories(calculator 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(calculator 
    PUBLIC 
		fmt::fmt
)

# Calculator runner
add_library(run_calc 
    run_calc.cpp
)
target_link_libraries(run_calc 
    PUBLIC 
        calculator
		fmt::fmt
)

# =============================================================================
# MQTT Library
# =============================================================================
add_library(my_mqtt 
    mqtt.cpp
)
target_include_directories(my_mqtt 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(my_mqtt
    PUBLIC
        PahoMqttCpp::paho-mqttpp3
		fmt::fmt
)

# =============================================================================
# Platform-Specific Libraries
# =============================================================================

# COM Ports library (Windows only)
if(WIN32)
    add_library(my_com_ports 
        ports.cpp
    )
    target_include_directories(my_com_ports 
        PUBLIC 
            ${CMAKE_SOURCE_DIR}/include
    )
	target_link_libraries(my_com_ports
		PUBLIC
			fmt::fmt
	)
endif()

# BLE scanner using WinRT (Windows WinRT only)
if(CMAKE_ENABLE_WINDOWS_WINRT)
    message(STATUS "Building BLE scanner with WinRT support")
    
    add_library(my_ble 
        ble.cpp
    )
    target_include_directories(my_ble 
        PUBLIC 
            ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(my_ble
        PUBLIC
            Microsoft::CppWinRT
            windowsapp
			fmt::fmt
    )
endif()