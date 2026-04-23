#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint16_t ArrrayWrit[]= {0x1234,0x5678};
uint16_t ArrrayRead[2];

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Key_Init();
	
	/*
	需要开启PWR的时钟后才能使能对BKP和RTC的访问
	1. 开启PWR和BKP时钟
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	/*
	2. 使用PWR使能BKP和RTC的访问
	*/
	PWR_BackupAccessCmd(ENABLE);
	
	uint8_t CheckKey;
	
	while (1)
	{
		CheckKey = Key_GetNum();
		if(CheckKey == 1)
		{
			//BKP后备寄存器有DR42个空间,84字节
			BKP_WriteBackupRegister(BKP_DR1,ArrrayWrit[0]);
			BKP_WriteBackupRegister(BKP_DR2,ArrrayWrit[1]);
			
			ArrrayWrit[0]++;
			ArrrayWrit[1]++;
		}
		
		ArrrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		ArrrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(1,1,ArrrayRead[0],4);
		OLED_ShowHexNum(2,1,ArrrayRead[1],4);
	}
}
