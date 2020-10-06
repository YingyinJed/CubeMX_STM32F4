#include "delay.h"

void delay_us(uint16_t us)
{
	uint16_t differ=0xffff-us-5; 

	
	HAL_TIM_Base_Start(&htim7);
	__HAL_TIM_SetCounter(&htim7,differ); 
	while(differ < 0xffff-5) 
	{ 
		differ = __HAL_TIM_GetCounter(&htim7); 
	} 
	HAL_TIM_Base_Stop(&htim7);
 
}

void delay_ms(int32_t nms) 
 {
  int32_t temp; 
  SysTick->LOAD = 8000*nms; 
  SysTick->VAL=0X00;//��ռ����� 
  SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ 
  do 
  { 
       temp=SysTick->CTRL;//��ȡ��ǰ������ֵ 
  }
     while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽�� 
     
     SysTick->CTRL=0x00; //�رռ����� 
     SysTick->VAL =0X00; //��ռ����� 
 } 
