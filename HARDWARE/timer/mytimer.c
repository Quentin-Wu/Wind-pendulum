#include "mytimer.h"
#include "inv_mpu.h"
#include "myuart.h"
#include "pid.h"
#include "led.h"
#include "control.h"
#include "mypwm.h"

float pitch,roll,yaw; 			//欧拉角
//float warebuf[8];
extern PIDTypedef PID_Struct_x,PID_Struct_y;

extern uint8_t mode;

void Timer4IntHandler(void);

void My_Timer4_Init(uint16_t xms)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
	//周期性计数模式
  TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(TIMER4_BASE, TIMER_A, 80000*xms);
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER4A);
	//允许处理某个定时器的中断请求
	TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER4A, 0<<5);
	//注册中断处理函数
	TimerIntRegister(TIMER4_BASE, TIMER_BOTH, Timer4IntHandler); 
	//开启定时器
  TimerEnable(TIMER4_BASE, TIMER_A);
}

void Timer4IntHandler(void)
{
	// 清除定时器中断标志位
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	
//	IntMasterDisable();
//	while(mpu_dmp_get_data(&pitch,&roll,&yaw) !=0);
//	IntMasterEnable();

//	PID_Struct_x.get = -pitch;
//	PID_Struct_y.get = roll;
	
	LED_BLUE(ON);	
	while(mpu_dmp_get_data(&pitch,&roll,&yaw) !=0);  //返回值为0 时获取mpu6050数据成功
//	mpu_dmp_get_data(&pitch,&roll,&yaw);
	LED_BLUE(OFF);	
	PID_Struct_x.get = -pitch;
	PID_Struct_y.get = roll;
	
		switch(mode)
		{
			case 1: mode_1();  	break;
			case 2: mode_2();  	break;
			case 3: mode_3();  	break;
			case 4: mode_4();  	break;
			case 5: mode_5();  	break;
			case 6: mode_6();   break;
			case 7: mode_7();   break;
			default: All_stop();break;
		}
		
}


