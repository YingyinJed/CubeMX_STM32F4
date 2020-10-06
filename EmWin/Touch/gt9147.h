#ifndef __GT9147_H
#define __GT9147_H	
#include "main.h"	
#include "touch.h"
#include "ctiic.h"
#include "delay.h"
#include "string.h"
#include "ILI93xx.h" 

//IO��������	 
//GT9147��λ����
#define GT_RST_L   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET) 	
#define GT_RST_H		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)

//GT9147�ж�����	
#define GT_INT_L   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET) 	
#define GT_INT_H		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
 
//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������
  
//GT9147 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT9147���ƼĴ���
#define GT_CFGS_REG 	0X8047   	//GT9147������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X80FF   	//GT9147У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT9147��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT9147��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X8150  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8158		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X8160		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8168		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X8170		//��������������ݵ�ַ  
 
uint8_t GT9147_Send_Cfg(uint8_t mode);
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len); 
uint8_t GT9147_Init(void);
uint8_t GT9147_Scan(uint8_t mode); 
#endif













