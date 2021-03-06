#ifndef __MYIIC_H
#define __MYIIC_H
#include "main.h" 
#include "gpio.h"
#include "delay.h"
	
   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
//IO操作函数	 
//SCL(PB8)
#define IIC_SCL_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) 	
#define IIC_SCL_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)

//SDA(PB9)
#define IIC_SDA_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET) 	
#define IIC_SDA_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)

#define IIC_READ_SDA   	(GPIOB->IDR & GPIO_PIN_9)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















