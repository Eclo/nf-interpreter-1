//
// Copyright (c) 2020 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#ifndef _TARGET_IMXRTFLASH_DRIVER_H_
#define _TARGET_IMXRTFLASH_DRIVER_H_ 1

#include <nanoPAL_BlockStorage.h>

#ifdef __cplusplus
extern "C" {
#endif

bool TM4CFlashDriver_InitializeDevice(void*);
bool TM4CFlashDriver_UninitializeDevice(void*);
DeviceBlockInfo* TM4CFlashDriver_GetDeviceInfo(void*);
bool TM4CFlashDriver_Read(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer);
bool TM4CFlashDriver_Write(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer, bool readModifyWrite);
bool TM4CFlashDriver_IsBlockErased(void*, ByteAddress blockAddress, unsigned int length);
bool TM4CFlashDriver_EraseBlock(void*, ByteAddress address);
void TM4CFlashDriver_SetPowerState(void*, unsigned int state);

#ifdef __cplusplus
}
#endif

#endif  //_TARGET_IMXRTFLASH_DRIVER_H_
