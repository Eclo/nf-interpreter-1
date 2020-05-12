//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <Target_BlockStorage_TM4CFlashDriver.h>

// local defines
#define FLASH_ERASED_WORD        0x0FFFFFFFFU

bool TM4CFlashDriver_InitializeDevice(void* context)
{
    (void)context;

    // SimpleLink driver takes care of this, so always true
    return true;
}

bool TM4CFlashDriver_UninitializeDevice(void* context)
{
    (void)context;

    // SimpleLink driver takes care of this, so always true
    return true;
}

DeviceBlockInfo* TM4CFlashDriver_GetDeviceInfo(void* context)
{
    
    MEMORY_MAPPED_NOR_BLOCK_CONFIG* config = context;
    
    return config->BlockConfig.BlockDeviceInformation;  
}

bool TM4CFlashDriver_Read(void* context, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer)
{
    (void)context;

    // direct read from memory...
    memcpy(buffer, (void *)startAddress, (size_t)numBytes);

    // ... and always return true
    return true;
}

bool TM4CFlashDriver_Write(void* context, ByteAddress startAddress, unsigned int numBytes, unsigned char* buffer, bool readModifyWrite)
{
    (void)context;
    (void)readModifyWrite;
(void)startAddress;
(void)numBytes;
(void)readModifyWrite;

// TODO 
return true;
    //return stm32FlashWrite(startAddress, numBytes, buffer);
}

bool TM4CFlashDriver_IsBlockErased(void* context, ByteAddress blockAddress, unsigned int length)
{
	(void)context;

    unsigned long* cursor = (unsigned long *)blockAddress;
    unsigned long* endAddress = (unsigned long *)(blockAddress + length);

    // an erased flash address has to read FLASH_ERASED_WORD
    // OK to check by word (32 bits) because the erase is performed by 'block' whose size is word multiple
    while(cursor < endAddress)
    {
        if(*cursor++ != FLASH_ERASED_WORD)
        {
            // found an address with something other than FLASH_ERASED_WORD!!
            return false;
        }
    }
    
    // reached here so the block must be erased
    return true;
}

bool TM4CFlashDriver_EraseBlock(void* context, ByteAddress address)
{
    (void)context;
(void)address;
    // TODO return stm32FlashErase(address);
}
