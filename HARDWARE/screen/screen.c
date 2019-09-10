#include "screen.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "math.h"

#define PI 3.1415
typedef struct
{
	int r_long;
	int angle;
}Long_Angle;
Long_Angle long_angle;

volatile	int x_before1,x_before2,x_before3,y_before1,y_before2,y_before3,
		  x_new1,   x_new2   ,x_new3   ,y_new1   ,y_new2   ,y_new3;
volatile	double	x_angle	, y_long;
	
int num_screen = 0;
uint8_t  startBit_Screen = 0;
uint8_t  endBit_Screen = 0;
uint16_t intpurscreenString[80] = {0};
uint16_t SccreenBit = 0;
extern void mode_change(uint8_t modee);																			//进行任务切换 取值0~7  0对应停止 
extern void mode_2_length(uint8_t mode_2_radiuss);
extern void mode_5_length(uint8_t mode_5_radiuss);
extern void mode_3_angel_change(uint16_t mode_3_angell);											//任务3改变角度的函数
extern void mode_7_change(uint16_t mode_7_angell, uint8_t mode_7_radiuss);		//定点 angel角度 0~360  radius半径 0~~30

//中断服务函数
void ScreenIntHandler(void)
{
		uint32_t flag = UARTIntStatus(SCREEN_GPIO_BASE,1);
		//清除中断标志
		UARTIntClear(SCREEN_GPIO_BASE,flag);

		SccreenBit =UARTCharGet(SCREEN_UART_BASE);	//读取接收到的数据
		
	
		//接受任务
		if(SccreenBit == '$' || SccreenBit =='%' || SccreenBit=='&'|| SccreenBit=='@' ||SccreenBit=='L' || SccreenBit=='*' || SccreenBit=='g')
	    {
	      	startBit_Screen = 1;
					num_screen = 0;
	    }
	    if(startBit_Screen == 1)
	    {
	       	intpurscreenString[num_screen] = SccreenBit; 
					
	    }  
	    if (startBit_Screen == 1 && SccreenBit == '#') 
	    {		
					endBit_Screen = 1; 
					startBit_Screen = 0;
				
				//切换任务
				if(	intpurscreenString[0]=='$' ||	intpurscreenString[0]=='L')
				{
					mode_change(intpurscreenString[1]-48);
					printf("%d ",intpurscreenString[0]);
				}
				//输入半径2			
				if(	intpurscreenString[0]=='%')
				{
					mode_2_length(set_long());
				}
				//输入半径5	
				if(	intpurscreenString[0]=='*')
				{
					mode_5_length(set_long());
				}
				//角度
				if(	intpurscreenString[0]=='&')
				{
					mode_3_angel_change(set_angle());						
				}
				//任务7
				if(	intpurscreenString[0]=='@')
				{
					set_long_angle();
					mode_7_change(long_angle.angle,long_angle.r_long);	
				}
				//坐标换算
				if( intpurscreenString[0]=='g' )
				{
					double x=intpurscreenString[1]*256+intpurscreenString[2]-400;
					double y=intpurscreenString[3]*256+intpurscreenString[4]-240;
					printf("x====%f         y=====%f\n",x,y);
					double l=sqrt(x*x+y*y);
					double angle1=asin(y/l);
					double angle;
					if(x>0 && y>0)
					{
						 angle=angle1/PI*180+180;
					}
					if(x<0 && y>0)
					{
						angle=90-angle1/PI*180+270;
					}
					if(x<0 && y<0)
					{
						angle=-angle1/PI*180;
					}
					if(x>0 && y<0)
					{
						angle=90+angle1/PI*180+90;
					}
					mode_7_change(l/14,angle);
					printf("x*x+y*y====%f         angle=====%f\n",l/14,angle);x=0,y=0;
				}
	    }
			num_screen++;
			if(num_screen >= 80)
				{
					num_screen = 0;
					startBit_Screen = 0;
					endBit_Screen	= 0;
				}	 
}


