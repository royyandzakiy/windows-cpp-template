# cmake/config_vcpkg.cmake
# =============================================================================
# VCPKG Configuration
# =============================================================================

# Set default triplet based on platform
if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg triplet")
else()
    set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "vcpkg triplet")
endif()

# Determine VCPKG root path
if(DEFINED VCPKG_ROOT_PATH)
    # Already defined via cache variable
elseif(DEFINED ENV{VCPKG_ROOT_PATH})
    set(VCPKG_ROOT_PATH "$ENV{VCPKG_ROOT_PATH}" CACHE PATH "vcpkg root directory")
elseif(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT_PATH "$ENV{VCPKG_ROOT}" CACHE PATH "vcpkg root directory")
endif()

# Validate VCPKG_ROOT_PATH
if(NOT DEFINED VCPKG_ROOT_PATH)
    message(FATAL_ERROR "\n╔════════════════════════════════════════════════════════════════════════════╗\n"
                        "║  VCPKG_ROOT_PATH is not set!                                              ║\n"
                        "║  Please define it in your CMake preset or environment:                    ║\n"
                        "║    - CMakePresets.json: \"VCPKG_ROOT_PATH\": \"/path/to/vcpkg\"            ║\n"
                        "║    - Environment: export VCPKG_ROOT_PATH=/path/to/vcpkg                   ║\n"
                        "╚════════════════════════════════════════════════════════════════════════════╝\n")
endif()

# Set toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain file")
endif()

# Status indicators
set(STATUS_OK "✓")
set(STATUS_FAIL "✗")
set(STATUS_WARN "⚠")
set(STATUS_INFO "ℹ")

message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════════════════════════╗")
message(STATUS "║                           VCPKG CONFIGURATION                              ║")
message(STATUS "╚════════════════════════════════════════════════════════════════════════════╝")
message(STATUS "")

# Initialize counters
set(VCPKG_CHECKS_PASSED 0)
set(VCPKG_CHECKS_TOTAL 3)

# -----------------------------------------------------------------------------
# Check 1: CMAKE_TOOLCHAIN_FILE
# -----------------------------------------------------------------------------
message(STATUS "┌─ [1/${VCPKG_CHECKS_TOTAL}] CMAKE_TOOLCHAIN_FILE")
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    if(EXISTS "${CMAKE_TOOLCHAIN_FILE}")
        message(STATUS "│   ${STATUS_OK} ${CMAKE_TOOLCHAIN_FILE}")
        math(EXPR VCPKG_CHECKS_PASSED "${VCPKG_CHECKS_PASSED} + 1")
    else()
        message(STATUS "│   ${STATUS_FAIL} ${CMAKE_TOOLCHAIN_FILE} (file not found)")
        message(FATAL_ERROR "│\n╚════════════════════════════════════════════════════════════════════════════╝\n"
                            "CMAKE_TOOLCHAIN_FILE not found! Please verify vcpkg installation path.\n")
    endif()
else()
    message(STATUS "│   ${STATUS_FAIL} CMAKE_TOOLCHAIN_FILE is not set!")
    message(FATAL_ERROR "│\n╚════════════════════════════════════════════════════════════════════════════╝\n"
                        "CMAKE_TOOLCHAIN_FILE must be set! Add to your CMake preset.\n")
endif()
message(STATUS "└─────────────────────────────────────────────────────────────────────────")

# -----------------------------------------------------------------------------
# Check 2: VCPKG_TARGET_TRIPLET
# -----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "┌─ [2/${VCPKG_CHECKS_TOTAL}] VCPKG_TARGET_TRIPLET")
if(DEFINED VCPKG_TARGET_TRIPLET)
    message(STATUS "│   ${STATUS_OK} ${VCPKG_TARGET_TRIPLET}")
    math(EXPR VCPKG_CHECKS_PASSED "${VCPKG_CHECKS_PASSED} + 1")
else()
    message(STATUS "│   ${STATUS_INFO} Using default: ${VCPKG_TARGET_TRIPLET}")
    set(VCPKG_TARGET_TRIPLET "${VCPKG_TARGET_TRIPLET}" CACHE STRING "vcpkg triplet" FORCE)
    math(EXPR VCPKG_CHECKS_PASSED "${VCPKG_CHECKS_PASSED} + 1")
