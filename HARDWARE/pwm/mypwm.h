#ifndef __MYPWM_H
#define __MYPWM_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

#define FREQ_PWM		1250  //1ms
#define WIDTH_PWM   1.25  //10us

void PWM_Init(void);

void M1_Forward(int32_t width);
void M1_Backward(int32_t width);
void M2_Forward(int32_t width);
void M2_Backward(int32_t width);
void M3_Forward(int32_t width);
void M3_Backward(int32_t width);
void M4_Forward(int32_t width);
void M4_Backward(int32_t width);

void All_stop(void);


#endif

