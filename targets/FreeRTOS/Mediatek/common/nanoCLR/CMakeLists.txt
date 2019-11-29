#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

include(NF_NativeAssemblies)

# append nanoHAL
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL.cpp)
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/nanoHAL.cpp)
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp)

# append target MEDIATEK nanoCLR source files
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_MonitorCommands.c)

# append target HAL source files
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_Power.c)
list(APPEND TARGET_MEDIATEK_NANOCLR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_Time.cpp)

# append TRNG only if option is ON
if(USE_RNG)

    # else()
    #     # this series doesn't feature hardware TRNG, report
    #     message(ERROR "This CPU doesn't have a Random Number Generation. Please change USE_RNG option to OFF.")
    # endif()

endif()

# add native assemblies
ParseNativeAssemblies()

# make var global
set(TARGET_MEDIATEK_NANOCLR_SOURCES ${TARGET_MEDIATEK_NANOCLR_SOURCES} CACHE INTERNAL "make global")

# append target MEDIATEK nanoCLR include directory
list(APPEND TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)

# append PAL include directory
list(APPEND TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)

# append Runtime.Native include directory
list(APPEND TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Runtime.Native)

# make var global
set(TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS ${TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS} CACHE INTERNAL "make global")
