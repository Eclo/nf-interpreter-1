//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoPAL_BlockStorage.h>
#include <Target_BlockStorage_MT3620_Driver.h>

// map here the Block Storage Interface to the STM32 driver
IBlockStorageDevice MT3620_BlockStorageInterface =
{                          
    &MT3620Driver_InitializeDevice,
    &MT3620Driver_UninitializeDevice,
    &MT3620Driver_GetDeviceInfo,
    &MT3620Driver_Read,
    &MT3620Driver_Write,
    NULL,
    &MT3620Driver_IsBlockErased,    
    &MT3620Driver_EraseBlock,
    NULL,
    NULL
};
