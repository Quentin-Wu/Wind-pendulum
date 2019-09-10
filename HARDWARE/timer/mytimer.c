#include "mytimer.h"
#include "inv_mpu.h"
#include "myuart.h"
#include "pid.h"
#include "led.h"
#include "control.h"
#include "mypwm.h"

float pitch,roll,yaw; 			//ŷ����
//float warebuf[8];
extern PIDTypedef PID_Struct_x,PID_Struct_y;

extern uint8_t mode;

void Timer4IntHandler(void);

void My_Timer4_Init(uint16_t xms)
{		
	//ʹ������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
	//�����Լ���ģʽ
  TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
	
	//���ü�����Χ
	TimerLoadSet(TIMER4_BASE, TIMER_A, 80000*xms);
	
	//��ϵͳ��ʹ�ܶ�ʱ���ж�
	IntEnable(INT_TIMER4A);
	//������ĳ����ʱ�����ж�����
	TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT); 
	//�����ж����ȼ�
	IntPrioritySet(INT_TIMER4A, 0<<5);
	//ע���жϴ�����
	TimerIntRegister(TIMER4_BASE, TIMER_BOTH, Timer4IntHandler); 
	//������ʱ��
  TimerEnable(TIMER4_BASE, TIMER_A);
}

void Timer4IntHandler(void)
{
	// �����ʱ���жϱ�־λ
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	
//	IntMasterDisable();
//	while(mpu_dmp_get_data(&pitch,&roll,&yaw) !=0);
//	IntMasterEnable();

//	PID_Struct_x.get = -pitch;
//	PID_Struct_y.get = roll;
	
	LED_BLUE(ON);	
	while(mpu_dmp_get_data(&pitch,&roll,&yaw) !=0);  //����ֵΪ0 ʱ��ȡmpu6050���ݳɹ�
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


