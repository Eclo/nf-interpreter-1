//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>
#include <nanoHAL_v2.h>
#include <nanoWeak.h>

#include <ti/drivers/net/wifi/simplelink.h>

// we'll store up to 4 network configuration blocks
#define NETWORK_CONFIG_MAX_COUNT            (4)
// files with network configuration have this namming patter "network-config-N.bin"
#define NETWORK_CONFIG_FILE_NAME            "nf/network-config0.bin"
// position of the index in the file name above (that's the 0)
#define NETWORK_CONFIG_FILE_INDEX_POSITION  (17)

typedef struct
{
    SlFileAttributes_t attribute;
    char fileName[SL_FS_MAX_FILE_NAME_LENGTH];
}slGetfileList_t;

// This configuration manager implementation is valid for CC32xx devices.
// Because everything that is meant to be stored in the configure block is handled by the SimpleLink 
// persistent storage, this code is either empty or acts as a proxy to the SimpleLink API

// network configuration blocks are stored as files in file storage
void* ConfigurationManagerCC32xx_FindNetworkConfigurationBlocks()
{
    int32_t ret = 1;
    int32_t retGetFileList = 1;
    slGetfileList_t* fileList;
    int32_t index = -1;

    int32_t i;
    uint32_t fileCount = 0;

    // allocate memory for file list buffer
    fileList = (slGetfileList_t*)platform_malloc(sizeof(slGetfileList_t) * NETWORK_CONFIG_MAX_COUNT);

    // check succesfull malloc
    if (fileList == NULL)
    {
        return NULL;
    }

    // clear memory
    memset(fileList, 0x0, sizeof(sizeof(SlFileAttributes_t) * NETWORK_CONFIG_MAX_COUNT));

    // first pass: find out how many files of this type we have
    while(retGetFileList > 0)
    {
        retGetFileList = sl_FsGetFileList( &index, NETWORK_CONFIG_MAX_COUNT, 
                                                (uint8_t)(SL_FS_MAX_FILE_NAME_LENGTH + sizeof(SlFileAttributes_t)),
                                                (unsigned char*)fileList, SL_FS_GET_FILE_ATTRIBUTES);
        if (retGetFileList < 0)
        {
            // error getting file list, or no more files
            break;
        }
        
        for (i = 0; i < retGetFileList; i++)
        {
            // check file name
            if(memcmp(fileList[i].fileName, NETWORK_CONFIG_FILE_NAME, sizeof(NETWORK_CONFIG_FILE_NAME)) == 0)
            {
                fileCount++;
            }
        }
    }

    // allocate config struct
    HAL_CONFIGURATION_NETWORK *networkConfigs = (HAL_CONFIGURATION_NETWORK *)platform_malloc(offsetof(HAL_CONFIGURATION_NETWORK, Configs) + fileCount * sizeof(networkConfigs->Configs[0]));
    // set collection count
    networkConfigs->Count = fileCount;

    // clear memory for file list
    platform_free(fileList);

    return networkConfigs;
}

// wireless profiles are stored as SimpleLink WLAN profile 
void* ConfigurationManagerCC32xx_FindNetworkWireless80211ConfigurationBlocks()
{
    uint16_t index, status;
    signed char name[32];
    int16_t nameLength;
    unsigned char macAddr[6];
    SlWlanSecParams_t secParams;
    SlWlanGetSecParamsExt_t secExtParams;
    uint32_t priority;
    uint16_t profileCount = 0;

    // first pass: find out how many profiles are stored
    for(index = 0; index < SL_WLAN_MAX_PROFILES; index++)
    {
        status = sl_WlanProfileGet(index, name, &nameLength, macAddr, &secParams, &secExtParams, &priority);
        if( status > 0)
        {
            profileCount++;
        }
    }

    // allocate config struct
    HAL_CONFIGURATION_NETWORK_WIRELESS80211 *networkWirelessConfigs = (HAL_CONFIGURATION_NETWORK_WIRELESS80211 *)platform_malloc(offsetof(HAL_CONFIGURATION_NETWORK_WIRELESS80211, Configs) + profileCount * sizeof(networkWirelessConfigs->Configs[0]));

    // set collection count
    networkWirelessConfigs->Count = profileCount;

    return networkWirelessConfigs;
}

// initialization of configuration manager
void ConfigurationManager_Initialize()
{
    // enumerate the blocks
    ConfigurationManager_EnumerateConfigurationBlocks();
};

