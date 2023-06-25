/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 20:39:13
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-25 22:00:49
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
    SystemClockInit(HFXT); // 系统时钟初始化
    RtcInit();             // 初始化RTC实时时钟

    volatile uint32_t i = 0;

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); // 设置DCO频率为48MHz

    Usart2_Init_It();   // 初始化串口2，波特率115200

    while (1)
    {
        
//        Usart2_SendString("222\r\n", strlen("222\r\n"));
//        delay_ms(50);
    }
}

