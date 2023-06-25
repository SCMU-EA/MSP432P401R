#include "main.h"

/*  全局变量申明区  */

/* 函数调用申明区 */

int main(void)
{
  SystemClockInit(HFXT); // 系统时钟初始化
  RtcInit();             // 初始化RTC实时时钟

  volatile uint32_t i = 0;

  MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                  // 设置P1.0为输出
  MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // 设置P1.1为输入，带上拉电阻

  MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1); // 使能SRAM Bank1保持功能

  while (1)
  {
    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW) // 如果P1.1为低电平
    {
      delay_ms(50);
      if (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW) // 如果P1.1为低电平
      {
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); // P1.0输出高电平
      }
      else
      {
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // P1.0输出低电平
      }
    }
  }
}
