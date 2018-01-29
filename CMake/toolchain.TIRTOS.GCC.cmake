#
# Copyright (c) 2017 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

INCLUDE(CMakeForceCompiler)
# if( DEFINED CMAKE_CROSSCOMPILING )
#     # subsequent toolchain loading is not really needed
#     return()
# endif()


###################################################
# set C and C++ flags for compiler detection tests
###################################################
# IMPORTANT: these flags need to be removed after 
# the compiler detection otherwise they'll get 
# added to all compiler build calls and 
# will colide with the use of nano.specs
###################################################
set(CMAKE_C_FLAGS_INIT "-specs=nosys.specs")
set(CMAKE_CXX_FLAGS_INIT "-specs=nosys.specs")
###################################################


# set toolchain directories
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
set(TOOLCHAIN_INC_DIR ${TOOLCHAIN_PREFIX}/arm-none-eabi/include)
set(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/arm-none-eabi/lib)

# the name of the operating system for which CMake is to build
set(CMAKE_SYSTEM_NAME Generic)

# name of the CPU CMake is building for
set(CMAKE_SYSTEM_PROCESSOR arm)

# macro to setup compilers
macro(SET_COMPILER_VAR var name)
   find_program(CMAKE_${var} arm-none-eabi-${name} HINTS ${TOOLCHAIN_BIN_DIR} DOC "${name} tool")
endmacro()

# setup C compiler
if(NOT CMAKE_C_COMPILER)
    SET_COMPILER_VAR(C_COMPILER gcc)
endif()

# setup C++ compiler
if(NOT CMAKE_CXX_COMPILER)
    SET_COMPILER_VAR(CXX_COMPILER g++)
endif()

# setup Assembler compiler
SET_COMPILER_VAR(ASM-ATT_COMPILER as)

# other toolchain configurations
set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-objdump CACHE INTERNAL "objdump tool")
set(CMAKE_SIZE ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-size CACHE INTERNAL "size tool")
set(CMAKE_DEBUGER ${TOOLCHAIN_BIN_DIR}/arm-none-eabi-gdb CACHE INTERNAL "debuger")

# root paths to search on the filesystem for cross-compiling
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PREFIX}/arm-none-eabi ${EXTRA_FIND_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# set required C and C++ standard for ALL targets
set(CMAKE_C_STANDARD 11 CACHE INTERNAL "C standard for all targets")
set(CMAKE_CXX_STANDARD 11 CACHE INTERNAL "C++ standard for all targets")

# set all C and C++ extensions to be OFF on ALL targets
# this forces the use of -std=c11 and -std=c++11 instead of -std=gnu11 and -std=gnu++11
set(CMAKE_C_EXTENSIONS OFF CACHE INTERNAL "C compiler extensions OFF")
set(CMAKE_CXX_EXTENSIONS OFF CACHE INTERNAL "C++ compiler extensions OFF")


function(NF_ADD_HEX_BIN_DUMP_TARGETS TARGET)
    if(EXECUTABLE_OUTPUT_PATH)
        set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
        set(FILENAME "${TARGET}")
    endif()

    # add targets for HEX, BIN and S19 formats with no output so they will always be built
    add_custom_target(${TARGET}.hex DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} -Oihex ${FILENAME} ${FILENAME}.hex)
    add_custom_target(${TARGET}.s19 DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} -Osrec ${FILENAME} ${FILENAME}.s19)
    add_custom_target(${TARGET}.bin DEPENDS ${TARGET} COMMAND ${CMAKE_OBJCOPY} -Obinary ${FILENAME} ${FILENAME}.bin)
    add_custom_target(${TARGET}.dump DEPENDS ${TARGET} COMMAND ${CMAKE_OBJDUMP} -d -EL -S ${FILENAME} ${FILENAME}.dump)
endfunction()


function(NF_PRINT_SIZE_OF_TARGETS TARGET)
    if(EXECUTABLE_OUTPUT_PATH)
      set(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}")
    else()
      set(FILENAME "${TARGET}")
    endif()
    add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_SIZE} ${FILENAME})
endfunction()


function(NF_SET_OPTIMIZATION_OPTIONS TARGET) 

    target_compile_options(${TARGET} PRIVATE
        $<$<CONFIG:Debug>:-Og -femit-class-debug-always -g3 -ggdb>
        $<$<CONFIG:Release>:-O3 -flto -fno-strict-aliasing>
        $<$<CONFIG:MinSizeRel>:-Os -flto -fno-strict-aliasing>
        $<$<CONFIG:RelWithDebInfo>:-Os -femit-class-debug-always -g3 -ggdb>
    )

endfunction()


function(NF_SET_LINK_MAP TARGET) 

    # need to remove the .elf suffix from target name
    string(FIND ${TARGET} "." TARGET_EXTENSION_DOT_INDEX)
    string(SUBSTRING ${TARGET} 0 ${TARGET_EXTENSION_DOT_INDEX} TARGET_SHORT)
    
    # add linker flags to generate map file
    set_property(TARGET ${TARGET_SHORT}.elf APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${TARGET_SHORT}.map,--library-path=${PROJECT_SOURCE_DIR}/targets/TI-RTOS/CC3200/nanoCLR")

endfunction()


function(NF_SET_COMPILER_DEFINITIONS TARGET)

    # definition for platform (always ARM here)
    target_compile_definitions(${TARGET} PUBLIC "-DPLATFORM_ARM ")

    # definition for platform 
    target_compile_definitions(${TARGET} PUBLIC "-Dgcc -DPART_CC3200 -DUSE_TIRTOS -DCCWARE -DSL_FULL -DSL_PLATFORM_MULTI_THREADED -DTARGET_IS_CC3200 -Dxdc_target_types__=target_types.h")

    # set compiler definitions related with the build type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        # build types that include debug have the define 'NANOCLR_ENABLE_SOURCELEVELDEBUGGING'
        target_compile_definitions(${TARGET} PUBLIC "-DNANOCLR_ENABLE_SOURCELEVELDEBUGGING ")
    endif()

    # set compiler definition for RTM build option
    if(NF_BUILD_RTM)
        target_compile_definitions(${TARGET} PUBLIC BUILD_RTM)
    endif()

    # set compiler definition for platform emulated floating point according to FPU
    # no FPU requires FP emulation from the platform
    target_compile_definitions(${TARGET} PUBLIC $<$<NOT:$<BOOL:USE_FPU_IS_TRUE>>:-DPLATFORM_EMULATED_FLOATINGPOINT>)

    # set compiler definition for CORTEX according to FPU
    target_compile_definitions(${TARGET} PUBLIC -DCORTEX_USE_FPU=$<$<BOOL:USE_FPU_IS_TRUE>:TRUE>$<$<NOT:$<BOOL:USE_FPU_IS_TRUE>>:FALSE>)

    # set compiler definition for using Application Domains feature
    if(NF_FEATURE_USE_APPDOMAINS)
        target_compile_definitions(${TARGET} PUBLIC -DNANOCLR_USE_APPDOMAINS)
    endif()

    # set definition for Wire Protocol trace mask
    target_compile_definitions(${TARGET} PUBLIC -DTRACE_MASK=${WP_TRACE_MASK})

endfunction()


function(NF_SET_LINKER_FILE TARGET LINKER_FILE_NAME)

    # set linker file name
    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "-T${LINKER_FILE_NAME}")

endfunction()