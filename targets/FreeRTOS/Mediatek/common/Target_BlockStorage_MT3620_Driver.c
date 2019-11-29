//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <Target_BlockStorage_MT3620_Driver.h>


// local defines
#define FLASH_ERASED_WORD        0x0FFFFFFFFU

bool MT3620Driver_InitializeDevice(void* context)
{
	(void)context;

    // Azure SDK takes care of this, so always true
	return true;
}

bool MT3620Driver_UninitializeDevice(void* context)
{
	(void)context;

    // Azure SDK takes care of this, so always true
	return true;
}

DeviceBlockInfo* MT3620Driver_GetDeviceInfo(void* context)
{
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = context;
    
    return config->BlockConfig.BlockDeviceInformation;  
}

bool MT3620Driver_Read(void* context, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer)
{
	(void)context;
    (void)startAddress;
    (void)numBytes;
    (void)buffer;

    // ... and always return true
    return true;
}

bool MT3620Driver_Write(void* context, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer, bool readModifyWrite)
{
	(void)context;
	(void)readModifyWrite;
    (void)startAddress;
    (void)numBytes;
    (void)buffer;

	// if(FlashProgram((unsigned long*)buffer, (unsigned long) startAddress, (unsigned long) numBytes) == 0)
	// {
	// 	return true;
	// }
	
	return false;
}

bool MT3620Driver_IsBlockErased(void* context, ByteAddress blockAddress, unsigned int length)
{
	(void)context;
    (void)blockAddress;
    (void)length;

    // unsigned long* cursor = (unsigned long *)blockAddress;
    // unsigned long* endAddress = (unsigned long *)(blockAddress + length);

    // // an erased flash address has to read FLASH_ERASED_WORD
    // // OK to check by word (32 bits) because the erase is performed by 'block' whose size is word multiple
    // while(cursor < endAddress)
    // {
    //     if(*cursor++ != FLASH_ERASED_WORD)
    //     {
    //         // found an address with something other than FLASH_ERASED_WORD!!
    //         return false;
    //     }
    // }
    
    // reached here so the block must be erased
    return true;
}

bool MT3620Driver_EraseBlock(void* context, ByteAddress address)
{
	(void)context;
    (void)address;

	// if(FlashErase((unsigned long) address) == 0)
	// {
	// 	return true;
	// }

	return false;
}
