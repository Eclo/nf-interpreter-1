//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoPAL.h>
#include <nanoHAL_Time.h>
#include <nanoHAL_Types.h>
#include <nanoPAL_BlockStorage.h>
#include <nanoHAL_ConfigurationManager.h>

// because nanoHAL_Initialize/Uninitialize needs to be called in both C and C++ we need a proxy to allow it to be called in 'C'
extern "C" {
    
    void nanoHAL_Initialize_C()
    {
        nanoHAL_Initialize();
    }

    void nanoHAL_Uninitialize_C()
    {
        nanoHAL_Uninitialize();
    }
}

void nanoHAL_Initialize()
{
    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION  ::InitializeList();

    BlockStorageList_Initialize();

    // initialize block storage devices
    BlockStorage_AddDevices();

    BlockStorageList_InitializeDevices();

    // clear managed heap region
    unsigned char* heapStart = NULL;
    unsigned int heapSize  = 0;

    ::HeapLocation( heapStart, heapSize );
    memset(heapStart, 0, heapSize);

    ConfigurationManager_Initialize();

    Events_Initialize();

    // TODO
    //CPU_GPIO_Initialize();

    // no PAL events required until now
    //PalEvent_Initialize();
	
	// Initialise Network Stack
    // Network_Initialize();
}

void nanoHAL_Uninitialize()
{
    // release the global mutex, just in case it's locked somewhere
    //chMtxUnlock(&interpreterGlobalMutex);

    // TODO check for s_rebootHandlers
    // for(int i = 0; i< ARRAYSIZE(s_rebootHandlers); i++)
    // {
    //     if(s_rebootHandlers[i] != NULL)
    //     {
    //         s_rebootHandlers[i]();
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }   

    // SOCKETS_CloseConnections();

  #if !defined(HAL_REDUCESIZE)
    // TODO need to call this but it's preventing the debug session from starting
    //Network_Uninitialize();
  #endif

    BlockStorageList_UnInitializeDevices();

    // TODO
    // CPU_GPIO_Uninitialize();

    Events_Uninitialize();
    
    HAL_CONTINUATION::Uninitialize();
    HAL_COMPLETION  ::Uninitialize();
}

volatile int32_t SystemStates[SYSTEM_STATE_TOTAL_STATES];

void SystemState_SetNoLock(SYSTEM_STATE_type state)
{
    SystemStates[state]++;
}

void SystemState_ClearNoLock(SYSTEM_STATE_type state)
{
    SystemStates[state]--;
}

bool SystemState_QueryNoLock(SYSTEM_STATE_type state)
{
    return (SystemStates[state] > 0) ? true : false;
}

void SystemState_Set(SYSTEM_STATE_type state)
{
    GLOBAL_LOCK();

    SystemState_SetNoLock(state);

    GLOBAL_UNLOCK();
}

void SystemState_Clear(SYSTEM_STATE_type state)
{
    GLOBAL_LOCK();

    SystemState_ClearNoLock(state );

    GLOBAL_UNLOCK();
}

bool SystemState_Query(SYSTEM_STATE_type state)
{
    GLOBAL_LOCK();

    bool systemStateCopy = SystemState_QueryNoLock(state);
    
    GLOBAL_UNLOCK();

    return systemStateCopy;
}
