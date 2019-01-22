# set include directories for ChibiOS nanoFramework Overlay
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include)

####################################################################################
# WHEN ADDING A NEW CHIBIOS OVERLAY component add the include directory(ies) bellow 
####################################################################################
# component STM32_FLASH
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_flash)
# component STM32_CRC
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_crc)
# component STM32_RNG
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_rng)
# component STM32_FSMC (Flexible Memory Controller)
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_fsmc)
# component STM32_ONEWIRE (One Wire driver)
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_onewire)
# component STM32_USB_MSD (USB MSD driver)
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/stm32_usb_msd)
list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/various)

##################################################################################################################################
# Add above the required include directory(ies) for a new nanoFramework overlay component that you are adding
# following the template bellow. 
#
# If the component includes a low level driver specif for a series make sure you add the include dirs 
# in the CHIBIOS_NNNNNN_sources.cmake
#
# list(APPEND ChibiOSnfOverlay_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/include/<path-here>)
##################################################################################################################################


##################################################
# source files for ChibiOS nanoFramework Overlay
##################################################
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/hal_community.c)

####################################################################################################
# WHEN ADDING A NEW CHIBIOS OVERLAY component add the source file(s) specific to this series bellow 
####################################################################################################
# component STM32_FLASH
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_flash/hal_stm32_flash.c)
# component STM32_CRC
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_crc/hal_stm32_crc.c)
# component STM32_RNG
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_rng/hal_stm32_rng.c) 
# component STM32_FSMC (Flexible Memory Controller)
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_fsmc/hal_stm32_fsmc.c)
# component STM32_ONEWIRE (One Wire driver)
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_onewire/hal_stm32_onewire.c)
# component STM32_USB_MSD (USB MSD driver)
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/stm32_usb_msd/hal_stm32_usb_msd.c)
list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/various/lib_scsi.c)

#########################################################################################################################
# Add above ALL the source code file(s) required for a new nanoFramework overlay component that you are adding
# following the template bellow.
#
# If the component includes a low level driver specif for a series make sure you add the source files 
# in the CHIBIOS_NNNNNN_sources.cmake
#
# list(APPEND ChibiOSnfOverlay_SOURCES ${PROJECT_SOURCE_DIR}/targets/CMSIS-OS/ChibiOS/nf-overlay/os/hal/src/<path-here>)
#########################################################################################################################


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ChibiOSnfOverlay DEFAULT_MSG ChibiOSnfOverlay_INCLUDE_DIRS ChibiOSnfOverlay_SOURCES)
