/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 20:46:33
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-26 11:01:19
 * @Description  : 封装串口库，有一说一·，这个库真的没啥用，就是把串口初始化和发送函数封装了一下，方便调用，还有串口0、1我根本没在板卡上找到这几个引脚 = =
 */
#include "usart.h"
#include "string.h"

/********************************************/
//波特率计算公式：
// N=时钟主频/波特率
// 如果 N<16  UCBRx=INT(N),UCBRF=0;
// 如果 N>16  UCBRx=INT(N/16),UCBRF=INT{[(N/16)-INT(N/16)]*16}
// UCBRS 取值见MSP432 USER'S GUIDE P731
// 或者 使用计算器 
// software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
/********************************************/

/* 我一般设OSD频率是48M，要是有要求功耗的话才降低频率
  * 一般就直接使用这个计算器网页了
  */

uint8_t 	aRxBuffer;						//接收中断缓冲
uint8_t 	aTxBuffer[200];		            //发送区缓冲
uint32_t    getCharNum=0;			        //获取字符计数值

const eUSCI_UART_Config uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,   // 时钟源为SMCLK
    26,                              // BRDIV = 312 (SMCLK = 48MHz, 波特率为9600)
    0,                                // UCxBRF = 0
    111,                             // UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,           // 无校验
    EUSCI_A_UART_LSB_FIRST,           // 低位在前
    EUSCI_A_UART_ONE_STOP_BIT,        // 1位停止位
    EUSCI_A_UART_MODE,                // UART模式
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // 过采样
};

//////////////////////////////////////////////////////
/* 串口0 */
/**
 * @name: void Usart0_Init(void)
 * @description: 串口0初始化
 * @return {*}
 */
void Usart0_Init(void)
{
    // 设置P1.2 P1.3为串口模式
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); 

    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig); // 初始化串口模块

    MAP_UART_enableModule(EUSCI_A0_BASE); // 使能串口模块
}

/**
 * @name: void Usart0_Init_It(void)
 * @description: 串口0初始化(中断)
 * @return {*}
 */
void Usart0_Init_It(void)
{
    Usart0_Init();

    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster(); 
}

/**
 * @name: void Usart0_SendByte(uint8_t data)
 * @description: 串口0发送单个字节
 * @param {uint8_t} data 所发送的字节
 * @return {*}
 */
void Usart0_SendByte(uint8_t data)
{
    MAP_UART_transmitData(EUSCI_A0_BASE, data); // 发送一个字节
}

/**
 * @name: void Usart0_SendString(uint8_t *data, uint16_t len)
 * @description: 串口0发送字符串
 * @param {uint8_t} *data 发送的字符串
 * @param {uint16_t} len 发送的字符串长度
 * @return {*}
 */
void Usart0_SendString(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        MAP_UART_transmitData(EUSCI_A0_BASE, data[i]); // 发送一个字节
    }
}

/**
 * @name: void EUSCIA0_IRQHandler(void)
 * @description: 串口0的中断服务函数
 * @return {*}
 */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        // 将中断执行的内容写在这里
        //Usart1_SendString("EUSCIA0_IRQHandler\r\n", strlen("EUSCIA0_IRQHandler\r\n"));
    }
}
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
/* 串口1 */

/**
 * @name: void Usart1_Init(void)
 * @description: 串口1初始化
 * @return {*}
 */
void Usart1_Init(void)
{
    // 设置P2.2 P2.3为串口模式
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); 

    MAP_UART_initModule(EUSCI_A1_BASE, &uartConfig); // 初始化串口模块

    MAP_UART_enableModule(EUSCI_A1_BASE); // 使能串口模块
}

/**
 * @name: void Usart1_Init_It(void)
 * @description: 串口1初始化(中断)
 * @return {*}
 */
void Usart1_Init_It(void)
{
    Usart1_Init();

    MAP_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA1);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster(); 
}

/**
 * @name: void Usart1_SendByte(uint8_t data)
 * @description: 串口1发送单个字节
 * @param {uint8_t} data 所发送的字节
 * @return {*}
 */