// Enumerates the configuration blocks from the configuration flash sector 
void ConfigurationManager_EnumerateConfigurationBlocks()
{
    // find network configuration blocks
    HAL_CONFIGURATION_NETWORK* networkConfigs = (HAL_CONFIGURATION_NETWORK*)ConfigurationManagerCC32xx_FindNetworkConfigurationBlocks();

    // find wireless 80211 network configuration blocks
    HAL_CONFIGURATION_NETWORK_WIRELESS80211* networkWirelessConfigs = (HAL_CONFIGURATION_NETWORK_WIRELESS80211*)ConfigurationManagerCC32xx_FindNetworkWireless80211ConfigurationBlocks();

    // // find X509 certificate blocks
    // HAL_CONFIGURATION_X509_CERTIFICATE* certificateStore = (HAL_CONFIGURATION_X509_CERTIFICATE*)ConfigurationManager_FindX509CertificateConfigurationBlocks((uint32_t)&__nanoConfig_start__, (uint32_t)&__nanoConfig_end__);

    // alloc memory for g_TargetConfiguration
    // because this is a struct of structs that use flexible members the memory has to be allocated from the heap
    // the malloc size for each struct is computed separately 
    uint32_t sizeOfNetworkInterfaceConfigs = offsetof(HAL_CONFIGURATION_NETWORK, Configs) + networkConfigs->Count * sizeof(networkConfigs->Configs[0]);
    uint32_t sizeOfWireless80211Configs = offsetof(HAL_CONFIGURATION_NETWORK_WIRELESS80211, Configs) + networkWirelessConfigs->Count * sizeof(networkWirelessConfigs->Configs[0]);
    // uint32_t sizeOfX509CertificateStore = offsetof(HAL_CONFIGURATION_X509_CERTIFICATE, Certificates) + certificateStore->Count * sizeof(certificateStore->Certificates[0]);

    g_TargetConfiguration.NetworkInterfaceConfigs = (HAL_CONFIGURATION_NETWORK*)platform_malloc(sizeOfNetworkInterfaceConfigs);
    g_TargetConfiguration.Wireless80211Configs = (HAL_CONFIGURATION_NETWORK_WIRELESS80211*)platform_malloc(sizeOfWireless80211Configs);
    // g_TargetConfiguration.CertificateStore = (HAL_CONFIGURATION_X509_CERTIFICATE*)platform_malloc(sizeOfX509CertificateStore);

    // copy structs to g_TargetConfiguration
    memcpy((HAL_CONFIGURATION_NETWORK*)g_TargetConfiguration.NetworkInterfaceConfigs, networkConfigs, sizeOfNetworkInterfaceConfigs);
    memcpy((HAL_CONFIGURATION_NETWORK_WIRELESS80211*)g_TargetConfiguration.Wireless80211Configs, networkWirelessConfigs, sizeOfWireless80211Configs);
    // memcpy((HAL_CONFIGURATION_X509_CERTIFICATE*)g_TargetConfiguration.CertificateStore, certificateStore, sizeOfX509CertificateStore);

    // now free the memory of the original structs
    platform_free(networkConfigs);
    platform_free(networkWirelessConfigs);
    // platform_free(certificateStore);
}

// Gets the network configuration block from the configuration flash sector 
bool ConfigurationManager_GetConfigurationBlock(void* configurationBlock, DeviceConfigurationOption configuration, uint32_t configurationIndex)
{
    int sizeOfBlock = 0;

    // validate if the requested block exists
    // Count has to be non zero
    // requested Index has to exist (array index starts at zero, so need to add one)
    if(configuration == DeviceConfigurationOption_Network)
    {
        if(g_TargetConfiguration.NetworkInterfaceConfigs->Count == 0)
        {
            // there is no network config block, init one with default settings
            if(!InitialiseNetworkDefaultConfig((HAL_Configuration_NetworkInterface*)configurationBlock, 0))
            {
                return false;
            }
        }
        else
        {
            if((configurationIndex + 1) > g_TargetConfiguration.NetworkInterfaceConfigs->Count)
            {
                return false;
            }
        }

        // set block size
        sizeOfBlock = sizeof(HAL_Configuration_NetworkInterface);
    }
    else if(configuration == DeviceConfigurationOption_Wireless80211Network)
    {
        if(g_TargetConfiguration.Wireless80211Configs->Count == 0 ||
            (configurationIndex + 1) > g_TargetConfiguration.Wireless80211Configs->Count)
        {
            return false;
        }

        // set block size
        sizeOfBlock = sizeof(HAL_Configuration_Wireless80211);
    }
    else if(configuration == DeviceConfigurationOption_X509CaRootBundle)
    {
        if(g_TargetConfiguration.CertificateStore->Count == 0 ||
            (configurationIndex + 1) > g_TargetConfiguration.CertificateStore->Count)
        {
            return false;
        }

        // // get block address
        // blockAddress = (uint8_t*)g_TargetConfiguration.CertificateStore->Certificates[configurationIndex];

        // // set block size
        // // because X509 certificate has a variable length need to compute the block size in two steps
        // sizeOfBlock = offsetof(HAL_Configuration_X509CaRootBundle, Certificate);
        // sizeOfBlock += ((HAL_Configuration_X509CaRootBundle*)blockAddress)->CertificateSize;
    }

    return true;
}

