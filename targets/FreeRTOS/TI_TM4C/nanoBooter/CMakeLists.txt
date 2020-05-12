#
# Copyright (c) 2017 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# append target TI_TM4C nanoBooter source files 
list(APPEND TARGET_TI_TM4C_NANOBOOTER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_App_Interface.c")
list(APPEND TARGET_TI_TM4C_NANOBOOTER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/WireProtocol_MonitorCommands.c")
# make var global
set(TARGET_TI_TM4C_NANOBOOTER_SOURCES ${TARGET_TI_TM4C_NANOBOOTER_SOURCES} CACHE INTERNAL "make global")

# append include directory for debugger
list(APPEND TARGET_TI_TM4C_NANOBOOTER_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/CLR/Debugger")
    
# append target TI_TM4C nanoBooter include directory
list(APPEND TARGET_TI_TM4C_NANOBOOTER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
# make var global
set(TARGET_TI_TM4C_NANOBOOTER_INCLUDE_DIRS ${TARGET_TI_TM4C_NANOBOOTER_INCLUDE_DIRS} CACHE INTERNAL "make global")
