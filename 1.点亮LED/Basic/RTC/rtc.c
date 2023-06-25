#include "rtc.h"

volatile RTC_C_Calendar newTime;

 const RTC_C_Calendar currentTime =
 {
     00,       //秒
     06,       //分
     20,       //时
     00,       //星期
     03,       //日
	   06,       //月
	   2018      //年
 };


unsigned int sec ;
 
void RtcInit(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,GPIO_PIN0 | GPIO_PIN1, 
	  GPIO_PRIMARY_MODULE_FUNCTION);//配置外部时钟管脚
	
    CS_setExternalClockSourceFrequency(32768,48000000);//设置外部时钟源频率

    /* Starting LFXT in non-bypass mode without a timeout. */
    CS_startLFXT(false);

    RTC_C_initCalendar(&currentTime,RTC_C_FORMAT_BINARY );//存取时间，二进制格式

	/*闹钟时间：12:40 */
    RTC_C_setCalendarAlarm(40, 12, RTC_C_ALARMCONDITION_OFF, RTC_C_ALARMCONDITION_OFF);//设置闹钟

    /* Specify an interrupt to assert every minute */
    RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);//指定中断每分钟中断一次

    /* Enable interrupt for RTC Ready Status, which asserts when the RTC
     * Calendar registers are ready to read.
     * Also, enable interrupts for the Calendar alarm and Calendar event. */
    RTC_C_clearInterruptFlag(RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT| RTC_C_CLOCK_ALARM_INTERRUPT);//选择产生中断的条件
    RTC_C_enableInterrupt( RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT | RTC_C_CLOCK_ALARM_INTERRUPT);

    /* Start RTC Clock */
    RTC_C_startClock();  //开启RTC时钟
    Interrupt_enableInterrupt(INT_RTC_C);//使能RTC中断
    Interrupt_enableMaster();//使能中断  

 }

/* RTC ISR */
void RTC_C_IRQHandler(void) //RTC中断函数
{
    uint32_t status;

    status = RTC_C_getEnabledInterruptStatus();//获取RTC中断的状态
    RTC_C_clearInterruptFlag(status);          //清除中断状态

    if (status & RTC_C_CLOCK_READ_READY_INTERRUPT)
    {
        newTime = RTC_C_getCalendarTime();    //获取时间
    }

    if (status & RTC_C_TIME_EVENT_INTERRUPT)  //每分钟触发的事件
    {
      
			
			
    }

    if (status & RTC_C_CLOCK_ALARM_INTERRUPT)  //到达闹钟时候 触发的事件
    {
      
		
			
    }

}

 
 