// Stores the configuration block to the configuration flash sector
// NOTE: because inserting or removing a configuration block it's very 'RAM expensive' we choose not to support those operations
// the host debugger will have to be used to manage these operations on the device configuration collection 
bool ConfigurationManager_StoreConfigurationBlock(void* configurationBlock, DeviceConfigurationOption configuration, uint32_t configurationIndex, uint32_t blockSize, uint32_t offset)
{
    // ByteAddress storageAddress = 0;
    // bool requiresEnumeration = FALSE;
    bool success = FALSE;

    unsigned char* fileName = NULL;

    int32_t fileHandle;
    uint32_t token = 0;
    int32_t retVal;

    if(configuration == DeviceConfigurationOption_Network)
    {
        // network config blocks are stored as files

        // compose file name
        fileName = (unsigned char*)platform_malloc(sizeof(NETWORK_CONFIG_FILE_NAME));
        memcpy(fileName, NETWORK_CONFIG_FILE_NAME, sizeof(NETWORK_CONFIG_FILE_NAME));
        // insert index number at position N as char
        fileName[NETWORK_CONFIG_FILE_INDEX_POSITION] = '0' + configurationIndex;

        fileHandle = sl_FsOpen( fileName,
                                SL_FS_CREATE | SL_FS_OVERWRITE |
                                SL_FS_CREATE_MAX_SIZE(sizeof(HAL_Configuration_NetworkInterface)) |
                                SL_FS_CREATE_PUBLIC_WRITE | SL_FS_CREATE_NOSIGNATURE,
                                (uint32_t *)&token);
        
        // on error there is no file handle, rather a negative error code
        if(fileHandle > 0)
        {
            // make sure the config block marker is set
            memcpy(configurationBlock, c_MARKER_CONFIGURATION_NETWORK_V1, sizeof(c_MARKER_CONFIGURATION_NETWORK_V1)); 

            retVal = sl_FsWrite(fileHandle, 0, (unsigned char*)configurationBlock, sizeof(HAL_Configuration_NetworkInterface));
            
            // on success the return value is the amount of bytes written
            if(retVal == sizeof(HAL_Configuration_NetworkInterface))
            {
                retVal = sl_FsClose(fileHandle, 0, 0, 0);

                if( retVal < 0 )
                {
                    // error closing file, API ceremony suggests calling "abort" operation
                    uint8_t signature = 'A';
                    sl_FsClose(fileHandle, 0, &signature, 1);
                }
            }
        }

        if(fileName != NULL)
        {
            platform_free(fileName);
        }
    }


    // if(configuration == DeviceConfigurationOption_Network)
    // {
    //     if( g_TargetConfiguration.NetworkInterfaceConfigs->Count == 0 ||
    //         (configurationIndex + 1) > g_TargetConfiguration.NetworkInterfaceConfigs->Count)
    //     {
    //         // there is no room for this block, or there are no blocks stored at all
    //         // failing the operation
    //         return FALSE;
    //     }

    //     // set storage address from block address, plus the requested offset
    //     storageAddress = (ByteAddress)g_TargetConfiguration.NetworkInterfaceConfigs->Configs[configurationIndex] + offset;

    //     // set block size, in case it's not already set
    //     blockSize = sizeof(HAL_Configuration_NetworkInterface);

    //     // make sure the config block marker is set
    //     memcpy(configurationBlock, c_MARKER_CONFIGURATION_NETWORK_V1, sizeof(c_MARKER_CONFIGURATION_NETWORK_V1)); 
    
    //     _ASSERTE(((HAL_Configuration_NetworkInterface*)configurationBlock)->StartupAddressMode > 0);
    // }
    // else if(configuration == DeviceConfigurationOption_Wireless80211Network)
    // {
    //     if( g_TargetConfiguration.Wireless80211Configs->Count == 0 ||
    //         (configurationIndex + 1) > g_TargetConfiguration.Wireless80211Configs->Count)
    //     {
    //         // there is no room for this block, or there are no blocks stored at all
    //         // failing the operation
    //         return FALSE;
    //     }

    //     // set storage address from block address, plus the requested offset
    //     storageAddress = (ByteAddress)g_TargetConfiguration.Wireless80211Configs->Configs[configurationIndex] + offset;

    //     // set block size, in case it's not already set
    //     blockSize = sizeof(HAL_Configuration_Wireless80211);

    //     // make sure the config block marker is set
    //     memcpy(configurationBlock, c_MARKER_CONFIGURATION_WIRELESS80211_V1, sizeof(c_MARKER_CONFIGURATION_WIRELESS80211_V1));        
    // }
    // else if(configuration == DeviceConfigurationOption_X509CaRootBundle)
    // {
    //     if( g_TargetConfiguration.CertificateStore->Count == 0 ||
    //         (configurationIndex + 1) > g_TargetConfiguration.CertificateStore->Count)
    //     {
    //         // there is no room for this block, or there are no blocks stored at all
    //         // failing the operation
    //         return FALSE;
    //     }

    //     // set storage address from block address, plus the requested offset
    //     storageAddress = (ByteAddress)g_TargetConfiguration.CertificateStore->Certificates[configurationIndex] + offset;

    //     // set block size, in case it's not already set
    //     // because X509 certificate has a variable length need to compute the block size in two steps
    //     blockSize = offsetof(HAL_Configuration_X509CaRootBundle, Certificate);
    //     blockSize += ((HAL_Configuration_X509CaRootBundle*)configurationBlock)->CertificateSize;

    //     // make sure the config block marker is set
    //     memcpy(configurationBlock, c_MARKER_CONFIGURATION_X509CAROOTBUNDLE_V1, sizeof(c_MARKER_CONFIGURATION_X509CAROOTBUNDLE_V1));        
    // }
    // else if(configuration == DeviceConfigurationOption_All)
    // {
    //     // particular situation where we are receiving the full configuration block

    //     // set storage address as the start of the flash configuration sector, plus the requested offset
    //     storageAddress = (ByteAddress)&__nanoConfig_start__ + offset;

    //     // always enumerate the blocks again after storing it
    //     requiresEnumeration = TRUE;

    //     // for save all the block size has to be provided, check that 
    //     if(blockSize == 0)
    //     {
    //         return FALSE;
    //     }
    // }

    // // copy the config block content to the config block storage
    // success = STM32FlashDriver_Write(NULL, storageAddress, blockSize, (unsigned char*)configurationBlock, true);

    // if(success == TRUE && requiresEnumeration)
    // {
    //     // free the current allocation(s)
    //     platform_free(g_TargetConfiguration.NetworkInterfaceConfigs);
    //     platform_free(g_TargetConfiguration.Wireless80211Configs);
    //     platform_free(g_TargetConfiguration.CertificateStore);

    //     // perform enumeration of configuration blocks
    //     ConfigurationManager_EnumerateConfigurationBlocks();
    // }

    return success;
}

