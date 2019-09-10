#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "math.h"

#include "led.h"
#include "key.h"
#include "myuart.h"
#include "delay.h"
#include "usmart.h"

#include "mpu6050.h"

#include "mypwm.h"
#include "control.h"
#include "pid.h"
#include "screen.h"

/****************************************************************************************************************************************************************************************
风力摆控制系统
所用外设： 
I2C      SDA ->PA2         SCL ->PA3
8路PWM   马达1 PD0 PD1     马达2 PE4 PE5       马达3 PB4 PB5          马达4 PA6 PA7
定时器    TIM4用于中断控制     TIM5用于usmart
uart0  

PS:切换任务函数只需调用一次即可！
		还下列还包含不同的任务的角度，长度切换
		计数时间 直接读取即可
******************************************************************************************************************************************************************************************/

volatile uint8_t mode = 0;
extern Mode_Typedef Mode_Struct;
extern PIDTypedef PID_Struct_x,PID_Struct_y;
extern uint16_t intpurscreenString[80] ;
extern uint32_t mode_time;  //符合要求的时间  单位0.1s

//进行任务切换 取值0~7  0对应停止 
void mode_change(uint8_t modee)
{
	PID_Resume();  			//复位pid
	pid_set();    	 		//设置pid值
	Mode_Struct_Init();	//复位有关谐振运动的参数
	count_time_init();  //复位计数时间
	
	switch(modee)       //改变某些任务的初始值 方便进行设置
		{
			case 2: Mode_Struct.radius = 35;  	break;
			case 3: Mode_Struct.angel  = 111;  	break;
			case 5: Mode_Struct.radius = 35;  	break;
		  case 7:{Mode_Struct.radius = 35;Mode_Struct.angel  = 111;} break;
			default: break;
		}
	All_stop();
	mode = modee;      
	printf("mode: %d\n",mode);
}

//任务2改变直线长度的函数 
//取值30~~60
void mode_2_length(uint8_t mode_2_radiuss)
{
	Mode_Struct.radius = mode_2_radiuss/2;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //对应的幅度
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	printf("mode_2_R: %d\n",Mode_Struct.radius);
}

//任务5改变直线长度的函数 
//取值30~~60
void mode_5_length(uint8_t mode_5_radiuss)
{
	Mode_Struct.radius = mode_5_radiuss/2;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958;  //对应的幅度
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958;
	printf("mode_5_R: %d\n",Mode_Struct.radius);
}

//任务3改变角度的函数
void mode_3_angel_change(uint16_t mode_3_angell)
{
	Mode_Struct.angel = mode_3_angell;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //对应的幅度
	Mode_Struct.Y      = -atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	printf("mode_3_du: %d\n",Mode_Struct.angel);
}

//定点 angel角度 0~360  radius半径 0~~30
void mode_7_change(uint16_t mode_7_angell, uint8_t mode_7_radiuss)
{
	Mode_Struct.angel = mode_7_angell;
	Mode_Struct.radius = mode_7_radiuss;
	Mode_Struct.X      = -atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //对应的幅度
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	Mode_Struct.omega  = 2*3.14159/Mode_Struct.period;           //欧米噶       //谐振 Asin(wt)
	printf("mode_7_angel: %d    mode_7_radius: %d\n",Mode_Struct.angel,Mode_Struct.radius);
}

int main(void)
{
	float xita,leng,x,y,xo,yo,h;
	//配置系统时钟80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	usmart_dev.init(80);	//80MHz

	PWM_Init(); //1ms  0us
	PID_Init();
	MPU6050_Init();
	
	Screen_Init();

//	printf("1");
//	mode_change(1);
//	mode_3_angel_change(30);
//	mode_25_length(30);
//	mode_7_change(10, 10);
	while(1)
	{
//		xo=PID_Struct_x.get/57;
//		yo=PID_Struct_y.get/57;
//		h=Mode_Struct.hight;
//		x=tan(xo)*h;
//		y=tan(yo)*h;
//		printf("%f   %f\n",x,y);
//		xita = atan(y/x);
//		leng = y*sin(xita);
		
//		draw_line(xita*57.2958,leng);
			if(mode !=7 && mode != 9 && mode !=0 && intpurscreenString[0]!='L')
			{
				sand_txt_time(mode_time);
			}
	}

}


