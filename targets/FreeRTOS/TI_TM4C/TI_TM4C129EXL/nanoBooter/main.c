//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <targetHAL.h>
#include <WireProtocol_ReceiverThread.h>
#include <nanoPAL_BlockStorage.h>

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// // need to declare the Receiver thread here
// osThreadDef(ReceiverThread, osPriorityHigh, 512, "ReceiverThread");

// // configuration for debugger serial port
// // dev notes:
// // conservative baud rate value as 921600 has a high error percentage on baud rate clocking
// // OVER8 bit on CR1 to further decrease baud rate clocking error 
// static const SerialConfig uartConfig =
// {
//   912600,
//   USART_CR1_OVER8,
//   USART_CR2_STOP1_BITS,
//   0
// };

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

// Application entry point.
int main(void)
{

    volatile uint32_t ui32Loop;
  
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    //
    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }

        //
        // Turn off the LED.
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }

  // // HAL initialization, this also initializes the configured device drivers
  // // and performs the board-specific initializations.
  // halInit();

  // // the following IF is not mandatory, it's just providing a way for a user to 'force'
  // // the board to remain in nanoBooter and not launching nanoCLR

  // // if the USER button (blue one) is pressed, skip the check for a valid CLR image and remain in booter
  // if (palReadPad(GPIOC, GPIOC_BUTTON))
  // {
  //   // check for valid CLR image at address contiguous to nanoBooter
  //   // this target DOES NOT have configuration block, so we need to use the __nanoImage_end__ address here
  //   if(CheckValidCLRImage((uint32_t)&__nanoImage_end__))
  //   {
  //     // there seems to be a valid CLR image
  //     // launch nanoCLR
  //     LaunchCLR((uint32_t)&__nanoImage_end__);
  //   }
  // }

  // // The kernel is initialized but not started yet, this means that
  // // main() is executing with absolute priority but interrupts are already enabled.
  // osKernelInitialize();

  // #if (HAL_NF_USE_STM32_CRC == TRUE)
  // // startup crc
  // crcStart(NULL);
  // #endif

  // // starts the serial driver
  // sdStart(&SERIAL_DRIVER, &uartConfig);

  // // create the receiver thread
  // osThreadCreate(osThread(ReceiverThread), NULL);

  // // start kernel, after this main() will behave like a thread with priority osPriorityNormal
  // osKernelStart();

  // // initialize block storage list and devices
  // // in CLR this is called in nanoHAL_Initialize()
  // // for nanoBooter we have to init it in order to provide the flash map for Monitor_FlashSectorMap command
  // BlockStorageList_Initialize();
  // BlockStorage_AddDevices();

  // //  Normal main() thread
  // while (true) {
  //   palClearPad(GPIOA, GPIOA_LED_GREEN);
  //   osDelay(500);
  //   palSetPad(GPIOA, GPIOA_LED_GREEN);
  //   osDelay(500);
  // }
}
