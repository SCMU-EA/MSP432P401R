/*** 
 * @Author       : RoxyKko
 * @Date         : 2023-06-27 10:20:54
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-07-03 12:46:01
 * @Description  : MSP432P401R定时器A头文件
 */
#ifndef __TIMA_H__
#define __TIMA_H__

#include "main.h"

/* 定时器初始化（中断模式）函数 */
void TimA0_Init_IT(uint16_t ccr0, uint16_t psc);
void TimA1_Init_IT(uint16_t ccr0, uint16_t psc);
void TimA2_Init_IT(uint16_t ccr0, uint16_t psc);
void TimA3_Init_IT(uint16_t ccr0, uint16_t psc);

/* 定时器初始化（PWM模式）函数 */
void TimA0_Init_PWM(uint16_t ccr0, uint16_t psc);
void TimA1_Init_PWM(uint16_t ccr0, uint16_t psc);
void TimA2_Init_PWM(uint16_t ccr0, uint16_t psc);
void TimA3_Init_PWM(uint16_t ccr0, uint16_t psc);

/* 定时器初始化（输入捕获模式）函数 */
void TimA0_Init_Capture(uint16_t ccr0, uint16_t psc);
void TimA1_Init_Capture(uint16_t ccr0, uint16_t psc);
void TimA2_Init_Capture(uint16_t ccr0, uint16_t psc);
void TimA3_Init_Capture(uint16_t ccr0, uint16_t psc);

/* 定时器初始化（输出比较模式）函数 */
void TimA0_Init_Compare(uint16_t ccr0, uint16_t psc);
void TimA1_Init_Compare(uint16_t ccr0, uint16_t psc);
void TimA2_Init_Compare(uint16_t ccr0, uint16_t psc);
void TimA3_Init_Compare(uint16_t ccr0, uint16_t psc);

#endif
