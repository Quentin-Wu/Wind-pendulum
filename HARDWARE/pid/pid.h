#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	float get;
	float set;
	float out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Ek;
	float Ek1;
	float Ek_sum;
}PIDTypedef;

void PID_Init(void);
void PID_Resume(void);
void Set_x_Kp(float kpp);
void Set_x_Kd(float kdd);
void Set_x_Ki(float kii);
void Set_y_Kp(float kpp);
void Set_y_Kd(float kdd);
void Set_y_Ki(float kii);

void PID_x_realize(void);
void PID_y_realize(void);


#endif


