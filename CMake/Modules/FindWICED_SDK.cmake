#
# Copyright (c) 2020 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# List of the required include paths
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/include)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/libraries/filesystems/wicedfs/src)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/libraries/utilities/ring_buffer)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/libraries/utilities/TLV)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/platforms/BCM943362WCD6)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/network/NoNS/WWD)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/network/LwIP/ver2.0.3/src/include)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/include)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/ARM_CM4)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/GCC)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU/STM32F4xx)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU/STM32F4xx/peripherals)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU/STM32F4xx/WWD)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU/STM32F4xx/peripherals/libraries/inc)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/security/BESL/include)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/RTOS/NoOS)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/RTOS/NoOS/WWD)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/RTOS/NoOS/WWD)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/RTOS/NoOS/WWD/Cortex_M3_M4)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/include)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/include/network)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/include/RTOS)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/chips/43362)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/bus_protocols)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/bus_protocols/SDIO)
list(APPEND WICED_SDK_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/security/PostgreSQL/include)


set(src_WICED_SDK

    # chip
    platform.c

    host_cm4.c
    platform.c
    platform_resource.c
    wiced_crypto.c
    wwd_ap.c
    wwd_ap_common.c
    wwd_buffer.c
    wwd_bus_common.c
    wwd_bus_protocol.c
    wwd_chip_specific_functions.c
    wwd_debug.c
    wwd_eapol.c
    wwd_internal.c
    wwd_management.c
    wwd_network.c
    wwd_platform.c
    wwd_platform_separate_mcu.c
    wwd_resources.c
    wwd_rtos.c
    wwd_SDIO.c
    wwd_sdpcm.c
    wwd_thread.c
    wwd_wifi.c
    wwd_wifi_chip_common.c
    wwd_wifi_sleep.c
    

    # platform 
)

foreach(SRC_FILE ${src_WICED_SDK})
    set(WICED_SDK_SRC_FILE SRC_FILE -NOTFOUND)
    find_file(WICED_SDK_SRC_FILE ${SRC_FILE}
        PATHS 

            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/platforms/BCM943362WCD6
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/bus_protocols
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/bus_protocols/SDIO
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/WWD/internal/chips/43362
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/ARM_CM4
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/internal/unit
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/internal
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/network/LwIP/WWD
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/platform/MCU/STM32F4xx/WWD
            ${PROJECT_BINARY_DIR}/WICED_SDK_Source/WICED/RTOS/NoOS/WWD


        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${WICED_SDK_SRC_FILE}") # debug helper
    list(APPEND WICED_SDK_SOURCES ${WICED_SDK_SRC_FILE})
endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(WICED_SDK DEFAULT_MSG WICED_SDK_INCLUDE_DIRS WICED_SDK_SOURCES)
