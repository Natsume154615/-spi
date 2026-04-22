#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	 MySPI_Init();
}


/*
	1.开启时序
	2.发送0x9f指令码，获取设备ID
	3.停止时序
*/
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//一个字节的厂商ID
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//两个字节的设备ID
	*DID <<=8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
	
}

/*写使能*/
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}


/*
判断状态寄存器中的busy位是否为1
等待忙状态结束
*/
void W25Q64_WaitBusy(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while( ( MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01 )  == 0x01 );//读取状态寄存器的最后一位BUSY位

	MySPI_Stop();

}


/*
	向从机写入多个字节的数据
1,写使能 2，开始时序；3，发送页编程的指令,发送3个字节的地址 
4，发送字节(通过数组发送多个字节); 5,停止时序 ; 6,等待忙状态结束
*/
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{	
	uint16_t i;
	W25Q64_WriteEnable();		
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);//地址的首地址
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address );	//地址的尾地址
	
	for(i = 0; i<Count;i++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

/*
	擦除指定的扇区
 与写数据一样需要先写使能 1.开始 ； 2，发送擦除指令； 3，发送地址；4，结束 ；5,等待忙
*/
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // 擦除前必须写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();    // 核心重点：必须等待擦除完成！如果没有这一句，紧接着的 PageProgram 会失效导致读出0xFF
}

/*
	制定地址读
1.开始 ； 2，读指令 ；3，发送地址 3，接收字节 ；4，结束

*/
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint16_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	
	MySPI_SwapByte(Address >> 16);//地址的首地址
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address );	//地址的尾地址
	
	for(i =0; i<Count ; i++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}







































