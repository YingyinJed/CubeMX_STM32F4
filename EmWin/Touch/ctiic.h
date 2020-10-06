#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "main.h"    	
#include "gpio.h"
#include "delay.h"
//IO��������
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11����ģʽ
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11���ģʽ
//IO��������	 
//SCL(PB0)
#define CT_IIC_SCL_L    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET) 	
#define CT_IIC_SCL_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
//SDA	(PF11)
#define CT_IIC_SDA_L   	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_RESET)  
#define CT_IIC_SDA_H   	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_SET)

#define CT_READ_SDA   	(GPIOF->IDR & GPIO_PIN_11)  //����SDA 


//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
uint8_t CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







