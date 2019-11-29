//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#ifndef _TARGET_MT3620_DRIVER_H_
#define _TARGET_MT3620_DRIVER_H_ 1

#include <nanoPAL_BlockStorage.h>

#ifdef __cplusplus
extern "C" {
#endif

bool MT3620Driver_InitializeDevice(void*);
bool MT3620Driver_UninitializeDevice(void*);
DeviceBlockInfo* MT3620Driver_GetDeviceInfo(void*);
bool MT3620Driver_Read(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer);
bool MT3620Driver_Write(void*, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer, bool readModifyWrite);
bool MT3620Driver_IsBlockErased(void*, ByteAddress blockAddress, unsigned int length);
bool MT3620Driver_EraseBlock(void*, ByteAddress address);
void MT3620Driver_SetPowerState(void*, unsigned int state);

#ifdef __cplusplus
}
#endif

#endif  //_TARGET_MT3620_DRIVER_H_