void Usart1_SendByte(uint8_t data)
{
    MAP_UART_transmitData(EUSCI_A1_BASE, data); // 发送一个字节
}

/**
 * @name: void Usart1_SendString(uint8_t *data, uint16_t len)
 * @description: 串口1发送字符串
 * @param {uint8_t} *data 发送的字符串
 * @param {uint16_t} len 发送的字符串长度
 * @return {*}
 */
void Usart1_SendString(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        MAP_UART_transmitData(EUSCI_A1_BASE, data[i]); // 发送一个字节
    }
}

/**
 * @name: void EUSCIA1_IRQHandler(void)
 * @description: 串口1的中断服务函数
 */
void EUSCIA1_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A1_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        
    }
}
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
/* 串口2 */

/**
 * @name: void Usart2_Init(void)
 * @description: 串口2初始化
 * @return {*}
 */
void Usart2_Init(void)
{
    // 设置P3.2 P3.3为串口模式
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); 

    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig); // 初始化串口模块

    MAP_UART_enableModule(EUSCI_A2_BASE); // 使能串口模块
}

/**
 * @name: void Usart2_Init_It(void)
 * @description: 串口2初始化(中断)
 * @return {*}
 */
void Usart2_Init_It(void)
{
    Usart2_Init();

    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster(); 
}

/**
 * @name: void Usart2_SendByte(uint8_t data)
 * @description: 串口2发送单个字节
 * @param {uint8_t} data 所发送的字节
 * @return {*}
 */
void Usart2_SendByte(uint8_t data)
{
    MAP_UART_transmitData(EUSCI_A2_BASE, data); // 发送一个字节
}

/**
 * @name: void Usart2_SendString(uint8_t *data, uint16_t len)
 * @description: 串口2发送字符串
 * @param {uint8_t} *data 发送的字符串
 * @param {uint16_t} len 发送的字符串长度
 * @return {*}
 */
void Usart2_SendString(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        MAP_UART_transmitData(EUSCI_A2_BASE, data[i]); // 发送一个字节
    }
}

/**
 * @name: void EUSCIA2_IRQHandler(void)
 * @description: 串口2的中断服务函数
 * @return {*}
 */
// void EUSCIA2_IRQHandler(void)
// {
//     uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

//     MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

//     if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//     {
//         MAP_UART_transmitData(EUSCI_A2_BASE, MAP_UART_receiveData(EUSCI_A2_BASE));
//         // 将中断执行的内容写在这里
//     }
// }
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
/* 串口3 */

/**
 * @name: void Usart3_Init(void)
 * @description: 串口3初始化
 * @return {*}
 */
void Usart3_Init(void)
{
    // 设置P9.6 P9.7为串口模式
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); 

    MAP_UART_initModule(EUSCI_A3_BASE, &uartConfig); // 初始化串口模块

    MAP_UART_enableModule(EUSCI_A3_BASE); // 使能串口模块
}

/**
 * @name: void Usart3_Init_It(void)
 * @description: 串口3初始化(中断)
 * @return {*}
 */
void Usart3_Init_It(void)
{
    Usart3_Init();

    MAP_UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA3);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster(); 
}

/**
 * @name: void Usart3_SendByte(uint8_t data)
 * @description: 串口3发送单个字节
 * @param {uint8_t} data 所发送的字节
 * @return {*}
 */
void Usart3_SendByte(uint8_t data)
{
    MAP_UART_transmitData(EUSCI_A3_BASE, data); // 发送一个字节
}

/**
 * @name: void Usart3_SendString(uint8_t *data, uint16_t len)
 * @description: 串口3发送字符串
 * @param {uint8_t} *data 发送的字符串
 * @param {uint16_t} len 发送的字符串长度
 * @return {*}
 */
void Usart3_SendString(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        MAP_UART_transmitData(EUSCI_A3_BASE, data[i]); // 发送一个字节
    }
}

/**
 * @name: void EUSCIA2_IRQHandler(void)
 * @description: 串口2的中断服务函数
 * @return {*}
 */
void EUSCIA3_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A3_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        // 将中断执行的内容写在这里
    }
}
//////////////////////////////////////////////////////
