#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# grab the board name from the target name
# need to strip out the leading 'TI_' like in 'TI_TM4C129EXL'

string(REPLACE "TI_" "" TARGET_NAME_SHORT "${TARGET_NAME}")

# includes for SimpleLink CC32xx SDK
list(APPEND TI_TM4C_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/TivaWare_Source)
list(APPEND TI_TM4C_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/TivaWare_Source/driverlib)
list(APPEND TI_TM4C_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib)
list(APPEND TI_TM4C_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib/device)
list(APPEND TI_TM4C_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib/host)

# includes for nanoFramework SSL driver
# list(APPEND TI_TM4C_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/PAL/Com/sockets/ssl")

# source file for nanoFramework SSL driver
# list(APPEND TI_TM4C_SOURCES ${PROJECT_SOURCE_DIR}/src/PAL/COM/sockets/ssl/ssl.cpp)


set(TI_TM4C_SRCS

    adc.c
    aes.c
    can.c
    comp.c
    cpu.c
    crc.c
    des.c
    eeprom.c
    emac.c
    epi.c
    flash.c
    fpu.c
    gpio.c
    hibernate.c
    i2c.c
    interrupt.c
    lcd.c
    mpu.c
    onewire.c
    pwm.c
    qei.c
    shamd5.c
    ssi.c
    sw_crc.c
    sysctl.c
    sysexc.c
    systick.c
    timer.c
    uart.c
    udma.c
    usb.c
    watchdog.c

)

foreach(SRC_FILE ${TI_TM4C_SRCS})
    set(TI_TM4C_SRC_FILE SRC_FILE-NOTFOUND)

    find_file(TI_TM4C_SRC_FILE ${SRC_FILE}
        PATHS 

        ${PROJECT_BINARY_DIR}/TivaWare_Source/driverlib
        ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib
        ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib/device
        ${PROJECT_BINARY_DIR}/TivaWare_Source/usblib/host

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${TI_TM4C_SRC_FILE}") # debug helper
    list(APPEND TI_TM4C_SOURCES ${TI_TM4C_SRC_FILE})
endforeach()
