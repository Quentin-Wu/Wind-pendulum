#include "mypwm.h"
#include "myuart.h"

/*******************************
 1,M1
		IN1		->PD0
		IN2		->PD1
 2,M2
	  IN1		->PE4
		IN2		->PE5
 3,M3 
		IN1		->PB4
		IN2		->PB5
 4,M4
		IN1		->PA6
		IN2		->PA7
*/


//PD0
void  M1PWM0_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	
	//���� PWM1 Generator0��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator0 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, FREQ_PWM*freq - 1);  //100us
	//���� PWM1 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, WIDTH_PWM*width - 1); //us
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

//PD1
void  M1PWM1_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	//���� PWM1 Generator0��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator0 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, FREQ_PWM*freq - 1);
	//���� PWM1 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

//PA6
void  M1PWM2_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA6_M1PWM2);
	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);
	
	//���� PWM1 Generator1��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator1 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, FREQ_PWM*freq - 1);
	//���� PWM1 Generator1 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_1);
}

//PA7
void  M1PWM3_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA7_M1PWM3);
	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);
	
	//���� PWM1 Generator1��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator1 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, FREQ_PWM*freq - 1);
	//���� PWM1 Generator1 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_1);
}


//PE4
void  M0PWM4_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM0����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinConfigure(GPIO_PE4_M0PWM4);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
	
	//���� PWM0 Generator2��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator2 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, FREQ_PWM*freq - 1);  //100us
	//���� PWM0 Generator2 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, WIDTH_PWM*width - 1); //us
	
	//ʹ�� PWM0 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
  //ʹ�� PWM0 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

//PE5
void  M0PWM5_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM0����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinConfigure(GPIO_PE5_M0PWM5);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
	
	//���� PWM0 Generator0��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator0 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, FREQ_PWM*freq - 1);
	//���� PWM0 Generator0 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM0 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
  //ʹ�� PWM0 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

//PB4
void  M0PWM2_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinConfigure(GPIO_PB4_M0PWM2);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
	
	//���� PWM0 Generator1��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator1 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, FREQ_PWM*freq - 1);
	//���� PWM0 Generator1 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM0 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
  //ʹ�� PWM0 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}

//PB5
void  M0PWM3_Init(uint32_t freq, uint32_t width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM0����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinConfigure(GPIO_PB5_M0PWM3);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	
	//���� PWM0 Generator1��������
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM0 Generator1 ����
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, FREQ_PWM*freq - 1);
	//���� PWM0 Generator1 ռ�ձ�
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, WIDTH_PWM*width - 1);
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}


void M1_Forward(int32_t width)
{
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, WIDTH_PWM*width - 1);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 0);
}

void M1_Backward(int32_t width)
{
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 0);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, WIDTH_PWM*width - 1);
}

void M2_Forward(int32_t width)
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, WIDTH_PWM*width - 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 0);
}

void M2_Backward(int32_t width)
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, WIDTH_PWM*width - 1);
}

void M3_Forward(int32_t width)
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, WIDTH_PWM*width - 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
}

void M3_Backward(int32_t width)
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, WIDTH_PWM*width - 1);
}

void M4_Forward(int32_t width)
{
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, WIDTH_PWM*width - 1);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 0);
}

void M4_Backward(int32_t width)
{
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 0);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, WIDTH_PWM*width - 1);
}

void PWM_Init(void)
{
  M1PWM0_Init(1, 0);
  M1PWM1_Init(1, 0);
  M1PWM2_Init(1, 0);
  M1PWM3_Init(1, 0);
  M0PWM4_Init(1, 0);
  M0PWM5_Init(1, 0);
  M0PWM2_Init(1, 0);
  M0PWM3_Init(1, 0);
}

void All_stop(void)
{
	//M1
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 0);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 0);
	//M2
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 0);
	//M3
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
	//M4
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 0);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 0);
}

