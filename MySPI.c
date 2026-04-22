#include "stm32f10x.h"                  // Device header



void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}	


void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}



void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}

void MySPI_Start()
{
	MySPI_W_SS(0);
}

void MySPI_Stop()
{
	MySPI_W_SS(1);
}

/*
模式0，可根据相位图，改为模式1,2,3,就更改下面代码的顺序和sck是低为启动还是高位启动
	MySPI_W_MOSI(ByteSend & ( 0x80 >> i ) );
	MySPI_W_SCK(1);
	两个变量，一个用于存放需要发送的数据
	一个用于存放需要接收的数据
*/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ReceiveData = 0x00,i;
	for(i=0;i<8;i++)
	{
	MySPI_W_MOSI(ByteSend & ( 0x80 >> i ) );//发送数据
	MySPI_W_SCK(1);
	if(MySPI_R_MISO()) { ReceiveData |= ( 0x80 >> i );} ;//接收数据
	MySPI_W_SCK(0);
	}
	
	return ReceiveData;
}

/*
	一个变量，同时存放需要发送的数据和接收数据，更符合移位模型
	发出高位左移接收数据存放到低位
*/
uint8_t MySPI_SwapByte_2(uint8_t ByteSend)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
	MySPI_W_MOSI(ByteSend & 0x80  );//取最高位
	ByteSend <<= 1 ;
	MySPI_W_SCK(1);
	if(MySPI_R_MISO()) { ByteSend |=  0x01 ;} ;//将数据依次存放到低位
	MySPI_W_SCK(0);
	}
	
	return ByteSend;
}



