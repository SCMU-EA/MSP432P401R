/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 15:22:15
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-25 17:29:08
 * @Description  : 串口的基本使用
 */
#include "main.h"

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

const eUSCI_UART_Config uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,   // 时钟源为SMCLK
    312,                              // BRDIV = 312 (SMCLK = 48MHz, 波特率为9600)
    0,                                // UCxBRF = 0
    0,                                // UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,           // 无校验
    EUSCI_A_UART_LSB_FIRST,           // 低位在前
    EUSCI_A_UART_ONE_STOP_BIT,        // 1位停止位
    EUSCI_A_UART_MODE,                // UART模式
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // 过采样
};

int main(void)
{
    SystemClockInit(HFXT); // 系统时钟初始化
    RtcInit();             // 初始化RTC实时时钟

    volatile uint32_t i = 0;

    // 设置P3.2 P3.3为串口模式
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); 

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48); // 设置DCO频率为48MHz

    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig); // 初始化串口模块

    MAP_UART_enableModule(EUSCI_A2_BASE); // 使能串口模块

    while (1)
    {
        for (i = 0; i < 100000; i++) {} // 延迟一段时间

        // 发送字符串 "232"
        MAP_UART_transmitData(EUSCI_A2_BASE, '2');
        MAP_UART_transmitData(EUSCI_A2_BASE, '3');
        MAP_UART_transmitData(EUSCI_A2_BASE, '2');
		MAP_UART_transmitData(EUSCI_A2_BASE, '\r');
		MAP_UART_transmitData(EUSCI_A2_BASE, '\n');
    }
}


