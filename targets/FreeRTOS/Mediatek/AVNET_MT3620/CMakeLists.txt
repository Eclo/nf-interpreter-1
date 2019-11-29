#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

# add header files with common OS definitions and board definitions specific for each image
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/target_board.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/target_board.h" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/target_common.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/target_common.h" @ONLY)

set(PROJECT_NAME "nanoCLR")

find_package(BuildUtils REQUIRED)
find_package(FreeRTOS REQUIRED)
find_package(WireProtocol REQUIRED)

# packages for nanoFramework libraries
#######################################
# mandatory 
find_package(NF_CoreCLR REQUIRED)

#######################################
# optional

# nF feature: debugger
if(NF_FEATURE_DEBUGGER)
    find_package(NF_Debugger REQUIRED)
    find_package(NF_Diagnostics REQUIRED)
endif()

#######################################

SET(ENV{PATH} "${AZURE_SPHERE_SDK_PATH}/Tools;$ENV{PATH}")

#######################
# nanoCLR executable

add_executable(
    # executables for project, project sources
    ${PROJECT_NAME}
   
    ${CMAKE_CURRENT_SOURCE_DIR}/Device_BlockStorage$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-DEBUG>.c
    ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_ConfigurationManager.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/main.c 
    
    ${CMAKE_CURRENT_SOURCE_DIR}/target_BlockStorage.c
    ${CMAKE_CURRENT_SOURCE_DIR}/target_common.c
    
    ${TARGET_FREERTOS_COMMON_SOURCES}
    ${TARGET_FREERTOS_NANOCLR_SOURCES}
    ${TARGET_MEDIATEK_COMMON_SOURCES}
    ${TARGET_MEDIATEK_NANOCLR_SOURCES}
    # ${NANOCLR_PROJECT_SOURCES}

    ${FREERTOS_SOURCES}

    ${WireProtocol_SOURCES}

    # sources for nanoFramework libraries
    ${NF_CoreCLR_SOURCES}
    ${NF_Debugger_SOURCES}
    ${NF_Diagnostics_SOURCES}

    # # sources for FatFS
    # "${FATFS_SOURCES}"

    # # sources for nanoFramework Network LWIP, Sockets and TLS
    # "${LWIP_SOURCES}"
    # "${NF_Networking_SOURCES}"
    # #"${mbedTLS_SOURCES}"

    # sources for nanoFramework APIs
    ${TARGET_NANO_APIS_SOURCES}
)

# add dependency from FreeRTOS (this is required to make sure the FreeRTOS repo is downloaded before the build starts)
# add_dependencies(${NANOBOOTER_PROJECT_NAME}.elf FreeRTOS CMSIS)
add_dependencies(${PROJECT_NAME} FreeRTOS)


# include common directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TARGET_FREERTOS_COMMON_INCLUDE_DIRS}
    ${TARGET_MEDIATEK_COMMON_INCLUDE_DIRS}
    ${TARGET_MEDIATEK_NANOCLR_INCLUDE_DIRS}

    ${PROJECT_SOURCE_DIR}/src/CLR/Core
    ${PROJECT_SOURCE_DIR}/src/CLR/Include
    ${PROJECT_SOURCE_DIR}/src/HAL/Include
    ${PROJECT_SOURCE_DIR}/src/PAL/Include
    ${PROJECT_SOURCE_DIR}/src/CLR/Runtime.Native

    # ${WireProtocol_INCLUDE_DIRS}
    ${FREERTOS_INCLUDE_DIRS}
    # ${CMSIS_INCLUDE_DIRS}
)

# include directories for nanoCLR
target_include_directories(${PROJECT_NAME} PUBLIC
    # ${CMAKE_CURRENT_BINARY_DIR}/nanoCLR
    # ${CMAKE_CURRENT_SOURCE_DIR}/nanoCLR

    # ${TARGET_CMSIS_NANOCLR_INCLUDE_DIRS}
    # ${TARGET_CHIBIOS_NANOCLR_INCLUDE_DIRS}

    # directories for nanoFramework libraries
    ${NF_CoreCLR_INCLUDE_DIRS}
    ${NF_Debugger_INCLUDE_DIRS}
    ${NF_Diagnostics_INCLUDE_DIRS}
    
    # includes for nanoFramework APIs
    ${TARGET_NANO_APIS_INCLUDES}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME})
# SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

# set compiler options
nf_set_compiler_options(${PROJECT_NAME})

# set compiler definitions
nf_set_compiler_definitions(${PROJECT_NAME})

# set linker files
if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    nf_set_linker_file(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
else()
    nf_set_linker_file(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
endif()

# set linker options
nf_set_linker_options(${PROJECT_NAME})

# Add MakeImage post-build command
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/AzureSphereMakeImageNF.cmake")
