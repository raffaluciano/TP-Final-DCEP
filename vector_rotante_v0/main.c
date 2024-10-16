

/**
 * main.c
 */
// Máxima tensión de salida 0.57 Vbus
float modulo = 1; // 0 - 1.5 V 
unsigned int frecuencia = 50;

#include "Lab.h"
#include "math.h"

int interrupcion()
{
    static float fase = 0;

    float periodo = 1 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz
    fase += Ts * 2 * M_PI / periodo;
    float Vx = modulo * cos(fase);
    float Vy = modulo * sin(fase);

    return 0;
}

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
        //interrupcion();
    }
}

