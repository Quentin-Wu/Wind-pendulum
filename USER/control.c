#include "control.h"
#include "led.h"
#include "pid.h"
#include "mypwm.h"
#include "math.h"
#include "myuart.h"

float warebuf[8];

//					x      y 
extern float pitch,roll,yaw; 			//欧拉角  
extern PIDTypedef PID_Struct_x,PID_Struct_y;

uint32_t mode_time = 0;
uint32_t mode_time1 = 0;
uint8_t  mode_OK = 0;

Mode_Typedef Mode_Struct;

int32_t out_x_max =600;
int32_t out_y_max =600;
	
void Mode_Struct_Init(void)
{
	Mode_Struct.radius = 25;			//半径 cm
	Mode_Struct.hight  = 79;			//高度 cm
	Mode_Struct.period = 1560;    //摆动周期 ms
	Mode_Struct.angel  = 90;      //初始化90°
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //对应的幅度
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	Mode_Struct.omega  = 2*3.14159/Mode_Struct.period;           //欧米噶       //谐振 Asin(wt)
	Mode_Struct.time   = 0;       //复位时间
}

void pid_set(void)
{
	PID_Struct_x.Kp = 50;
	PID_Struct_x.Kd = 30000;
	PID_Struct_x.Ki = 0;
	PID_Struct_y.Kp = 50;
	PID_Struct_y.Kd = 30000;
	PID_Struct_y.Ki = 0;
}

void count_time_init(void)
{
	mode_time = 0;
	mode_time1 = 0;
	mode_OK = 0;
}

//谐振运动 
void Resonant_Motion(void)
{
	LED_BLUE_TUGGEL
	
	PID_Struct_x.set = Mode_Struct.X *sin(Mode_Struct.omega*Mode_Struct.time);
	PID_Struct_y.set = Mode_Struct.Y *sin(Mode_Struct.omega*Mode_Struct.time);
	PID_x_realize();
	PID_y_realize();
		
	Motor_Out((int32_t)PID_Struct_x.out, (int32_t)PID_Struct_y.out);
	
	Mode_Struct.time += 10; //ms
	if(Mode_Struct.time>=Mode_Struct.period) Mode_Struct.time=0;
	
	if((fabs(PID_Struct_x.set-PID_Struct_x.get) <1) && (fabs(PID_Struct_y.set-PID_Struct_y.get) <1) && mode_OK<=250)
	{
		mode_OK++;
	}
	if(++mode_time1 >=10) 
	{ 
		if(mode_OK <=200) mode_time++;
    else mode_OK = 250;		
		mode_time1=0;
	}
}
	


//从静止开始， 15s 内控制风力摆做类似自由摆运动， 使激光笔稳定地
//在地面画出一条长度不短于 50cm 的直线段， 其线性度偏差不大于±2.5cm， 并且具有较好的重复性；
void mode_1(void)
{
	Resonant_Motion();
}

//从静止开始， 15s 内完成幅度可控的摆动，画出长度在 30~60cm 间可设
//置， 长度偏差不大于±2.5cm 的直线段，并且具有较好的重复性；
void mode_2(void)
{	
	if(Mode_Struct.radius != 35)  //任务二时初始化半径为35  如果半径不为35（直径70） 即改变了半径 则开始运行  
	{
		Resonant_Motion();
	}
}

//可设定摆动方向，风力摆从静止开始， 15s 内按照设置的方向（角度）
//摆动， 画出不短于 20cm 的直线段；
void mode_3(void)
{
	if(Mode_Struct.angel != 111)  //任务三时初始化角度为111  如果角度不为111 即改变了角度 则开始运行  
	{
		Resonant_Motion();
	}
}

//将风力摆拉起一定角度（30° ~45° ） 放开， 5s 内使风力摆制动达到静
//止状态。
void mode_4(void)
{
	if( fabs(PID_Struct_x.get) < 50 &&  fabs(PID_Struct_y.get) < 50)
	{
		PID_Struct_x.Kp = 100;
		PID_Struct_x.Kd = 20000;
		PID_Struct_x.Ki = 0.1;
		PID_Struct_y.Kp = 100;
		PID_Struct_y.Kd = 20000;
		PID_Struct_y.Ki = 0.1;
		
		PID_Struct_x.set = 0;
		PID_Struct_y.set = 0;
		PID_x_realize();
		PID_y_realize();
		
		if(fabs(PID_Struct_x.set-PID_Struct_x.get) <1 && fabs(PID_Struct_y.set-PID_Struct_y.get) <1)
		{
			mode_OK++;
		}
		if(++mode_time1 >=10) 
		{ 
			if(mode_OK <=200) mode_time++; 
			mode_time1=0;
		}
	}
	else
	{
		PID_Resume();
	}
		Motor_Out((int32_t)PID_Struct_x.out, (int32_t)PID_Struct_y.out);
}

