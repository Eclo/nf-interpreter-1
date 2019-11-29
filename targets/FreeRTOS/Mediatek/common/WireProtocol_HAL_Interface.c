//
// Copyright (c) 2019 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "mt3620-uart.h"

#include <WireProtocol.h>
#include <WireProtocol_Message.h>

WP_Message inboundMessage;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// currently using ISU0 UART
////////////////////////////////////////////////////////////////////////////////////////////////

int WP_ReceiveBytes(uint8_t* ptr, uint16_t* size)
{
    // save for latter comparison
    uint16_t requestedSize = *size;

    // sanity check for request of 0 size
    if(*size)
    {
        //////////////////////////////////////////////////////////
        //               PORTING CHANGE REQUIRED HERE           //
        //////////////////////////////////////////////////////////
        // change here to read (size) bytes from the input stream
        // preferably with read timeout and being able to check 
        // if the requested number of bytes was actually read
        //////////////////////////////////////////////////////////

        // read from UART 
        size_t read = Uart_DequeueData(UartIsu0, ptr, *size); 

        ptr  += read;
        *size -= read;

        TRACE( TRACE_STATE, "RXMSG: Expecting %d bytes, received %d.\n",requestedSize, read);
        
        // check if the requested read matches the actual read count
        return (requestedSize == read);
    }

    return true;
}

int WP_TransmitMessage(WP_Message* message)
{
    uint32_t writeResult;
    bool operationResult = false;

    ///////////////////////////////////////////////////////////
    //              PORTING CHANGE REQUIRED HERE             //
    ///////////////////////////////////////////////////////////
    // change here to write (size) bytes to the output stream
    // preferably with timeout and being able to check 
    // if the write was sucessfull or at least buffered
    //////////////////////////////////////////////////////////

    TRACE( TRACE_HEADERS, "TXMSG: 0x%08X, 0x%08X, 0x%08X\n", message->m_header.m_cmd, message->m_header.m_flags, message->m_header.m_size );

    // write header to output stream
    writeResult = Uart_EnqueueData(UartIsu0, (const uint8_t *)&message->m_header, sizeof(message->m_header));

    if(writeResult == sizeof(message->m_header))
    {
        operationResult = true;

        // if there is anything on the payload send it to the output stream
        if(message->m_header.m_size && message->m_payload)
        {
            ///////////////////////////////////////////////////////////
            //              PORTING CHANGE REQUIRED HERE             //
            ///////////////////////////////////////////////////////////
            // see description above
            //////////////////////////////////////////////////////////

            // reset flag
            operationResult = false;

            writeResult = Uart_EnqueueData(UartIsu0, message->m_payload, message->m_header.m_size);

            if(writeResult == message->m_header.m_size)
            {
                operationResult = true;

                TRACE0( TRACE_ERRORS, "TXMSG: OK\n");                    
            }
        }
    }

    return operationResult;
}