//初始化
void Screen_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN_GPIO);
//	//GPIO复用模式配置
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xFF;
	
	GPIOPinConfigure(SCREEN_GPIO_RX);
	GPIOPinConfigure(SCREEN_GPIO_TX);
	GPIOPinTypeUART(SCREEN_GPIO_BASE, SCREEN_GPIO_PIN_RX |SCREEN_GPIO_PIN_TX);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(SCREEN_UART_BASE, SysCtlClockGet(),
                    SCREEN_BAUD, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(SCREEN_GPIO_BASE);
	//使能UART0中断
	IntEnable(SCREEN_INT_UART);
	//使能UART0接收中断
	UARTIntEnable(SCREEN_UART_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(SCREEN_UART_BASE,ScreenIntHandler);
	
	IntPrioritySet(SCREEN_INT_UART, 3<<5);
	
	
}

//写数组
void txt_putbuff(char *buff, uint32_t len)
{
	while(len--)
	{
		UARTCharPut(UART2_BASE,*buff);
		buff++;
	}
}

//显示时间函数
void sand_txt_time(uint16_t t1)				//t1为1s t2为0.1s
{
	int t10s=t1/100+48,
		  t1s =(t1/10)%10+48,
			t01s=t1%10+48;
		
			
			
	char txt[13]={0};
			 txt[0]='t';
			 txt[1]='2';
			 txt[2]='.';
			 txt[3]='t';
			 txt[4]='x';
			 txt[5]='t';
			 txt[6]='=';
			 txt[7]='"';
			 txt[8]=t10s;
			 txt[9]=t1s;
			 txt[10]='.';
			 txt[11]=t01s;
			 txt[12]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,13);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}



//显示轨迹
void draw_line(double angle,double r_long)
{
	//x145

	

			x_before1=x_new1;
			x_before2=x_new2;
			x_before3=x_new3;
			y_before1=y_new1;
			y_before2=y_new2;
			y_before3=y_new3;
      x_angle=-r_long*cos(3.1415*angle/180)*6+400;
	    y_long=-r_long*sin(3.1415*angle/180)*6+240;
			x_new1=(int)x_angle/100+48;
			x_new2=(int)(x_angle/10)%10+48;
			x_new3=(int)x_angle%10+48;
			y_new1=(int)y_long/100+48;
			y_new2=(int)(y_long/10)%10+48;
			y_new3=(int)y_long%10+48;
	if(x_before1==0 && x_before2==0 && x_before3==0)
	{
			x_before1=x_new1;
			x_before2=x_new2;
			x_before3=x_new3;
			y_before1=y_new1;
			y_before2=y_new2;
			y_before3=y_new3;
	}
	char  line_down[24]={0};
		line_down[0]='l';
		line_down[1]='i';
		line_down[2]='n';
		line_down[3]='e';
		line_down[4]=' ';
		line_down[5]=x_before1;
		line_down[6]=x_before2;
		line_down[7]=x_before3;
		line_down[8]=',';
		line_down[9]=y_before1;
		line_down[10]=y_before2;
		line_down[11]=y_before3;
		line_down[12]=',';
		line_down[13]=x_new1;
		line_down[14]=x_new2;
		line_down[15]=x_new3;
		line_down[16]=',';
		line_down[17]=y_new1;
		line_down[18]=y_new2;
		line_down[19]=y_new3;
		line_down[20]=',';
		line_down[21]='R';
		line_down[22]='E';
		line_down[23]='D';
			double Hex_end[3]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) line_down,24);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
			printf("x=%f   ",x_angle);
			printf("y=%f   \n",y_long);
			printf("x_before1=%d   ",x_before1);
			printf("x_before2=%d   ",x_before2);
			printf("x_before3=%d   \n",x_before3);
			printf("y_before1=%d   ",y_before1);
			printf("y_before2=%d   ",y_before2);
			printf("y_before3=%d   \n",y_before3);
			printf("x_new1=%d   ",x_new1);
			printf("x_new2=%d   ",x_new2);
			printf("x_new3=%d   \n",x_new3);
			printf("y_new1=%d   ",y_new1);
			printf("y_new2=%d   ",y_new2);
			printf("y_new3=%d   \n\n\n\n\n",y_new3);
}


//显示圈数