endif()
message(STATUS "└─────────────────────────────────────────────────────────────────────────")

# -----------------------------------------------------------------------------
# Check 3: vcpkg installation
# -----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "┌─ [3/${VCPKG_CHECKS_TOTAL}] vcpkg Installation")
set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT_PATH}/installed")

if(EXISTS "${VCPKG_INSTALLED_DIR}")
    message(STATUS "│   ${STATUS_OK} vcpkg root: ${VCPKG_ROOT_PATH}")
    message(STATUS "│   ${STATUS_OK} installed packages: ${VCPKG_INSTALLED_DIR}")
    
    set(TRIPLET_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
    if(EXISTS "${TRIPLET_DIR}")
        message(STATUS "│   ${STATUS_OK} triplet directory: ${TRIPLET_DIR}")
        math(EXPR VCPKG_CHECKS_PASSED "${VCPKG_CHECKS_PASSED} + 1")
        
        # Quick package checks
        if(EXISTS "${TRIPLET_DIR}/include/drogon")
            message(STATUS "│   ${STATUS_OK} drogon: found")
        else()
            message(STATUS "│   ${STATUS_WARN} drogon: not found (run: ./vcpkg install drogon --triplet ${VCPKG_TARGET_TRIPLET})")
        endif()
        
        if(EXISTS "${TRIPLET_DIR}/include/fmt")
            message(STATUS "│   ${STATUS_OK} fmt: found")
        else()
            message(STATUS "│   ${STATUS_WARN} fmt: not found (run: ./vcpkg install fmt --triplet ${VCPKG_TARGET_TRIPLET})")
        endif()
        
        if(EXISTS "${TRIPLET_DIR}/include/json")
            message(STATUS "│   ${STATUS_OK} jsoncpp: found")
        else()
            message(STATUS "│   ${STATUS_WARN} jsoncpp: not found (run: ./vcpkg install jsoncpp --triplet ${VCPKG_TARGET_TRIPLET})")
        endif()
    else()
        message(STATUS "│   ${STATUS_WARN} triplet directory not found: ${TRIPLET_DIR}")
        message(STATUS "│   ${STATUS_INFO} Run: ./vcpkg install --triplet ${VCPKG_TARGET_TRIPLET}")
        math(EXPR VCPKG_CHECKS_PASSED "${VCPKG_CHECKS_PASSED} + 0")
    endif()
else()
    message(STATUS "│   ${STATUS_FAIL} vcpkg installation not found at: ${VCPKG_ROOT_PATH}")
    message(STATUS "│   ${STATUS_INFO} Please verify VCPKG_ROOT_PATH points to a valid vcpkg installation")
    message(FATAL_ERROR "│\n╚════════════════════════════════════════════════════════════════════════════╝\n"
                        "vcpkg installation not found! Please verify your vcpkg path.\n")
endif()
message(STATUS "└─────────────────────────────────────────────────────────────────────────")

# -----------------------------------------------------------------------------
# Summary
# -----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════════════════════════╗")
message(STATUS "║                         VCPKG CONFIGURATION SUMMARY                        ║")
message(STATUS "╠════════════════════════════════════════════════════════════════════════════╣")
message(STATUS "║  ${STATUS_OK} Toolchain file       : ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "║  ${STATUS_OK} vcpkg root           : ${VCPKG_ROOT_PATH}")
message(STATUS "║  ${STATUS_OK} Triplet              : ${VCPKG_TARGET_TRIPLET}")
message(STATUS "╠════════════════════════════════════════════════════════════════════════════╣")
message(STATUS "║  Checks passed: ${VCPKG_CHECKS_PASSED}/${VCPKG_CHECKS_TOTAL} (${VCPKG_CHECKS_TOTAL} critical)				                            ║")
if(VCPKG_CHECKS_PASSED EQUAL VCPKG_CHECKS_TOTAL)
    message(STATUS "║  Status: ${STATUS_OK} READY for configuration                                        ║")
else()
    message(STATUS "║  Status: ${STATUS_WARN} Some checks failed - proceed with caution                      ║")
endif()
message(STATUS "╚════════════════════════════════════════════════════════════════════════════╝")
message(STATUS "")