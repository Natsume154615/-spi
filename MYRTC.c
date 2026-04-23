#include "stm32f10x.h"                  // Device header
#include "time.h"

uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 30, 30};
 uint16_t Read_Time[6];
void MyRTC_SetTime(void);

/*
	1. 开启PWR和BKP时钟
2. 使用PWR使能BKP和RTC的访问（PWR（电源控制外设 Power Control）而BKP和RTC都在后备区域）
3.开启LSE时钟并等待标志位
4.选择RTCCLK时钟为LSE，并使能
5. 调用等待同步函数（PCLK和RTCCLK的时钟不一致，PCLK大于RTCCLK，所以要等一下）
6. 写入后需等待RTC_CR寄存器的RTOFF位为1时才可再次写入（调用等待函数），因为PCLK和RTCCLK的时钟不一致所以需要时间调整后写入
7.设置分频器并等待写入完成
8.设置时间
*/
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);

	PWR_BackupAccessCmd(ENABLE);
	
	
	if(BKP_ReadBackupRegister( BKP_DR1 ) != 0xA5A5 )
	{
	RCC_LSEConfig(RCC_LSE_ON);
	while( RCC_GetFlagStatus(RCC_FLAG_LSERDY) !=SET);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	
	RTC_SetPrescaler(32768 - 1);
	RTC_WaitForLastTask();
	
	MyRTC_SetTime();
	
	BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else{
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt; 	
	struct tm time_date={0};
	
	time_date.tm_year=MyRTC_Time[0] -1900 ;
	time_date.tm_mon = MyRTC_Time[1] -1; //内部月份为0-11表示
	time_date.tm_mday=MyRTC_Time[2];
	time_date.tm_hour=MyRTC_Time[3];
	time_date.tm_min=MyRTC_Time[4];
	time_date.tm_sec =MyRTC_Time[5];
	
	time_cnt=mktime(&time_date) ;
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();

}


void MyRTC_ReadTime(void)
{
	time_t time_cnt; 	
	struct tm time_date={0};
	
	time_cnt = RTC_GetCounter()  ;
	
	time_date =*localtime(&time_cnt) ;

	Read_Time[0] =  time_date.tm_year+1900 ;
	Read_Time[1] = time_date.tm_mon + 1 ; //内部月份为0-11表示
	Read_Time[2] = time_date.tm_mday;
	Read_Time[3] = time_date.tm_hour;
	Read_Time[4] = time_date.tm_min;
	Read_Time[5] = time_date.tm_sec;
	
}



