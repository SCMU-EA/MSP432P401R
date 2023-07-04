/*
 * @Author       : RoxyKko
 * @Date         : 2023-06-27 10:21:02
 * @LastEditors  : RoxyKko
 * @LastEditTime : 2023-07-04 14:09:45
 * @Description  : MSP432P401R定时器A头文件
 */

#include "timA.h"

/*
PWM输出的通道的选择：                  TA0            TA1              TA2             TA3
TIMER_A_CAPTURECOMPARE_REGISTER_0  P7.3/PM_TA0.0  *P8.0/TA1.0       *P8.1/TA2.0    *P10.4/TA3.0
TIMER_A_CAPTURECOMPARE_REGISTER_1  P2.4/PM_TA0.1  P7.7/PM_TA1.1     *P5.6/TA2.1    *P10.5/TA3.1
TIMER_A_CAPTURECOMPARE_REGISTER_2  P2.5/PM_TA0.2  P7.6/PM_TA1.2     *P5.7/TA2.2    *P8.2/TA3.2
TIMER_A_CAPTURECOMPARE_REGISTER_3  P2.6/PM_TA0.3  P7.5/PM_TA1.3     *P6.6/TA2.3    *P9.2/TA3.3
TIMER_A_CAPTURECOMPARE_REGISTER_4  P2.7/PM_TA0.4  P7.4/PM_TA1.4     *P6.7/TA2.4    *P9.3/TA3.4
TIMER_A_CAPTURECOMPARE_REGISTER_5
TIMER_A_CAPTURECOMPARE_REGISTER_6

 带有*号的不能输出PWM波
*/

/**
 * @name: void TimA0_Init_IT(uint16_t ccr0, uint16_t psc)
 * @description: 普通定时器初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA0_Init_IT(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_UpModeConfig upConfig = {0};    // 定义增计数模式结构体
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    upConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    upConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;    // 禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;    // 启用 ccr0更新中断 
    upConfig.timerClear = TIMER_A_DO_CLEAR;    // 清除计数器

    // 2.初始化定时器
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);

    // 3.启动定时器
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    
    // 5.开启中断
    MAP_Interrupt_enableInterrupt(INT_TA0_0);
}

/**
 * @name: void TimA0_Init_PWM(uint16_t ccr0, uint16_t psc)
 * @description: 定时器PWM模式初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA0_Init_PWM(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_PWMConfig pwmConfig = {0};    // 定义增计数模式结构体
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    pwmConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    pwmConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
    pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;    // 输出模式选择PWM模式
    pwmConfig.dutyCycle = ccr0/2;    // 占空比设置

    // 2.初始化定时器
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // 3.设置PWM输出引脚
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);    // P2.4/PM_TA0.1

    // 4.启动定时器
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

}

///////////////////// 检修中 ///////////////////////

// /**
//  * @name: void TimA0_Init_Capture(uint16_t ccr0, uint16_t psc)
//  * @description: 定时器输入捕获模式初始化函数
//  * @param {uint16_t} ccr0 自动重装载值
//  * @param {uint16_t} psc  时钟分频系数
//  * @return {*}
//  */
// void TimA0_Init_Capture(uint16_t ccr0, uint16_t psc)
// {
//     // 1.增计数模式初始化
//     Timer_A_initCaptureModeParam captureConfig = {0};    // 定义增计数模式结构体
//     captureConfig.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
//     captureConfig.captureMode = TIMER_A_CAPTUREMODE_RISING_EDGE;    // 捕获模式选择上升沿捕获
//     captureConfig.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;    // 捕获输入选择CCIxA
//     captureConfig.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;    // 捕获源同步
//     captureConfig.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;    // 使能捕获中断
//     captureConfig.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;    // 捕获输出模式
//     captureConfig.channelCaptureMode = TIMER_A_CAPTURECOMPARE_MODE;    // 捕获模式

//     // 2.初始化定时器
//     Timer_A_initCaptureMode(TIMER_A0_BASE, &captureConfig);

//     // 3.设置捕获输入引脚
//     GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);    // P2.4/PM_TA0.1

//     // 4.设置捕获中断优先级
//     Interrupt_setPriority(INT_TA0_0, 0x20);    // 设置中断优先级为0x20

//     // 5.使能捕获中断
//     Interrupt_enableInterrupt(INT_TA0_0);

//     // 6.使能总中断
//     Interrupt_enableMaster();

//     // 7.启动定时器
//     Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

// }

// /**
//  * @name: void TimA0_Init_Compare(uint16_t ccr0, uint16_t psc)
//  * @description: 定时器输出比较模式初始化函数
//  * @param {uint16_t} ccr0 自动重装载值
//  * @param {uint16_t} psc  时钟分频系数
//  * @return {*}
//  */
// void TimA0_Init_Compare(uint16_t ccr0, uint16_t psc)
// {
//     // 1.增计数模式初始化
//     Timer_A_CompareModeConfig compareConfig = {0};    // 定义输出比较模式结构体
//     compareConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
//     compareConfig.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;    // 禁用比较中断
//     compareConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;    // 输出模式选择比较模式（翻转和设置）
//     compareConfig.compareValue = ccr0;    // 比较值

//     // 2.初始化定时器
//     Timer_A_initCompareMode(TIMER_A0_BASE, &compareConfig);

