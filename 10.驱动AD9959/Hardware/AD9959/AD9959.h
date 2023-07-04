#ifndef _AD9959_H_
#define _AD9959_H_
#include "main.h"
#include "stdint.h"

#define MHz 1000000
#define kHz 1000*0.9987013790118112670830499041494
//误差比例0.9986705

//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器
#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器
#define LSRR_ADD  0x07   //LSR 通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW 通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW 通道线性向下扫描寄存器\

/*gpiO引脚设置*/
#define SD2_Pin GPIO_PIN6
#define SD2_GPIO_Port GPIO_PORT_P9

#define SD1_Pin GPIO_PIN0
#define SD1_GPIO_Port GPIO_PORT_P8

#define SD0_Pin GPIO_PIN4
#define SD0_GPIO_Port GPIO_PORT_P7

#define PS3_Pin GPIO_PIN6
#define PS3_GPIO_Port GPIO_PORT_P7

#define PS2_Pin GPIO_PIN0
#define PS2_GPIO_Port GPIO_PORT_P10

#define PS1_Pin GPIO_PIN2
#define PS1_GPIO_Port GPIO_PORT_P10

#define PS0_Pin GPIO_PIN4
#define PS0_GPIO_Port GPIO_PORT_P10

#define SD3_Pin GPIO_PIN7
#define SD3_GPIO_Port GPIO_PORT_P9

#define SCK_Pin GPIO_PIN5
#define SCK_GPIO_Port GPIO_PORT_P7

#define CS_Pin GPIO_PIN7
#define CS_GPIO_Port GPIO_PORT_P7

#define UP_Pin GPIO_PIN1
#define UP_GPIO_Port GPIO_PORT_P10

#define RST_Pin GPIO_PIN3
#define RST_GPIO_Port GPIO_PORT_P10

#define PDC_Pin GPIO_PIN5
#define PDC_GPIO_Port GPIO_PORT_P10


//AD9959管脚宏定义
////////////////////////////////////////////////////////////////////////////////////////

#define AD9959_PWR_H()  MAP_GPIO_setOutputHighOnPin(PDC_GPIO_Port, PDC_Pin)
#define AD9959_PWR_L()  MAP_GPIO_setOutputLowOnPin(PDC_GPIO_Port, PDC_Pin)

#define Reset_H()       MAP_GPIO_setOutputHighOnPin(RST_GPIO_Port, RST_Pin)
#define Reset_L()       MAP_GPIO_setOutputLowOnPin(RST_GPIO_Port, RST_Pin)

#define UPDATE_H()      MAP_GPIO_setOutputHighOnPin(UP_GPIO_Port, UP_Pin)
#define UPDATE_L()      MAP_GPIO_setOutputLowOnPin(UP_GPIO_Port, UP_Pin)

#define CS_H()          MAP_GPIO_setOutputHighOnPin(CS_GPIO_Port, CS_Pin)
#define CS_L()          MAP_GPIO_setOutputLowOnPin(CS_GPIO_Port, CS_Pin)

#define SCLK_H()        MAP_GPIO_setOutputHighOnPin(SCK_GPIO_Port, SCK_Pin)
#define SCLK_L()        MAP_GPIO_setOutputLowOnPin(SCK_GPIO_Port, SCK_Pin)

#define SDIO3_H()       MAP_GPIO_setOutputHighOnPin(SD3_GPIO_Port, SD3_Pin)
#define SDIO3_L()       MAP_GPIO_setOutputLowOnPin(SD3_GPIO_Port, SD3_Pin)

////////////////////////////////////////////////////////////////////////////////////////

#define SDIO2_H()       MAP_GPIO_setOutputHighOnPin(SD2_GPIO_Port, SD2_Pin)
#define SDIO2_L()       MAP_GPIO_setOutputLowOnPin(SD2_GPIO_Port, SD2_Pin)

#define SDIO1_H()       MAP_GPIO_setOutputHighOnPin(SD1_GPIO_Port, SD1_Pin)
#define SDIO1_L()       MAP_GPIO_setOutputLowOnPin(SD1_GPIO_Port, SD1_Pin)

#define SDIO0_H()       MAP_GPIO_setOutputHighOnPin(SD0_GPIO_Port, SD0_Pin)
#define SDIO0_L()       MAP_GPIO_setOutputLowOnPin(SD0_GPIO_Port, SD0_Pin)

#define PS3_H()         MAP_GPIO_setOutputHighOnPin(PS3_GPIO_Port, PS3_Pin)
#define PS3_L()         MAP_GPIO_setOutputLowOnPin(PS3_GPIO_Port, PS3_Pin)

#define PS2_H()         MAP_GPIO_setOutputHighOnPin(PS2_GPIO_Port, PS2_Pin)
#define PS2_L()         MAP_GPIO_setOutputLowOnPin(PS2_GPIO_Port, PS2_Pin)

#define PS1_H()         MAP_GPIO_setOutputHighOnPin(PS1_GPIO_Port, PS1_Pin)
#define PS1_L()         MAP_GPIO_setOutputLowOnPin(PS1_GPIO_Port, PS1_Pin)

#define PS0_H()         MAP_GPIO_setOutputHighOnPin(PS0_GPIO_Port, PS0_Pin)
#define PS0_L()         MAP_GPIO_setOutputLowOnPin(PS0_GPIO_Port, PS0_Pin)

////////////////////////////////////////////////////////////////////////////////////////

// #define AD9959_PWR	PBout(12)
// #define Reset		PBout(10)
// #define UPDATE		PEout(14)
// #define CS			PEout(12)
// #define SCLK		PEout(10)
// #define SDIO3		PEout(8)

// #define SDIO2		PBout(13)
// #define SDIO1		PBout(11)
// #define SDIO0		PEout(15)
// #define PS3		PEout(13)
// #define PS2		PEout(11)
// #define PS1		PEout(9)
// #define PS0		PEout(7)


void delay1 (uint32_t length);
void IntReset(void);	  //AD9959复位
void IO_Update(void);   //AD9959更新数据
void Intserve(void);		   //IO口初始化
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData,uint8_t temp);
void Init_AD9959(void);
void Write_frequence(uint8_t Channel,uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void Write_Phase(uint8_t Channel,uint16_t Phase);



#endif
