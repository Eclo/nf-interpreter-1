//
// Copyright (c) 2019 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>

extern "C" {
    void HeapLocation_C(unsigned char** baseAddress, unsigned int* sizeInBytes)
    {
        // *baseAddress = (unsigned char*)(unsigned char*) &HeapBegin;
        // *sizeInBytes = (unsigned int)((size_t)&HeapEnd - (size_t)&HeapBegin);
        *baseAddress = (unsigned char*)0x22000000;
        *sizeInBytes = (unsigned int)0x10000;
    }
}

void HeapLocation(unsigned char*& baseAddress, unsigned int& sizeInBytes)
{
    NATIVE_PROFILE_PAL_HEAP();

    // baseAddress = (unsigned char*)                            &HeapBegin;
    // sizeInBytes = (unsigned int)((size_t)&HeapEnd - (size_t)&HeapBegin);

    // TODO
    // this managed heap needs to be properly defined
    baseAddress = (unsigned char*)0x22000000;   //start of SYSRAM
    sizeInBytes = 0x10000;                      //64kBB
}
