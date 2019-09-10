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
�����ڿ���ϵͳ
�������裺 
I2C      SDA ->PA2         SCL ->PA3
8·PWM   ���1 PD0 PD1     ���2 PE4 PE5       ���3 PB4 PB5          ���4 PA6 PA7
��ʱ��    TIM4�����жϿ���     TIM5����usmart
uart0  

PS:�л�������ֻ�����һ�μ��ɣ�
		�����л�������ͬ������ĽǶȣ������л�
		����ʱ�� ֱ�Ӷ�ȡ����
******************************************************************************************************************************************************************************************/

volatile uint8_t mode = 0;
extern Mode_Typedef Mode_Struct;
extern PIDTypedef PID_Struct_x,PID_Struct_y;
extern uint16_t intpurscreenString[80] ;
extern uint32_t mode_time;  //����Ҫ���ʱ��  ��λ0.1s

//���������л� ȡֵ0~7  0��Ӧֹͣ 
void mode_change(uint8_t modee)
{
	PID_Resume();  			//��λpid
	pid_set();    	 		//����pidֵ
	Mode_Struct_Init();	//��λ�й�г���˶��Ĳ���
	count_time_init();  //��λ����ʱ��
	
	switch(modee)       //�ı�ĳЩ����ĳ�ʼֵ �����������
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

//����2�ı�ֱ�߳��ȵĺ��� 
//ȡֵ30~~60
void mode_2_length(uint8_t mode_2_radiuss)
{
	Mode_Struct.radius = mode_2_radiuss/2;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //��Ӧ�ķ���
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	printf("mode_2_R: %d\n",Mode_Struct.radius);
}

//����5�ı�ֱ�߳��ȵĺ��� 
//ȡֵ30~~60
void mode_5_length(uint8_t mode_5_radiuss)
{
	Mode_Struct.radius = mode_5_radiuss/2;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958;  //��Ӧ�ķ���
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958;
	printf("mode_5_R: %d\n",Mode_Struct.radius);
}

//����3�ı�Ƕȵĺ���
void mode_3_angel_change(uint16_t mode_3_angell)
{
	Mode_Struct.angel = mode_3_angell;
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //��Ӧ�ķ���
	Mode_Struct.Y      = -atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	printf("mode_3_du: %d\n",Mode_Struct.angel);
}

//���� angel�Ƕ� 0~360  radius�뾶 0~~30
void mode_7_change(uint16_t mode_7_angell, uint8_t mode_7_radiuss)
{
	Mode_Struct.angel = mode_7_angell;
	Mode_Struct.radius = mode_7_radiuss;
	Mode_Struct.X      = -atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //��Ӧ�ķ���
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	Mode_Struct.omega  = 2*3.14159/Mode_Struct.period;           //ŷ�׸�       //г�� Asin(wt)
	printf("mode_7_angel: %d    mode_7_radius: %d\n",Mode_Struct.angel,Mode_Struct.radius);
}

int main(void)
{
	float xita,leng,x,y,xo,yo,h;
	//����ϵͳʱ��80Mhz
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


