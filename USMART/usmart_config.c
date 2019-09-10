#include "usmart.h"
#include "usmart_str.h"

#include "led.h"
#include "delay.h"
#include "mypwm.h"
#include "control.h"

extern void mode_change(uint8_t modee);
extern void mode_2_length(uint8_t mode_2_radiuss);
extern void mode_3_angel_change(uint8_t mode_3_angell);

//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
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



//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};


