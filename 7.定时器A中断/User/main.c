/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 20:39:13
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-26 10:50:54
 * @Description  : 串口中断，并将串口所用的库封装，库位于工程目录下Basic\Usart下
 */
#include "main.h"
#include "string.h"

/*  全局变量申明区  */

/* 函数调用申明区 */

/* MSP432P401R 串口 :
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *            |                 |
 *       RST -|     P3.3/UCA0TXD|----> PC (echo)
 *            |                 |
 *            |                 |
 *            |     P3.2/UCA0RXD|<---- PC
 *            |                 |
 *
 */

int main(void)
{
    SysInit(); 		// 系统时钟初始化
	LED_Init();		// 
	TimA0_Init_IT(37499,64);	//定时器A 0中断开始
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); // 设置DCO频率为48MHz
	
    TFT_Init();   // 初始化串口2，波特率115200
	
	MAP_Interrupt_enableMaster(); // 开启总中断
    while (1)
    {
        
//        Usart2_SendString("222\r\n", strlen("222\r\n"));
//        delay_ms(50);
    }
}

