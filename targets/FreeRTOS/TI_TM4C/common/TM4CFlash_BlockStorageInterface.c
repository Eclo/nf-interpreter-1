//
// Copyright (c) 2020 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoPAL_BlockStorage.h>
#include <Target_BlockStorage_TM4CFlashDriver.h>

// map here the Block Storage Interface to the TI TM4C driver
IBlockStorageDevice TM4CFlash_BlockStorageInterface =
{                          
    &TM4CFlashDriver_InitializeDevice,
    &TM4CFlashDriver_UninitializeDevice,
    &TM4CFlashDriver_GetDeviceInfo,
    &TM4CFlashDriver_Read,
    &TM4CFlashDriver_Write,
    NULL,
    &TM4CFlashDriver_IsBlockErased,    
    &TM4CFlashDriver_EraseBlock,
    NULL,
    NULL
};