void clr_num(int i)
{
	char crl_num[12]={0};
		crl_num[0]='t';
		crl_num[1]='c';
		crl_num[2]='l';
		crl_num[3]='r';
		crl_num[4]='.';
		crl_num[5]='t';
		crl_num[6]='x';
		crl_num[7]='t';
		crl_num[8]='=';
		crl_num[9]='"';
		crl_num[10]=i-48;
		crl_num[11]='"';
	double Hex_end[4]={0};
	  Hex_end[0]=0xff;
	  Hex_end[1]=0xff;
	  Hex_end[2]=0xff;
		txt_putbuff((char*) crl_num,12);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
	

}

//设置角度
uint16_t set_angle()
{
		int findend=1;
		int angle=0;
		for(;intpurscreenString[findend]!='#';findend++)
		{
			angle=angle*10+intpurscreenString[findend]-48;

		}

		return angle;
		
}

//设置长度
uint8_t set_long()
{
		int findend=1;
		int r_long=0;
		for(;intpurscreenString[findend]!='#';findend++)
		{
			r_long=r_long*10+intpurscreenString[findend]-48;
			//printf("circle_num=%d\n",r_long);
		}
		//printf("circle_num_end=%d",r_long);
		return r_long;
		
}


//设置角度+长度

void set_long_angle()
{

		int findend1=1;
	  int findend2=0;
  	long_angle.angle=0;
		long_angle.r_long=0;
		for(;intpurscreenString[findend1]!='+';findend1++)
		{
			long_angle.angle=long_angle.angle*10+intpurscreenString[findend1]-48;
		}

		findend2=findend1+1;
		for(;intpurscreenString[findend2]!='#';findend2++)
		{
			long_angle.r_long=long_angle.r_long*10+intpurscreenString[findend2]-48;
		}

		
}
//double change_pwm()
//{
//	int i=2;
//	double a;
//	for(;intpurscreenString[i]=='#';i++)
//	{
//			if(intpurscreenString[i]!='#')
//			{
//			if(intpurscreenString[i]=='.')
//			{
//				a=a*10+intpurscreenString[i];
//				a=a/10;
//			}
//			a=a*10+intpurscreenString[i];
//		}
//	}
//	return a;
//}



//double change_pid()
//{
//	int i=2;
//	double a;
//	for(;intpurscreenString[i]=='#';i++)
//	{
//			if(intpurscreenString[i]!='#')
//			{
//			if(intpurscreenString[i]=='.')
//			{
//				a=a*10+intpurscreenString[i];
//				a=a/10;
//			}
//			a=a*10+intpurscreenString[i];
//		}
//	}
//	return a;
//}


////显示下中值
//void show_center_down(float a)
//{
//	int b,t1,t2,t3,t4;
//	b=a*10;
//	t1=(b/1000)+48;
//	t2=(b/100)%10+48;
//	t3=(b/10)%10+48;
//	t4=(b%10)+48;
//	char  center_down[14]={0};
//				center_down[0]='t';
//				center_down[1]='d';
//				center_down[2]='.';
//				center_down[3]='t';
//				center_down[4]='x';
//				center_down[5]='t';
//				center_down[6]='=';
//				center_down[7]='"';
//				center_down[8]=t1;
//				center_down[9]=t2;
//				center_down[10]=t3;
//				center_down[11]='.';
//				center_down[12]=t4;
//				center_down[13]='"';
//	double Hex_end[4]={0};
//				 Hex_end[0]=0xff;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;
//		txt_putbuff((char*) center_down,14);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);

//}


////显示波形
//void sand_waveform(int a)//a最大为2048
//{
//	char pwm_waveform[15];
//	int b,a1,a2,a3;
////	if(a>90)
////		{
////			b=(a-90)/1.4;
////		}
////		else {
////		b=(a+270)/1.4;
////		}
//	b=a/1.4;
//	a1=(b/100)%10+48;
//	a2=(b/10)%10+48;
//	a3=b%10+48;
//	pwm_waveform[0]='a';
//	pwm_waveform[1]='d';
//	pwm_waveform[2]='d';
//	pwm_waveform[3]=' ';
//	pwm_waveform[4]='2';
//	pwm_waveform[5]=',';
//	pwm_waveform[6]='1';
//	pwm_waveform[7]=',';
//	pwm_waveform[8]=a1;
//	pwm_waveform[9]=a2;
//	pwm_waveform[10]=a3;

