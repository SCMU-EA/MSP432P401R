/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 20:46:33
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-25 20:52:55
 * @Description  : 封装串口库，有一说一·，这个库真的没啥用，就是把串口初始化和发送函数封装了一下，方便调用，还有串口0、1我根本没在板卡上找到这几个引脚 = =
 */
#ifndef _USART_H
#define _USART_H

#include "main.h"

/* MSP432P401R 串口0 :
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *            |                 |
 *       RST -|     P1.3/UCA0TXD|----> PC (echo)
 *            |                 |
 *            |                 |
 *            |     P1.2/UCA0RXD|<---- PC
 *            |                 |
 *
 */
void Usart0_Init(void);                             // 串口初始化
void Usart0_Init_It(void);                          // 串口初始化(中断
void Usart0_SendByte(uint8_t data);                 // 发送一个字节
void Usart0_SendString(uint8_t *data, uint16_t len);              // 发送字符串

/* MSP432P401R 串口1 :
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *            |                 |
 *       RST -|     P2.3/UCA0TXD|----> PC (echo)
 *            |                 |
 *            |                 |
 *            |     P2.2/UCA0RXD|<---- PC
 *            |                 |
 *
 */
void Usart1_Init(void);                             // 串口初始化
void Usart1_Init_It(void);                          // 串口初始化(中断
void Usart1_SendByte(uint8_t data);                 // 发送一个字节
void Usart1_SendString(uint8_t *data, uint16_t len);              // 发送字符串

/* MSP432P401R 串口2 :
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
void Usart2_Init(void);                             // 串口初始化
void Usart2_Init_It(void);                          // 串口初始化(中断
void Usart2_SendByte(uint8_t data);                 // 发送一个字节
void Usart2_SendString(uint8_t *data, uint16_t len);              // 发送字符串

/* MSP432P401R 串口3 :
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *            |                 |
 *       RST -|     P9.7/UCA0TXD|----> PC (echo)
 *            |                 |
 *            |                 |
 *            |     P9.6/UCA0RXD|<---- PC
 *            |                 |
 *
 */
void Usart3_Init(void);                             // 串口初始化
void Usart3_Init_It(void);                          // 串口初始化(中断
void Usart3_SendByte(uint8_t data);                 // 发送一个字节
void Usart3_SendString(uint8_t *data, uint16_t len);              // 发送字符串



#endif
