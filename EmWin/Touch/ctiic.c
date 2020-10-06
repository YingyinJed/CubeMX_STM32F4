#include "ctiic.h"

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	delay_us(2);
} 
//���ݴ���оƬIIC�ӿڳ�ʼ��
//��ʼ������PB0(SCK),PF11(SDA)
void CT_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();//ʹ��GPIOB,Fʱ��
	
  GPIO_InitStructure.Pin = GPIO_PIN_0;//PB0����Ϊ�������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
  GPIO_InitStructure.Pull = GPIO_PULLUP;//����
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
		
  GPIO_InitStructure.Pin = GPIO_PIN_11;//PF11�����������
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��	

}
//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	CT_IIC_SDA_H;	  	  
	CT_IIC_SCL_H;
	delay_us(30);
 	CT_IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL_L;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	CT_IIC_SCL_H;
	delay_us(30);
	CT_IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA_H;//����I2C���߽����ź�  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t CT_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
	CT_IIC_SDA_H;	   
	CT_IIC_SCL_H;
	CT_Delay();
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL_L;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void CT_IIC_Ack(void)
{
	CT_IIC_SCL_L;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA_L;
	CT_Delay();
	CT_IIC_SCL_H;
	CT_Delay();
	CT_IIC_SCL_L;
}
//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL_L;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA_H;
	CT_Delay();
	CT_IIC_SCL_H;
	CT_Delay();
	CT_IIC_SCL_L;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
	CT_SDA_OUT(); 	    
  CT_IIC_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
	CT_Delay();
	for(t=0;t<8;t++)
    {              
        if (txd & 0x80)
			{
				CT_IIC_SDA_H;
			}
			else
			{
				CT_IIC_SDA_L;
			} 	      
			CT_Delay();
			CT_IIC_SCL_H;
			CT_Delay();
			CT_IIC_SCL_L;
			if (t == 7)
			{
				CT_IIC_SDA_H; 	// �ͷ�����
			}
			txd <<= 1;		// ����һ��bit 
			CT_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
	uint8_t i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ����
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL_L; 	    	   
		CT_Delay();
		CT_IIC_SCL_H;	 
		receive<<=1;
		if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}





























