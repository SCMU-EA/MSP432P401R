#include "main.h"

/*  全局变量申明区  */

/* 函数调用申明区 */

int main(void)
{
  SystemClockInit(HFXT); // 系统时钟初始化
  RtcInit();             // 初始化RTC实时时钟

  volatile uint32_t i = 0;

  MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // 设置P1.0为输出

  while (1)
  {

    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); // 翻转P1.0
	delay_ms(200);

  }
}
