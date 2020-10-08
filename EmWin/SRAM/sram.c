#include "sram.h"	  	     

//ʹ��NOR/SRAM�� Bank1.sector3,��ַλHADDR[27,26]=10 
//��IS61LV25616/IS62WV25616,��ַ�߷�ΧΪA0~A17 
//��IS61LV51216/IS62WV51216,��ַ�߷�ΧΪA0~A18
#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))	
  						   
//��ʼ���ⲿSRAM
void FSMC_SRAM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SRAM_HandleTypeDef FSMC_NORSRAM;
	FSMC_NORSRAM_TimingTypeDef Timing = {0};
	//ʹ��PD,PE,PF,PGʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	//��������PD0,1,4,5,8,9,10,11,12,13,14,15
	GPIO_InitStructure.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;//����ģʽ
  GPIO_InitStructure.Pull = GPIO_PULLUP;//����
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//100Mʱ��
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 \
																|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15); //PE0,1,7,8,9,10,11,12,13,14,15
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12 \
																|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15); //PF0,1,2,3,4,5,12,13,14,15
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10 ; //PG0,1,2,3,4,5,10
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//FSMC����
	FSMC_NORSRAM.Instance = FSMC_NORSRAM_DEVICE;
  FSMC_NORSRAM.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* FSMC_NORSRAM.Init */
	FSMC_NORSRAM.Init.NSBank = FSMC_NORSRAM_BANK3;  //NOR/SRAM��Bank3
	FSMC_NORSRAM.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE; //���ݺ͵�ַ�߲�����
	FSMC_NORSRAM.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM; //SRAM�洢ģʽ
	FSMC_NORSRAM.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16; //16λ���ݿ��
	FSMC_NORSRAM.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE; //FLASHʹ�õ�,SRAMδʹ��
	FSMC_NORSRAM.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE; //�Ƿ�ʹ��ͬ������ģʽ�µĵȴ��ź�,�˴�δ�õ�
	FSMC_NORSRAM.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW; //�ȴ��źŵļ���,����ͻ��ģʽ����������
	FSMC_NORSRAM.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;  //�Ƿ�ʹ�ܻ�·ͻ��ģʽ,�˴�δ�õ�
	FSMC_NORSRAM.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS; //�洢�����ڵȴ�����֮ǰ��һ��ʱ�����ڻ��ǵȴ������ڼ�ʹ��NWAIT
	FSMC_NORSRAM.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE; //�洢��дʹ��
	FSMC_NORSRAM.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;   //�ȴ�ʹ��λ,�˴�δ�õ�
	FSMC_NORSRAM.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE; //��дʹ����ͬ��ʱ��
	FSMC_NORSRAM.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;  //�첽�����ڼ�ĵȴ��ź�
	/* Timing */
	Timing.AddressSetupTime = 0X00; //��ַ����ʱ��0��HCLK 0ns
	Timing.AddressHoldTime = 0x00;  //��ַ����ʱ��,ģʽAδ�õ�
	Timing.DataSetupTime = 0x09;    //���ݱ���ʱ��,9��HCLK��6*9=54ns
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A; //ģʽA
	/* ExtTiming */

  if (HAL_SRAM_Init(&FSMC_NORSRAM, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  } 
}
	  														  
//��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t n)
{
	for(;n!=0;n--)  
	{										    
		*(uint8_t*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    
//��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(uint8_t*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 
////////////////////////////////////////////////////////////////////////////////////////
//���Ժ���
//��ָ����ַд��1���ֽ�
//addr:��ַ
//data:Ҫд�������
void fsmc_sram_test_write(uint32_t addr,uint8_t data)
{			   
	FSMC_SRAM_WriteBuffer(&data,addr,1);//д��1���ֽ�
}
//��ȡ1���ֽ�
//addr:Ҫ��ȡ�ĵ�ַ
//����ֵ:��ȡ��������
uint8_t fsmc_sram_test_read(uint32_t addr)
{
	uint8_t data;
	FSMC_SRAM_ReadBuffer(&data,addr,1);
	return data;
}










