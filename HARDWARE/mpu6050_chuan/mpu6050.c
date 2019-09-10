#include "mpu6050.h"
#include "led.h"

#include "myuart.h"
#include "pid.h"
#include "led.h"
#include "control.h"
#include "mypwm.h"

unsigned char counter=0;
static unsigned char Temp[11];
float a[3],w[3],angle[3],T;

// float warebuf[8];
extern PIDTypedef PID_Struct_x,PID_Struct_y;
extern uint8_t mode;

void UART1IntHandler(void);

//ʹ��uart1
//RX  PC4
//TX  PC5
void MPU6050_Init(void)
{
	//ʹ��UART����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	//ʹ��GPIO����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO����ģʽ����
	GPIOPinConfigure(GPIO_PC4_U1RX);
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart���� �����ʣ�115200 8λ 1λֹͣλ ��У��
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
	//���ú����1λ�Ͳ����ж�
	UARTFIFODisable(UART1_BASE);
	//ʹ��UART1�ж�
	IntEnable(INT_UART1);
	//ʹ��UART1�����ж�
	UARTIntEnable(UART1_BASE,UART_INT_RX |UART_INT_RT);
	//UART�жϵ�ַע��
	UARTIntRegister(UART1_BASE,UART1IntHandler);
	//�����ж����ȼ�
	IntPrioritySet(INT_UART1, 0<<5);
}


void UART1IntHandler(void)
{
	uint32_t flag = UARTIntStatus(UART1_BASE,1);
	UARTIntClear(UART1_BASE,flag);
	
	Temp[counter] =UARTCharGet(UART1_BASE);	  //��ȡ���յ�������
	
	if(counter == 0 && Temp[0] != 0x55)  return;    //�� 0 �����ݲ���֡ͷ������
//	if(counter == 1 && Temp[1] != 0x53)  return;  //��1�����ݲ�Ϊ�ǶȰ�������
	
	counter++; 
	if(counter==11) //���յ� 11 ������
	{ 
		 counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���

			if(Temp[1] == 0x53) //��ʶ������ǽǶȰ�
			{
				angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
				angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ

//				warebuf[0] = angle[0];
//				warebuf[1] = angle[1];
//				vcan_sendware((uint8_t *)warebuf,sizeof(warebuf));
				LED_GREEN_TUGGEL;
				PID_Struct_x.get = -angle[1];
				PID_Struct_y.get = angle[0];
				
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
//				printf("%f    %f\n",angle[0],angle[1]);
			}
		}
}

















//	if(sign)
//      {  
//         memcpy(Temp,Re_buf,11);
//         sign=0;
//				
//         if(Re_buf[0]==0x55)       //���֡ͷ
//         {  
//            switch(Re_buf[1])
//            {
//               case 0x51: //��ʶ������Ǽ��ٶȰ�
//                  a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X����ٶ�
//                  a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y����ٶ�
//                  a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z����ٶ�
//                  T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //�¶�
////							 LED_BLUE(1);
//                  break;
//               case 0x52: //��ʶ������ǽ��ٶȰ�
//                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X����ٶ�
//                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y����ٶ�
//                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z����ٶ�
//                  T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //�¶�
////									LED_RED(1);
//                  break;
//               case 0x53: //��ʶ������ǽǶȰ�
//                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
//                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
//                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
//                  T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //�¶�

//                  //printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
//                  break;
//               default:  break;
//            }
//			printf("X�Ƕȣ�%.2f  Y�Ƕȣ�%.2f  Z�Ƕȣ�%.2f  X�ٶȣ�%.2f  Y�ٶȣ�%.2f  Z�ٶȣ�%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);
//         }
//			 }
	
	














