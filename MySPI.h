#ifndef MYSPI_H
#define	MYSPI_H

void MySPI_Init(void);

void MySPI_Start(void);

void MySPI_Stop(void);
/*
模式0，可根据相位图，改为模式1,2,3,就更改下面代码的顺序和sck是低为启动还是高位启动
	MySPI_W_MOSI(ByteSend & ( 0x80 >> i ) );
	MySPI_W_SCK(1);
	两个变量，一个用于存放需要发送的数据
	一个用于存放需要接收的数据
*/
uint8_t MySPI_SwapByte(uint8_t ByteSend);

/*
	一个变量，同时存放需要发送的数据和接收数据，更符合移位模型
	发出高位左移接收数据存放到低位
*/
uint8_t MySPI_SwapByte_2(uint8_t ByteSend);


#endif







