#include "control.h"
#include "led.h"
#include "pid.h"
#include "mypwm.h"
#include "math.h"
#include "myuart.h"

float warebuf[8];

//					x      y 
extern float pitch,roll,yaw; 			//ŷ����  
extern PIDTypedef PID_Struct_x,PID_Struct_y;

uint32_t mode_time = 0;
uint32_t mode_time1 = 0;
uint8_t  mode_OK = 0;

Mode_Typedef Mode_Struct;

int32_t out_x_max =600;
int32_t out_y_max =600;
	
void Mode_Struct_Init(void)
{
	Mode_Struct.radius = 25;			//�뾶 cm
	Mode_Struct.hight  = 79;			//�߶� cm
	Mode_Struct.period = 1560;    //�ڶ����� ms
	Mode_Struct.angel  = 90;      //��ʼ��90��
	Mode_Struct.X      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*cos(3.14159*Mode_Struct.angel/180);  //��Ӧ�ķ���
	Mode_Struct.Y      = atan((float)Mode_Struct.radius/Mode_Struct.hight) *57.2958*sin(3.14159*Mode_Struct.angel/180);
	Mode_Struct.omega  = 2*3.14159/Mode_Struct.period;           //ŷ�׸�       //г�� Asin(wt)
	Mode_Struct.time   = 0;       //��λʱ��
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

//г���˶� 
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
	


//�Ӿ�ֹ��ʼ�� 15s �ڿ��Ʒ��������������ɰ��˶��� ʹ������ȶ���
//�ڵ��滭��һ�����Ȳ����� 50cm ��ֱ�߶Σ� �����Զ�ƫ����ڡ�2.5cm�� ���Ҿ��нϺõ��ظ��ԣ�
void mode_1(void)
{
	Resonant_Motion();
}

//�Ӿ�ֹ��ʼ�� 15s ����ɷ��ȿɿصİڶ������������� 30~60cm �����
//�ã� ����ƫ����ڡ�2.5cm ��ֱ�߶Σ����Ҿ��нϺõ��ظ��ԣ�
void mode_2(void)
{	
	if(Mode_Struct.radius != 35)  //�����ʱ��ʼ���뾶Ϊ35  ����뾶��Ϊ35��ֱ��70�� ���ı��˰뾶 ��ʼ����  
	{
		Resonant_Motion();
	}
}

//���趨�ڶ����򣬷����ڴӾ�ֹ��ʼ�� 15s �ڰ������õķ��򣨽Ƕȣ�
//�ڶ��� ���������� 20cm ��ֱ�߶Σ�
void mode_3(void)
{
	if(Mode_Struct.angel != 111)  //������ʱ��ʼ���Ƕ�Ϊ111  ����ǶȲ�Ϊ111 ���ı��˽Ƕ� ��ʼ����  
	{
		Resonant_Motion();
	}
}

//������������һ���Ƕȣ�30�� ~45�� �� �ſ��� 5s ��ʹ�������ƶ��ﵽ��
//ֹ״̬��
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

//�Է����ھ�ֹʱ����ʵĹ��ΪԲ�ģ� �����������ü�����ڵ��滭Բ�� 30s �����ظ� 3 �Σ�
//Բ�뾶���� 15~35cm ��Χ�����ã� ����ʻ����Ĺ켣Ӧ����ָ���뾶��2.5cm ��Բ���ڣ�
void mode_5(void)
{
	if(Mode_Struct.radius != 35)  //������ʱ��ʼ���뾶Ϊ35  ����뾶��Ϊ35��ֱ��70�� ���ı��˰뾶 ��ʼ����  
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

//�ڷ��Ӳ��֣�1���������Բ���˶����ھ�������� 1~2m ��������һ̨ 50~60W ̨����ˮƽ����������ڣ�̨�ȴ� 5s ��ֹͣ�� 
//�������ܹ��� 5s �ڻָ����Ӳ��֣�1�� �涨��Բ���˶�������ʻ�������Ҫ��Ĺ켣��
void mode_6(void)
{
	mode_5();
}

//����
//��������
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



