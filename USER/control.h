#ifndef __CONTROL_H
#define __CONTROL_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t		radius;  //�뾶 cm
	uint8_t		hight;   //�߶� cm
	uint16_t	period;	 //�ڶ����� ms
	uint16_t 	angel;   //�Ƕ�
	float     Y		;		 //����
	float     X		;		 //����
	float			omega;   //ŷ�׸�
	uint32_t  time;		 //ʱ��
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


