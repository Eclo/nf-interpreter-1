#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

include(nf_utils)

# add_subdirectory(nanoBooter)
add_subdirectory(nanoCLR)

# append include directory for target MEDIATEK
list(APPEND TARGET_MEDIATEK_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND TARGET_MEDIATEK_COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

# append Target files
list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Target_BlockStorage_MT3620_Driver.c)
list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_HAL_Interface.c)
list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/platform_BlockStorage.c)

# include configuration manager file, if feature is enabled
if(NF_FEATURE_HAS_CONFIG_BLOCK)
    list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_ConfigurationManager.cpp)
endif()

# include MT3620 files
list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mt3620-gpio.c)
list(APPEND TARGET_MEDIATEK_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mt3620-uart.c)

# make var global
set(TARGET_MEDIATEK_COMMON_SOURCES ${TARGET_MEDIATEK_COMMON_SOURCES} CACHE INTERNAL "make global")
set(TARGET_MEDIATEK_COMMON_INCLUDE_DIRS ${TARGET_MEDIATEK_COMMON_INCLUDE_DIRS} CACHE INTERNAL "make global")