// Updates a configuration block
bool ConfigurationManager_UpdateConfigurationBlock(void* configurationBlock, DeviceConfigurationOption configuration, uint32_t configurationIndex)
{
    // ByteAddress storageAddress;
    // uint32_t blockOffset;
    // uint8_t* blockAddressInCopy;
    // uint32_t blockSize;
    bool success = FALSE;

    // // config sector size
    // int sizeOfConfigSector = (uint32_t)&__nanoConfig_end__ - (uint32_t)&__nanoConfig_start__;

    // // allocate memory from CRT heap
    // uint8_t* configSectorCopy = (uint8_t*)platform_malloc(sizeOfConfigSector);

    // if(configSectorCopy != NULL)
    // {
    //     // copy config sector from flash to RAM
    //     memcpy(configSectorCopy, &__nanoConfig_start__, sizeOfConfigSector);

    //     // find out the address for the config block to update in the configSectorCopy
    //     // because we are copying back the config block to flash and just replacing the config block content
    //     // the addresses in g_TargetConfiguration will remain the same
    //     // plus we can calculate the offset of the config block from g_TargetConfiguration
    //     if(configuration == DeviceConfigurationOption_Network)
    //     {
    //         // get storage address from block address
    //         storageAddress = (ByteAddress)g_TargetConfiguration.NetworkInterfaceConfigs->Configs[configurationIndex];

    //         // set block size, in case it's not already set
    //         blockSize = sizeof(HAL_Configuration_NetworkInterface);
                
    //         // make sure the config block marker is set
    //         memcpy(configurationBlock, c_MARKER_CONFIGURATION_NETWORK_V1, sizeof(c_MARKER_CONFIGURATION_NETWORK_V1));

    //         _ASSERTE(((HAL_Configuration_NetworkInterface*)configurationBlock)->StartupAddressMode > 0);
    //     }
    //     else if(configuration == DeviceConfigurationOption_Wireless80211Network)
    //     {
    //         // storage address from block address
    //         storageAddress = (ByteAddress)g_TargetConfiguration.Wireless80211Configs->Configs[configurationIndex];

    //         // set block size, in case it's not already set
    //         blockSize = sizeof(HAL_Configuration_Wireless80211);

    //         // make sure the config block marker is set
    //         memcpy(configurationBlock, c_MARKER_CONFIGURATION_WIRELESS80211_V1, sizeof(c_MARKER_CONFIGURATION_WIRELESS80211_V1));
    //     }
    //     else if(configuration == DeviceConfigurationOption_X509CaRootBundle)
    //     {
    //         // storage address from block address
    //         storageAddress = (ByteAddress)g_TargetConfiguration.CertificateStore->Certificates[configurationIndex];

    //         // set block size, in case it's not already set
    //         // because X509 certificate has a variable length need to compute the block size in two steps
    //         blockSize = offsetof(HAL_Configuration_X509CaRootBundle, Certificate);
    //         blockSize += ((HAL_Configuration_X509CaRootBundle*)configurationBlock)->CertificateSize;

    //         // make sure the config block marker is set
    //         memcpy(configurationBlock, c_MARKER_CONFIGURATION_X509CAROOTBUNDLE_V1, sizeof(c_MARKER_CONFIGURATION_X509CAROOTBUNDLE_V1));
    //     }
    //     else
    //     {
    //         // this not a valid configuration option to update, quit
    //         // free memory first
    //         platform_free(configSectorCopy);

    //         return FALSE;
    //     }
    
    //     // erase config sector
    //     if(STM32FlashDriver_EraseBlock(NULL, (uint32_t)&__nanoConfig_start__) == TRUE)
    //     {
    //         // flash block is erased

    //         // subtract the start address of config sector to get the offset
    //         blockOffset = storageAddress - (uint32_t)&__nanoConfig_start__;

    //         // set pointer to block to udpate
    //         blockAddressInCopy = configSectorCopy + blockOffset;
            
    //         // replace config block with new content by replacing memory
    //         memcpy(blockAddressInCopy, configSectorCopy, blockSize);

    //         // copy the config block copy back to the config block storage
    //         success = STM32FlashDriver_Write(NULL, (uint32_t)&__nanoConfig_start__, sizeOfConfigSector, (unsigned char*)configSectorCopy, true);
    //     }

    //     // free memory
    //     platform_free(configSectorCopy);
    // }

    return success;
}

//  Default initialisation for wireless config block
void InitialiseWirelessDefaultConfig(HAL_Configuration_Wireless80211 * pconfig, uint32_t configurationIndex)
{
    (void)pconfig;
    (void)configurationIndex;

    // TODO    
}

//  Default initialisation for Network interface config blocks
bool InitialiseNetworkDefaultConfig(HAL_Configuration_NetworkInterface * pconfig, uint32_t configurationIndex)
{
    uint8_t  macAddress[SL_MAC_ADDR_LEN];
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;

    memset(pconfig, 0, sizeof(HAL_Configuration_NetworkInterface));
    
    pconfig->InterfaceType = NetworkInterfaceType_Wireless80211;
    pconfig->StartupAddressMode = AddressMode_DHCP;
    pconfig->SpecificConfigId = 0;

    sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen, pconfig->MacAddress);

    return true;
}