//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <ch.h>
#include <hal.h>
#include <cmsis_os.h>

#include <swo.h>
#include <CLR_Startup_Thread.h>
#include <WireProtocol_ReceiverThread.h>
#include <nanoCLR_Application.h>
#include <nanoPAL_BlockStorage.h>
#include <targetPAL.h>


// need to declare the Receiver thread here
osThreadDef(ReceiverThread, osPriorityHigh, 2048, "");
// declare CLRStartup thread here 
osThreadDef(CLRStartupThread, osPriorityNormal, 4096, ""); 

//  Application entry point.
int main(void) {

  // HAL initialization, this also initializes the configured device drivers
  // and performs the board-specific initializations.
  halInit();

  // init SWO as soon as possible to make it available to output ASAP
  #if (SWO_OUTPUT == TRUE)  
  SwoInit();
  #endif

  // The kernel is initialized but not started yet, this means that
  // main() is executing with absolute priority but interrupts are already enabled.
  osKernelInitialize();

  // Prepares the serial driver 2 using UART2
  sdStart(&SD2, NULL);

  // create the receiver thread
  osThreadCreate(osThread(ReceiverThread), NULL);
  // create the CLR Startup thread 
  osThreadCreate(osThread(CLRStartupThread), NULL); 

  // start kernel, after this main() will behave like a thread with priority osPriorityNormal
  osKernelStart();

  while (true) { 
    osDelay(100);
  }
}
