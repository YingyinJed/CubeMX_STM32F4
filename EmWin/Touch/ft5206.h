#ifndef __FT5206_H
#define __FT5206_H	
#include "main.h"	
#include "touch.h"
#include "ctiic.h"
#include "delay.h" 
#include "string.h"

//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	 
//FT5206��λ����
#define FT_RST_L   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)
#define FT_RST_H   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)

//FT5206�ж�����	
#define FT_INT_L   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)
#define FT_INT_H   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���


uint8_t FT5206_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT5206_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t FT5206_Init(void);
uint8_t FT5206_Scan(uint8_t mode);

#endif

