//以风力摆静止时激光笔的光点为圆心， 驱动风力摆用激光笔在地面画圆， 30s 内需重复 3 次；
//圆半径可在 15~35cm 范围内设置， 激光笔画出的轨迹应落在指定半径±2.5cm 的圆环内；
void mode_5(void)
{
	if(Mode_Struct.radius != 35)  //任务五时初始化半径为35  如果半径不为35（直径70） 即改变了半径 则开始运行  
	{
		PID_Struct_x.set = Mode_Struct.X *cos(Mode_Struct.omega*Mode_Struct.time);
		PID_Struct_y.set = Mode_Struct.Y *sin(Mode_Struct.omega*Mode_Struct.time);
		PID_x_realize();
		PID_y_realize();
			
		Motor_Out((int32_t)PID_Struct_x.out, (int32_t)PID_Struct_y.out);
		
		Mode_Struct.time += 10; //ms
		if(Mode_Struct.time>=Mode_Struct.period) Mode_Struct.time=0;
		
		if(fabs(PID_Struct_x.set-PID_Struct_x.get) <1 && fabs(PID_Struct_y.set-PID_Struct_y.get) <1)
		{
			mode_OK++;
		}
		if(++mode_time1 >=10) 
		{ 
			if(mode_OK <=200) mode_time++; 
			mode_time1=0;
		}
	}
}

//在发挥部分（1）后继续作圆周运动，在距离风力摆 1~2m 距离内用一台 50~60W 台扇在水平方向吹向风力摆，台扇吹 5s 后停止， 
//风力摆能够在 5s 内恢复发挥部分（1） 规定的圆周运动，激光笔画出符合要求的轨迹；
void mode_6(void)
{
	mode_5();
}

//其他
//定点设置
void mode_7(void)
{
	if((Mode_Struct.radius != 35) && (Mode_Struct.angel != 111)) 
	{
		PID_Struct_x.Kp = 200;
		PID_Struct_x.Kd = 10000;
		PID_Struct_x.Ki = 1;
		PID_Struct_y.Kp = 200;
		PID_Struct_y.Kd = 10000;
		PID_Struct_y.Ki = 1;
		PID_Struct_x.set = Mode_Struct.X ;
		PID_Struct_y.set = Mode_Struct.Y ;
		PID_x_realize();
		PID_y_realize();
		
		if(fabs(PID_Struct_x.set-PID_Struct_x.get) <1 && fabs(PID_Struct_y.set-PID_Struct_y.get) <1)
		{
			mode_OK++;
		}
		if(++mode_time1 >=10) 
		{ 
			if(mode_OK <=200) mode_time++; 
			mode_time1=0;
		}
	}
	else
	{
		PID_Resume();
	}
	Motor_Out((int32_t)PID_Struct_x.out, (int32_t)PID_Struct_y.out);
}

void Motor_Out(int32_t out_x, int32_t out_y)
{
	if(out_x >  out_x_max)  out_x =  out_x_max;
	if(out_x < -out_x_max)  out_x = -out_x_max;
	if(out_y >  out_y_max)  out_y =  out_y_max;
	if(out_y < -out_y_max)  out_y = -out_y_max;
	
	if(out_x >= 0) {  M1_Backward(out_x);		    M2_Forward(out_x);	}
	if(out_x < 0)  {  M1_Forward(fabs(out_x));  M2_Backward(fabs(out_x));	}
	if(out_y >= 0) {  M3_Backward(0);		   	M4_Backward(out_y);	}
	if(out_y < 0)  {  M3_Forward(fabs(out_y));  M4_Backward(0);	}

//		warebuf[0] = PID_Struct_y.set;
//		warebuf[1] = PID_Struct_y.get;
//		warebuf[2] = out_y;
//		warebuf[3] = PID_Struct_x.set;
//		warebuf[4] = PID_Struct_x.get;
//		warebuf[5] = out_x;
//		vcan_sendware((uint8_t *)warebuf,sizeof(warebuf));
	
//	warebuf[0] = out_x;
//	warebuf[1] = out_y;
//	vcan_sendware((uint8_t *)warebuf,sizeof(warebuf)); 
}



