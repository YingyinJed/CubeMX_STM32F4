#ifndef __MYIIC_H
#define __MYIIC_H
#include "main.h" 
#include "gpio.h"
#include "delay.h"
	
   	   		   
//IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
//SCL(PB8)
#define IIC_SCL_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) 	
#define IIC_SCL_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)

//SDA(PB9)
#define IIC_SDA_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET) 	
#define IIC_SDA_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)

#define IIC_READ_SDA   	(GPIOB->IDR & GPIO_PIN_9)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