//	double Hex_end[4]={0};
//				 Hex_end[0]=0xFF;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;	
//		txt_putbuff((char*) pwm_waveform,11);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//}



////显示波形值
//void show_center(double a)
//{
//	int b,t1,t2,t3,t4;
//	b=a*10;
//	t1=(b/1000)+48;
//	t2=(b/100)%10+48;
//	t3=(b/10)%10+48;
//	t4=(b%10)+48;
//		char centerr[15]={0};
//					centerr[0]='t';
//					centerr[1]='1';
//					centerr[2]='5';
//					centerr[3]='.';
//					centerr[4]='t';
//					centerr[5]='x';
//					centerr[6]='t';
//					centerr[7]='=';
//					centerr[8]='"';
//					centerr[9]=t1;
//					centerr[10]=t2;
//					centerr[11]=t3;
//					centerr[12]='.';
//					centerr[13]=t4;
//					centerr[14]='"';
//	double Hex_end[4]={0};
//				 Hex_end[0]=0xff;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;
//		txt_putbuff((char*) centerr,15);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);

//}



////显示上中值
//void show_center_up(float a)
//{
//	int b,t1,t2,t3,t4;
//	b=a*10;
//	t1=(b/1000)+48;
//	t2=(b/100)%10+48;
//	t3=(b/10)%10+48;
//	t4=(b%10)+48;
//		char center_up[14]={0};
//					center_up[0]='t';
//					center_up[1]='u';
//					center_up[2]='.';
//					center_up[3]='t';
//					center_up[4]='x';
//					center_up[5]='t';
//					center_up[6]='=';
//					center_up[7]='"';
//					center_up[8]=t1;
//					center_up[9]=t2;
//					center_up[10]=t3;
//					center_up[11]='.';
//					center_up[12]=t4;
//					center_up[13]='"';
//	double Hex_end[4]={0};
//				 Hex_end[0]=0xff;
//				 Hex_end[1]=0xff;
//				 Hex_end[2]=0xff;
//		txt_putbuff((char*) center_up,14);
//		UARTCharPut(UART0_BASE,Hex_end[0]);		
//		UARTCharPut(UART2_BASE,Hex_end[0]);
//		UARTCharPut(UART0_BASE,Hex_end[1]);		
//		UARTCharPut(UART2_BASE,Hex_end[1]);
//		UARTCharPut(UART0_BASE,Hex_end[2]);
//		UARTCharPut(UART2_BASE,Hex_end[1]);

//}



////设置中值
//double set_center()
//{
//	int findend=2;
//	int flag=1;
//	double center_up=0;
//	double center_down=0;
//	double set_center=0;
//	if(intpurscreenString[1]=='u')
//	{
//			for(;intpurscreenString[findend]!='#';findend++)
//			{

//				if(flag==0)
//				{
//						center_up=center_up*10+intpurscreenString[findend]-48;
//						center_up=center_up/10;	
//				}	
//				if(intpurscreenString[findend]=='.')
//				{
//						flag=0;
//				}
//				if(flag==1)
//				{
//					center_up=center_up*10+intpurscreenString[findend]-48;
//					
//				}
//			}
//			set_center= center_up;
//			//printf("cnter_up_end=%f\n",center_up);
//	}
//	if(intpurscreenString[1]=='d')
//	{
//			for(;intpurscreenString[findend]!='#';findend++)
//			{

//				if(flag==0)
//				{
//						center_down=center_down*10+intpurscreenString[findend]-48;
//						center_down=center_down/10;
//				}	
//				if(intpurscreenString[findend]=='.')
//				{
//						flag=0;
//				}
//				if(flag==1)
//				{
//					center_down=center_down*10+intpurscreenString[findend]-48;
//				}
//			}
//			//printf("cnter_up=%f",center_up);
//			set_center= center_down;
//	}
//	return set_center;
//}

////设置圈数
//uint8_t set_cricle()
//{
//		int findend=2;
//		int circle_num;
//		for(;intpurscreenString[findend]!='#';findend++)
//		{
//			circle_num=circle_num*10+intpurscreenString[findend]-48;
//			printf("circle_num=%d\n",circle_num);
//		}
//		printf("circle_num_end=%d",circle_num);
//		return circle_num;
//		
//}

