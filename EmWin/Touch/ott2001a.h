#ifndef __OTT2001A_H
#define __OTT2001A_H	
#include "main.h"	
#include "touch.h"
#include "ctiic.h"
#include "delay.h"

//IO��������	 
//OTT2001A��λ����
#define OTT_RST_L   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET) 	
#define OTT_RST_H   	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET) 	

//OTT2001A�ж�����	
#define OTT_INT_L   	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET) 	
#define OTT_INT_H			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

//ͨ��OTT_SET_REGָ��,���Բ�ѯ�������Ϣ 
//ע��,�����X,Y����Ļ������ϵ�պ��Ƿ���.
#define OTT_MAX_X 		2700	 	//TP X��������ֵ(������)
#define OTT_MAX_Y 		1500    	//TP Y��������ֵ(�᷽��)

//��������
#define OTT_SCAL_X		0.2963		//��Ļ�� ������/OTT_MAX_X		
#define OTT_SCAL_Y		0.32		//��Ļ�� ������/OTT_MAX_Y		
 
//I2C��д����	
#define OTT_CMD_WR 		0XB2     	//д����
#define OTT_CMD_RD 		0XB3		//������
 
//�Ĵ�����ַ
#define OTT_GSTID_REG 	0X0000   	//OTT2001A��ǰ��⵽�Ĵ������
#define OTT_TP1_REG 	0X0100  	//��һ�����������ݵ�ַ
#define OTT_TP2_REG 	0X0500		//�ڶ������������ݵ�ַ
#define OTT_TP3_REG 	0X1000		//���������������ݵ�ַ
#define OTT_TP4_REG 	0X1400		//���ĸ����������ݵ�ַ
#define OTT_TP5_REG 	0X1800		//��������������ݵ�ַ  
#define OTT_SET_REG   	0X0900   	//�ֱ������üĴ�����ַ
#define OTT_CTRL_REG  	0X0D00   	//����������(��/��)  

#define OTT_MAX_TOUCH  5    		//������֧�ֵĵ���,�̶�Ϊ5��
 	

uint8_t OTT2001A_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);		//д�Ĵ���(ʵ������)
void OTT2001A_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);	//���Ĵ���
void OTT2001A_SensorControl(uint8_t cmd);//��������/�رղ���
uint8_t OTT2001A_Init(void); 			//4.3���ݴ�����ʼ������
uint8_t OTT2001A_Scan(uint8_t mode);			//���ݴ�����ɨ�躯��

#endif












