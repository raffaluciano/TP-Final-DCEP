

/**
 * main.c
 */
// Máxima tensión de salida 0.57 Vbus

#include "Lab.h"

int main(void)
{
    //--- CPU Initialization
    InitSysCtrl();  // Initialize the CPU (FILE: SysCtrl.c)
    InitGpio();     // Initialize the shared GPIO pins (FILE: Gpio.c)
    InitPieCtrl();  // Initialize and enable the PIE (FILE: PieCtrl.c)
    InitWatchdog(); // Initialize the Watchdog Timer (FILE: WatchDog.c)

    InitEPwm();
    InitAdc();

    while (1)
    {
        // interrupcion();
    }
}