//     // 3.设置比较输出引脚
//     GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);    // P2.4/PM_TA0.1

//     // 4.启动定时器
//     Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
// }

///////////////////// 检修中 ///////////////////////

/**
 * @name: void TA0_0_IRQHandler(void)
 * @description: 定时器A0中断服务函数
 * @return {*}
 */
void TA0_0_IRQHandler(void)
{
    // 清除中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // 处理定时器A0中断事件
    // TODO: 在这里添加你的代码
    // ...
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    
}

/**
 * @name: void TimA1_Init_IT(uint16_t ccr0, uint16_t psc)
 * @description: 普通定时器初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA1_Init_IT(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_UpModeConfig upConfig = {0};    // 定义增计数模式结构体
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    upConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    upConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;    // 禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;    // 启用 ccr0更新中断 
    upConfig.timerClear = TIMER_A_DO_CLEAR;    // 清除计数器

    // 2.初始化定时器
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    // 3.启动定时器
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    
    // 5.开启中断
    MAP_Interrupt_enableInterrupt(INT_TA1_0);   // 测试用
}

/**
 * @name: void TimA1_Init_PWM(uint16_t ccr0, uint16_t psc)
 * @description: 定时器PWM模式初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA1_Init_PWM(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_PWMConfig pwmConfig = {0};    // 定义增计数模式结构体
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    pwmConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    pwmConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
    pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;    // 输出模式选择PWM模式
    pwmConfig.dutyCycle = ccr0/2;    // 占空比设置

    // 2.初始化定时器
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwmConfig);

    // 3.设置PWM输出引脚
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);    // P7.7/PM_TA1.1

    // 4.启动定时器
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

}

/**
 * @name: void TA1_0_IRQHandler(void)
 * @description: 定时器A1中断服务函数
 * @return {*}
 */
void TA1_0_IRQHandler(void)
{
    // 清除中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // 处理定时器A1中断事件

    // TODO: 在这里添加你的代码
    // ...
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);    // 测试用
}

/**
 * @name: void TimA2_Init_IT(uint16_t ccr0, uint16_t psc)
 * @description: 普通定时器初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA2_Init_IT(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_UpModeConfig upConfig = {0};    // 定义增计数模式结构体
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    upConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    upConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;    // 禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;    // 启用 ccr0更新中断
    upConfig.timerClear = TIMER_A_DO_CLEAR;    // 清除计数器

    // 2.初始化定时器
    MAP_Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);

    // 3.启动定时器
    MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // 5.开启中断
    MAP_Interrupt_enableInterrupt(INT_TA2_0);
}

/**
 * @name: void TimA1_Init_PWM(uint16_t ccr0, uint16_t psc)
 * @description: 定时器PWM模式初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA2_Init_PWM(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_PWMConfig pwmConfig = {0};    // 定义增计数模式结构体
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    pwmConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    pwmConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
    pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;    // 输出模式选择PWM模式
    pwmConfig.dutyCycle = ccr0/2;    // 占空比设置

    // 2.初始化定时器
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwmConfig);

    // 3.设置PWM输出引脚
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);    // P5.6/PM_TA2.1

    // 4.启动定时器
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

}

/**
 * @name: void TA2_0_IRQHandler(void)
 * @description: 定时器A2中断服务函数
 * @return {*}
 */
void TA2_0_IRQHandler(void)
{
    // 清除中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // 处理定时器A2中断事件

    // TODO: 在这里添加你的代码
    // ...
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); // 测试用
}

/**
 * @name: void TimA3_Init_IT(uint16_t ccr0, uint16_t psc)
 * @description: 普通定时器初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA3_Init_IT(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_UpModeConfig upConfig = {0};    // 定义增计数模式结构体
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    upConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    upConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;    // 禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;    // 启用 ccr0更新中断
    upConfig.timerClear = TIMER_A_DO_CLEAR;    // 清除计数器

    // 2.初始化定时器
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);

    // 3.启动定时器
    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // 5.开启中断
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
}

/**
 * @name: void TimA1_Init_PWM(uint16_t ccr0, uint16_t psc)
 * @description: 定时器PWM模式初始化函数
 * @param {uint16_t} ccr0 自动重装载值
 * @param {uint16_t} psc  时钟分频系数
 * @return {*}
 */
void TimA3_Init_PWM(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_PWMConfig pwmConfig = {0};    // 定义增计数模式结构体
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 时钟源选择SMCLK
    pwmConfig.clockSourceDivider = psc;      // 时钟分频系数1 范围1-64
    pwmConfig.timerPeriod = ccr0;            // 自动重装载值（ARR）
    pwmConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // 比较寄存器选择CCR1
    pwmConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;    // 输出模式选择PWM模式
    pwmConfig.dutyCycle = ccr0/2;    // 占空比设置

    // 2.初始化定时器
    MAP_Timer_A_generatePWM(TIMER_A3_BASE, &pwmConfig);

    // 3.设置PWM输出引脚
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);    // P6.6/PM_TA3.1

    // 4.启动定时器
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
}

/**
 * @name: void TA3_0_IRQHandler(void)
 * @description: 定时器A3中断服务函数
 * @return {*}
 */
void TA3_0_IRQHandler(void)
{
    // 清除中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // 处理定时器A3中断事件

    // TODO: 在这里添加你的代码
    // ...
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); // 测试用
}
