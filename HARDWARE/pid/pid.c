#include "pid.h"
#include "myuart.h"
#include "math.h"

PIDTypedef PID_Struct_x,PID_Struct_y;

void PID_Init(void)
{
	PID_Struct_x.get = 0;
	PID_Struct_x.set = 0;
	PID_Struct_x.out = 0;
	
	PID_Struct_x.Kp = 0;
	PID_Struct_x.Kd = 0;
	PID_Struct_x.Ki = 0;
	
	PID_Struct_x.Ek	= 0;
	PID_Struct_x.Ek1 = 0;
	PID_Struct_x.Ek_sum = 0;
	
	PID_Struct_y.get = 0;
	PID_Struct_y.set = 0;
	PID_Struct_y.out = 0;
	
	PID_Struct_y.Kp = 0;
	PID_Struct_y.Kd = 0;
	PID_Struct_y.Ki = 0;
	
	PID_Struct_y.Ek	= 0;
	PID_Struct_y.Ek1 = 0;
	PID_Struct_y.Ek_sum = 0;
}

void PID_Resume(void)
{
	PID_Struct_x.set = 0;
	PID_Struct_x.out = 0;
	
	PID_Struct_x.Kp = 0;
	PID_Struct_x.Kd = 0;
	PID_Struct_x.Ki = 0;
	
	PID_Struct_x.Ek	= 0;
	PID_Struct_x.Ek1 = 0;
	PID_Struct_x.Ek_sum = 0;
	
	PID_Struct_y.set = 0;
	PID_Struct_y.out = 0;
	
	PID_Struct_y.Kp = 0;
	PID_Struct_y.Kd = 0;
	PID_Struct_y.Ki = 0;
	
	PID_Struct_y.Ek	= 0;
	PID_Struct_y.Ek1 = 0;
	PID_Struct_y.Ek_sum = 0;
}

void Set_x_Kp(float kpp)
{
	PID_Struct_x.Kp = kpp;
	printf("Kp = %f\n", PID_Struct_x.Kp);
}

void Set_x_Kd(float kdd)
{
	PID_Struct_x.Kd = kdd;
	printf("Kd = %f\n", PID_Struct_x.Kd);
}

void Set_x_Ki(float kii)
{
	PID_Struct_x.Ki = kii;
	printf("Ki = %f\n", PID_Struct_x.Ki);
}

void Set_y_Kp(float kpp)
{
	PID_Struct_y.Kp = kpp;
	printf("Kp = %f\n", PID_Struct_y.Kp);
}

void Set_y_Kd(float kdd)
{
	PID_Struct_y.Kd = kdd;
	printf("Kd = %f\n", PID_Struct_y.Kd);
}

void Set_y_Ki(float kii)
{
	PID_Struct_y.Ki = kii;
	printf("Ki = %f\n", PID_Struct_y.Ki);
}

void PID_x_realize(void)
{	  
	PID_Struct_x.Ek = PID_Struct_x.set -PID_Struct_x.get;	
	
	PID_Struct_x.Ek_sum += PID_Struct_x.Ek;

	PID_Struct_x.out = PID_Struct_x.Kp*PID_Struct_x.Ek 
										+PID_Struct_x.Ki*PID_Struct_x.Ek_sum
										+PID_Struct_x.Kd*(PID_Struct_x.Ek -PID_Struct_x.Ek1);
	
	PID_Struct_x.Ek1 = PID_Struct_x.Ek;	
}

void PID_y_realize(void)
{	  
	PID_Struct_y.Ek = PID_Struct_y.set -PID_Struct_y.get;	
	
	PID_Struct_y.Ek_sum += PID_Struct_y.Ek;

	PID_Struct_y.out = PID_Struct_y.Kp*PID_Struct_y.Ek 
										+PID_Struct_y.Ki*PID_Struct_y.Ek_sum
										+PID_Struct_y.Kd*(PID_Struct_y.Ek -PID_Struct_y.Ek1);
	
	PID_Struct_y.Ek1 = PID_Struct_y.Ek;	
}
