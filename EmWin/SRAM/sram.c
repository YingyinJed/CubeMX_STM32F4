#include "sram.h"	  	     

//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))	
  						   
//初始化外部SRAM
void FSMC_SRAM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SRAM_HandleTypeDef FSMC_NORSRAM;
	FSMC_NORSRAM_TimingTypeDef Timing = {0};
	//使能PD,PE,PF,PG时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	//配置引脚PD0,1,4,5,8,9,10,11,12,13,14,15
	GPIO_InitStructure.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;//复用模式
  GPIO_InitStructure.Pull = GPIO_PULLUP;//上拉
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//100M时钟
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
	
	//FSMC配置
	FSMC_NORSRAM.Instance = FSMC_NORSRAM_DEVICE;
  FSMC_NORSRAM.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* FSMC_NORSRAM.Init */
	FSMC_NORSRAM.Init.NSBank = FSMC_NORSRAM_BANK3;  //NOR/SRAM的Bank3
	FSMC_NORSRAM.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE; //数据和地址线不复用
	FSMC_NORSRAM.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM; //SRAM存储模式
	FSMC_NORSRAM.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
	FSMC_NORSRAM.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE; //FLASH使用的,SRAM未使用
	FSMC_NORSRAM.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE; //是否使能同步传输模式下的等待信号,此处未用到
	FSMC_NORSRAM.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW; //等待信号的极性,仅在突发模式访问下有用
	FSMC_NORSRAM.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;  //是否使能环路突发模式,此处未用到
	FSMC_NORSRAM.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS; //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	FSMC_NORSRAM.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE; //存储器写使能
	FSMC_NORSRAM.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;   //等待使能位,此处未用到
	FSMC_NORSRAM.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE; //读写使用相同的时序
	FSMC_NORSRAM.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;  //异步传输期间的等待信号
	/* Timing */
	Timing.AddressSetupTime = 0X00; //地址建立时间0个HCLK 0ns
	Timing.AddressHoldTime = 0x00;  //地址保持时间,模式A未用到
	Timing.DataSetupTime = 0x09;    //数据保持时间,9个HCLK，6*9=54ns
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A; //模式A
	/* ExtTiming */

  if (HAL_SRAM_Init(&FSMC_NORSRAM, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  } 
}
	  														  
//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t n)
{
	for(;n!=0;n--)  
	{										    
		*(uint8_t*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    
//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FSMC_SRAM_ReadBuffer(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(uint8_t*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 
////////////////////////////////////////////////////////////////////////////////////////
//测试函数
//在指定地址写入1个字节
//addr:地址
//data:要写入的数据
void fsmc_sram_test_write(uint32_t addr,uint8_t data)
{			   
	FSMC_SRAM_WriteBuffer(&data,addr,1);//写入1个字节
}
//读取1个字节
//addr:要读取的地址
//返回值:读取到的数据
uint8_t fsmc_sram_test_read(uint32_t addr)
{
	uint8_t data;
	FSMC_SRAM_ReadBuffer(&data,addr,1);
	return data;
}










