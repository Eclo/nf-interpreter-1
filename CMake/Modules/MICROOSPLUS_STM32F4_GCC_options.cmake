# set(CMAKE_C_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -Wall -std=gnu11 -w -ffunction-sections -fdata-sections -fomit-frame-pointer -mlong-calls -fdollars-in-identifiers -mabi=aapcs -fno-exceptions -fno-unroll-loops -mstructure-size-boundary=8 -ffast-math -ftree-vectorize -specs=nano.specs" CACHE INTERNAL "c compiler flags")

set(CMAKE_C_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -Wall -std=gnu11 -w -ffunction-sections -fdata-sections -mabi=aapcs -fno-exceptions -ffast-math -specs=nano.specs" CACHE INTERNAL "c compiler flags")


# set(CMAKE_CXX_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -Wall -std=gnu++14 -w -fcheck-new -fno-rtti  -fno-use-cxa-atexit -fno-threadsafe-statics -ffunction-sections -fdata-sections -fomit-frame-pointer -mlong-calls -fdollars-in-identifiers -mabi=aapcs # -fno-exceptions -fno-unroll-loops -mstructure-size-boundary=8 -ffast-math -ftree-vectorize -specs=nano.specs" CACHE INTERNAL "cxx compiler flags")

set(CMAKE_CXX_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -Wall -std=gnu++14 -w -fcheck-new -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -ffunction-sections -fdata-sections -mabi=aapcs -fno-exceptions -ffast-math -specs=nano.specs" CACHE INTERNAL "cxx compiler flags")

# set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")

set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")


# set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "executable linker flags")
# set(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "module linker flags")
# set(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "shared linker flags")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -mthumb -nostartfiles -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "executable linker flags")
set(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -nostartfiles -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "module linker flags")
set(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -nostartfiles -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mabi=aapcs" CACHE INTERNAL "shared linker flags")


set(STM32_CHIP_TYPES 405xx 415xx 407xx 417xx 427xx 437xx 429xx 439xx 446xx 401xC 401xE 411xE CACHE INTERNAL "stm32f4 chip types")
set(STM32_CODES "405.." "415.." "407.." "417.." "427.." "437.." "429.." "439.." "446.." "401.[CB]" "401.[ED]" "411.[ED]")

macro(STM32_GET_CHIP_TYPE CHIP CHIP_TYPE)
    string(REGEX REPLACE "^[S][T][M]32[F](4[01234][15679].[BCEGI]).*$" "\\1" STM32_CODE ${CHIP})
    set(INDEX 0)
    foreach(C_TYPE ${STM32_CHIP_TYPES})
        list(GET STM32_CODES ${INDEX} CHIP_TYPE_REGEXP)
        if(STM32_CODE MATCHES ${CHIP_TYPE_REGEXP})
            set(RESULT_TYPE ${C_TYPE})
        endif()
        math(EXPR INDEX "${INDEX}+1")
    endforeach()
    set(${CHIP_TYPE} ${RESULT_TYPE})
endmacro()

macro(STM32_GET_CHIP_PARAMETERS CHIP FLASH_SIZE RAM_SIZE)
    string(REGEX REPLACE "^[S][T][M]32[F](4[01234][15679].[BCEGI]).*$" "\\1" STM32_CODE ${CHIP})
    string(REGEX REPLACE "^[S][T][M]32[F]4[01234][15679].([BCEGI]).*$" "\\1" STM32_SIZE_CODE ${CHIP})
    
    if(STM32_SIZE_CODE STREQUAL "B")
        set(FLASH "128K")
    elseif(STM32_SIZE_CODE STREQUAL "C")
        set(FLASH "256K")
    elseif(STM32_SIZE_CODE STREQUAL "E")
        set(FLASH "512K")
    elseif(STM32_SIZE_CODE STREQUAL "G")
        set(FLASH "1024K")
    elseif(STM32_SIZE_CODE STREQUAL "I")
        set(FLASH "2048K")
    endif()
    
    STM32_GET_CHIP_TYPE(${CHIP} TYPE)
    
    if(${TYPE} STREQUAL "401xC")
        set(RAM "64K")
    elseif(${TYPE} STREQUAL "401xE")
        set(RAM "96K")
    elseif(${TYPE} STREQUAL "411xE")
        set(RAM "128K")
    elseif(${TYPE} STREQUAL "405xx")
        set(RAM "128K")
    elseif(${TYPE} STREQUAL "415xx")
        set(RAM "128K")
    elseif(${TYPE} STREQUAL "407xx")
        set(RAM "128K")
    elseif(${TYPE} STREQUAL "417xx")
        set(RAM "128K")
    elseif(${TYPE} STREQUAL "427xx")
        set(RAM "192K")
    elseif(${TYPE} STREQUAL "437xx")
        set(RAM "192K")
    elseif(${TYPE} STREQUAL "429xx")
        set(RAM "192K")
    elseif(${TYPE} STREQUAL "439xx")
        set(RAM "192K")
    elseif(${TYPE} STREQUAL "446xx")
        set(RAM "128K")
    endif()
    
    set(${FLASH_SIZE} ${FLASH})
    set(${RAM_SIZE} ${RAM})
endmacro()

function(STM32_SET_CHIP_DEFINITIONS TARGET CHIP_TYPE)
    list(FIND STM32_CHIP_TYPES ${CHIP_TYPE} TYPE_INDEX)
    if(TYPE_INDEX EQUAL -1)
        MESSAGE(FATAL_ERROR "${CHIP_TYPE} is not supported.")
    endif()
    get_target_property(TARGET_DEFS ${TARGET} COMPILE_DEFINITIONS)
    
    if(TARGET_DEFS)
        set(TARGET_DEFS "STM32F4;STM32F${CHIP_TYPE};${TARGET_DEFS}")
    else()
        set(TARGET_DEFS "STM32F4;STM32F${CHIP_TYPE}")
    endif()
        
    set_target_properties(${TARGET} PROPERTIES COMPILE_DEFINITIONS "${TARGET_DEFS}")
endfunction()
