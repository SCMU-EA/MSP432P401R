/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-25 15:08:33
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-06-25 15:20:01
 * @Description  : MSP432P401R 设置按键中断
 */
#include "main.h"

/*  全局变量申明区  */

/* 函数调用申明区 */

/* MSP432P401R 设置按键中断方法 :
  * 1. 设置P1.1为输入，带上拉电阻
  * 2. 清除P1.1的中断标志位
  * 3. 使能P1.1的中断
  * 4. 使能P1的中断
  * 5. 在中断服务函数中，判断P1.1是否为低电平，如果是，执行相应的操作
  * 6. 在main函数中，使能总中断
  */


int main(void)
{
  SystemClockInit(HFXT); // 系统时钟初始化
  RtcInit();             // 初始化RTC实时时钟

  volatile uint32_t i = 0;

  MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                  // 设置P1.0为输出

  MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // 设置P1.1为输入，带上拉电阻
  MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);              // 清除P1.1的中断标志位
  MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);                 // 使能P1.1的中断
  MAP_Interrupt_enableInterrupt(INT_PORT1);                           // 使能P1的中断

  MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1); // 使能SRAM Bank1保持功能

  MAP_Interrupt_enableMaster(); // 使能总中断

  while (1)
  {
    MAP_PCM_gotoLPM0(); // 进入低功耗模式0
  }
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
  uint32_t status;

  status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1); // 获取P1的中断状态
  MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);         // 清除P1的中断标志位

  /* 翻转输出引脚LED的电平 */
  // 这里status & GPIO_PIN1的意思是，如果P1.1引脚的中断标志位为1，那么就执行下面的操作
  // 因为MSP432的判断引脚高低电平比较奇怪，如果引脚寄存器与自己的引脚号相同，那么就是高电平，否则就是低电平
  // 这是因为MSP432的GPIO寄存器是32位的，而引脚号只有5位，所以如果引脚寄存器与引脚号相同，那么就是高电平，MSP432对GPIO的操作不是对单个位进行操作的，他是对一个寄存器组进行操作的
  // 所以这里用status & GPIO_PIN1来判断P1.1引脚的中断标志位是否为1
  if(status & GPIO_PIN1)
  {
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
  }

}
