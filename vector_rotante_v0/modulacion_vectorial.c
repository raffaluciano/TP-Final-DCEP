#include "math.h"
#include "Lab.h"

float frecuencia = 50;

int interrupcion()
{
    static float fase = 0;
    float E = 1;
    float V_ref = 0.5; // modulo?

    float periodo = 1.0 / frecuencia;
    float Ts = 0.0001; // Siempre que Fs sea 10 kHz

    fase += Ts * 2 * M_PI / periodo;
    // fase = fase % (2 * M_PI);
    fase = fmod(fase, 2 * M_PI); // Creo que esto es lo que deberia ir

    float Vx = V_ref * cos(fase);
    float Vy = V_ref * sin(fase);

    //float fase_rel = fase % (M_PI / 3);
    float fase_rel = fmod(fase, M_PI / 3);
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

    EPwm4Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm1 / Ts); // adjust duty for output EPwm4A
	EPwm5Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm2 / Ts); // adjust duty for output EPWM5A
	EPwm6Regs.CMPA.half.CMPA = PROJECT_PERIOD * (1 - t_pwm3 / Ts); // adjust duty for output EPWM6A

    return 0;
}
