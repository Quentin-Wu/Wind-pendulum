#ifndef __CONTROL_H
#define __CONTROL_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t		radius;  //半径 cm
	uint8_t		hight;   //高度 cm
	uint16_t	period;	 //摆动周期 ms
	uint16_t 	angel;   //角度
	float     Y		;		 //幅度
	float     X		;		 //幅度
	float			omega;   //欧米噶
	uint32_t  time;		 //时间
}Mode_Typedef;

void Mode_Struct_Init(void);
void pid_set(void);
void count_time_init(void);

void mode_1(void);
void mode_2(void);
void mode_3(void);
void mode_4(void);
void mode_5(void);
void mode_6(void);
void mode_7(void);

void Motor_Out(int32_t out_x, int32_t out_y);


#endif


