

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
    float E = 1;
    float V_ref = 1;  //modulo?

    float periodo = 1 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz

    fase += Ts * 2 * M_PI / periodo;
    fase = fase % (2 * M_PI);

    float Vx = modulo * cos(fase);
    float Vy = modulo * sin(fase);

    float fase_rel = fase % (M_PI / 3);
    int sext = (int) ceil(fase / (M_PI / 3));

    float ta, tb, t0;
    ta = sqrt(3) * V_ref / E * Ts * sin(M_PI / 3 - fase_rel);
    tb = sqrt(3) * V_ref / E * Ts * sin(fase_rel);
    t0 = Ts - ta - tb;
    
    float t_pwm1, t_pwm2, t_pwm3;
    switch (sext)
    {
    case 1:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;
    
    case 2:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm3 = t0 / 2;
        break;
    
    case 3:
        t_pwm1 = t0 / 2;
        t_pwm2 = ta + tb + t0 / 2;
        t_pwm1 = tb + t0 / 2;
        break;

    case 4:
        t_pwm1 = t0 / 2;
        t_pwm2 = tb + t0 / 2;
        t_pwm1 = ta + tb + t0 / 2;
        break;

    case 5:
        t_pwm1 = tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm1 = ta + tb + t0 / 2;
        break;

    case 6:
        t_pwm1 = ta + tb + t0 / 2;
        t_pwm2 = t0 / 2;
        t_pwm1 = tb + t0 / 2;
        break;
    
    default:
        break;
    }

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

