/**********************************************************************************************************
                                天穹飞控 —— 致力于打造中国最好的多旋翼开源飞控
                                Github: github.com/loveuav/BlueSkyFlightControl
                                技术讨论：bbs.loveuav.com/forum-68-1.html
 * @文件     logger.c
 * @说明     飞行日志
 * @版本  	 V1.0
 * @作者     BlueSky
 * @网站     bbs.loveuav.com
 * @日期     2018.07
**********************************************************************************************************/
#include "logger.h"

#include "ff.h"		
#include "diskio.h"	
#include <stdio.h>

FATFS fs;
FIL   file;

uint8_t cardExistFlag = 0;

void LoggerTest(void);

/**********************************************************************************************************
*函 数 名: LoggerInit
*功能说明: 飞行日志初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void LoggerInit(void)
{
    FRESULT fresult;
    TCHAR testName[] = "0:initTest";   

    //挂载文件系统
    f_mount(0, &fs);
    
    //打开文件
	fresult = f_open(&file, testName, FA_OPEN_ALWAYS | FA_WRITE);

    if(fresult == FR_OK)
    {
        cardExistFlag = 1;
        //关闭文件
        fresult = f_close(&file);
        //删除测试文件
        f_unlink(testName);
    }
    else
    {
        cardExistFlag = 0;
    }
}

/**********************************************************************************************************
*函 数 名: LoggerLoop
*功能说明: 日志记录主循环
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void LoggerLoop(void)
{  
    if(!cardExistFlag)
        return;
    
   LoggerTest();
}

void LoggerTest(void)
{
    /*TF卡写入测试*/
    static uint32_t count = 300000;
    static FRESULT fresult;
    TCHAR fileName[] = "0:bluesky.txt";   
    static uint8_t testInit = 0;
    
    if(testInit == 0)
    {
        fresult = f_open(&file, fileName, FA_OPEN_ALWAYS | FA_WRITE);
        testInit = 1;
    }

    if(fresult == FR_OK && count > 0)
    {
        //写入数据
        f_printf(&file,"Welcome to use the BlueSky FlightControl %d!\n", count--);       
        
        //刷新缓存
        if(count % 500 == 0)
        {                      
            f_sync(&file);
        }
        
        //关闭文件
        if(count == 0)
        {
            fresult = f_close(&file);
        }
    }
}


