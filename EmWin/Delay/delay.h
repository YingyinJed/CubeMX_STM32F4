#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"
#include "tim.h"
//���÷�Ƶ��84MHz��Ƶ��ʱ������TIM7��ɵ�������ʱ
void delay_us(uint16_t us);
//���õδ�ʱ����ɵ���ʱ
void delay_ms(int32_t nms);

#endif
