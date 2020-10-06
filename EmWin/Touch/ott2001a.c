#include "ott2001a.h"

//��OTT2001Aд��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
uint8_t OTT2001A_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//��OTT2001A����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void OTT2001A_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(OTT_CMD_RD);   //���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����    
}
//��������/�رղ���
//cmd:1,�򿪴�����;0,�رմ�����
void OTT2001A_SensorControl(uint8_t cmd)
{
	uint8_t regval=0X00;
	if(cmd)regval=0X80;
	OTT2001A_WR_Reg(OTT_CTRL_REG,&regval,1); 
} 
//��ʼ��������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
uint8_t OTT2001A_Init(void)
{
 	uint8_t regval=0; 
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  //ʹ��GPIOB,Cʱ��	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
    //GPIOB1��ʼ������
  GPIO_InitStructure.Pin = GPIO_PIN_1 ;//PB1����Ϊ��������
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//����ģʽ
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//100MHz
  GPIO_InitStructure.Pull = GPIO_PULLUP;//����
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.Pin = GPIO_PIN_13;//PC13����Ϊ�������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//���ģʽ
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
 
	CT_IIC_Init();      	//��ʼ����������I2C����  
	OTT_RST_L;				//��λ
	delay_ms(100);
 	OTT_RST_H;				//�ͷŸ�λ		    
	delay_ms(100); 
	OTT2001A_SensorControl(1);	//�򿪴����� 
	OTT2001A_RD_Reg(OTT_CTRL_REG,&regval,1);//��ȡ���������мĴ�����ֵ���ж�I2Cͨ���Ƿ�����
    if(regval==0x80)return 0;
	return 1;
}

const uint16_t OTT_TPX_TBL[5]={OTT_TP1_REG,OTT_TP2_REG,OTT_TP3_REG,OTT_TP4_REG,OTT_TP5_REG};
//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
uint8_t OTT2001A_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	static uint8_t t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
		OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1);//��ȡ�������״̬  
		if(mode&0X1F)
		{
			tp_dev.sta=(mode&0X1F)|TP_PRES_DOWN|TP_CATH_PRES;
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					OTT2001A_RD_Reg(OTT_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
					if(tp_dev.touchtype&0X01)//����
					{
						tp_dev.y[i]=(((uint16_t)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
						tp_dev.x[i]=800-((((uint16_t)buf[0]<<8)+buf[1])*OTT_SCAL_X);
					}else
					{
						tp_dev.x[i]=(((uint16_t)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
						tp_dev.y[i]=(((uint16_t)buf[0]<<8)+buf[1])*OTT_SCAL_X;
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
			t=0;		//����һ��,��������������10��,�Ӷ����������
		}
	}
	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}
 































 




























