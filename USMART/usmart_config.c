#include "usmart.h"
#include "usmart_str.h"

#include "led.h"
#include "delay.h"
#include "mypwm.h"
#include "control.h"

extern void mode_change(uint8_t modee);
extern void mode_2_length(uint8_t mode_2_radiuss);
extern void mode_3_angel_change(uint8_t mode_3_angell);

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 xms)",
 	(void*)delay_us,"void delay_us(u32 xus)",	
	(void*)LED_RED_DEBUG, "void LED_RED_DEBUG(bool a)",
	(void*)LED_BLUE_DEBUG, "void LED_BLUE_DEBUG(bool a)",
	(void*)LED_GREEN_DEBUG, "void LED_GREEN_DEBUG(bool a)",
	(void*)M1_Forward,		"void M1_Forward(int32_t width)",
	(void*)M1_Backward,		"void M1_Backward(int32_t width)",
	(void*)M2_Forward,		"void M2_Forward(int32_t width)",
	(void*)M2_Backward,		"void M2_Backward(int32_t width)",
	(void*)M3_Forward,		"void M3_Forward(int32_t width)",
	(void*)M3_Backward,		"void M3_Backward(int32_t width)",
	(void*)M4_Forward,		"void M4_Forward(int32_t width)",
	(void*)M4_Backward,		"void M4_Backward(int32_t width)",
	(void*)All_stop,			"void All_stop(void)",
	(void*)mode_change,		"void mode_change(uint8_t modee)",
	(void*)mode_2_length, "void mode_2_length(uint8_t mode_2_radiuss)",
	(void*)mode_3_angel_change,	"mode_3_angel_change(uint8_t mode_3_angell)",
};	



//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};


