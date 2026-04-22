#include "stm32f10x.h"                  // Device header



void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}


/*

开启spi1时钟，APB2外设
4号引脚还是软件模拟从机选择线
5与7引脚配置为复用推挽输出给外设
结构体配置SPI_InitTypeDef
使能spi
ss默认高电平


*/
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//开启spi1时钟，APB2外设

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;//4号引脚还是软件模拟从机选择线
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_7;//5与7引脚配置为复用推挽输出给外设
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	SPI_InitTypeDef SPI_InitTypeStruct;//结构体配置SPI_InitTypeDef
	SPI_InitTypeStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//频率根据需求来
	SPI_InitTypeStruct.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitTypeStruct.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitTypeStruct.SPI_CRCPolynomial=7;
	SPI_InitTypeStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitTypeStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitTypeStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitTypeStruct.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitTypeStruct);
	
	SPI_Cmd(SPI1,ENABLE);//使能spi

	MySPI_W_SS(1);
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
读取标志位TXE为空
写入数据
读取标志位RXNE非空
读取数据
*/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	
	while( SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET) ;
	SPI_I2S_SendData(SPI1,ByteSend);//内部只带等待忙状态结束
	while( SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET );
	
	return SPI_I2S_ReceiveData(SPI1);
}


