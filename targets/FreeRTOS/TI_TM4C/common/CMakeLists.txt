#
# Copyright (c) 2020 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# append include directory for platform TI TM4C
list(APPEND TARGET_TI_TM4C_COMMON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

# append common source files
# list(APPEND TARGET_TI_TM4C_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_HAL_Interface.c")

# append Target files
list(APPEND TARGET_TI_TM4C_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Target_BlockStorage_TM4CFlashDriver.c")
list(APPEND TARGET_TI_TM4C_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/TM4CFlash_BlockStorageInterface.c")

# include configuration manager file, if feature is enabled
if(NF_FEATURE_HAS_CONFIG_BLOCK)
    list(APPEND TARGET_TI_TM4C_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_ConfigurationManager.cpp")
endif()

# make var global
set(TARGET_TI_TM4C_COMMON_SOURCES ${TARGET_TI_TM4C_COMMON_SOURCES} CACHE INTERNAL "make global")
set(TARGET_TI_TM4C_COMMON_INCLUDE_DIRS ${TARGET_TI_TM4C_COMMON_INCLUDE_DIRS} CACHE INTERNAL "make global")
