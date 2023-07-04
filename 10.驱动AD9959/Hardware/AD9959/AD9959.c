#include "AD9959.h"


uint8_t CSR_DATA0[1] = {0x10}; // 开 CH0
uint8_t CSR_DATA1[1] = {0x20}; // 开 CH1
uint8_t CSR_DATA2[1] = {0x40}; // 开 CH2
uint8_t CSR_DATA3[1] = {0x80}; // 开 CH3

uint8_t FR2_DATA[2] = {0x00, 0x00};		  // default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00, 0x03, 0x02}; // default Value = 0x000302

uint8_t CPOW0_DATA[2] = {0x00, 0x00}; // default Value = 0x0000   @ = POW/2^14*360

uint8_t LSRR_DATA[2] = {0x00, 0x00}; // default Value = 0x----

uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------

uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // default Value = 0x--------

// AD9959初始化
void Init_AD9959(void)
{
	uint8_t FR1_DATA[3] = {0xD0, 0x00, 0x00}; // 20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;

	/* MSP432引脚初始化 */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 );
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN6 | GPIO_PIN7 );
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0 );
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 );


	Intserve(); // IO口初始化
	IntReset(); // AD9959复位

	WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1); // 写功能寄存器1
	//  WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);
	//  WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);
	//  WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
	//  WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
	//  WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
	//  WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);
	//  WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);

	// 写入初始频率

	Write_frequence(0, 5000000);
	Write_frequence(1, 10000000);
	Write_frequence(2, 40000000);
	Write_frequence(3, 40000000);

	// 写入初始相位
	Write_Phase(0, 0);
	Write_Phase(1, 0);
	Write_Phase(2, 0);
	Write_Phase(3, 0);

	// 写入初始幅度
	Write_Amplitude(0, 250);
	Write_Amplitude(1, 200);
	Write_Amplitude(2, 933);
	Write_Amplitude(3, 100);
}
// 延时
void delay1(uint32_t length)
{
	length = length * 12;
	while (length--)
		;
}
// IO口初始化
void Intserve(void)
{
	AD9959_PWR_L();
	CS_H();
	SCLK_L();
	UPDATE_L();
	PS0_L();
	PS1_L();
	PS2_L();
	PS3_L();
	SDIO0_L();
	SDIO1_L();
	SDIO2_L();
	SDIO3_L();
}
// AD9959复位
void IntReset(void)
{
	Reset_L();
	delay1(1);
	Reset_H();
	delay1(30);
	Reset_L();
}
// AD9959更新数据
void IO_Update(void)
{
	UPDATE_L();
	delay1(2);
	UPDATE_H();
	delay1(4);
	UPDATE_L();
}
/*--------------------------------------------
函数功能：控制器通过SPI向AD9959写数据
RegisterAddress: 寄存器地址
NumberofRegisters: 所含字节数
*RegisterData: 数据起始地址
temp: 是否更新IO寄存器
----------------------------------------------*/
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData, uint8_t temp)
{
	uint8_t ControlValue = 0;
	uint8_t ValueToWrite = 0;
	uint8_t RegisterIndex = 0;
	uint8_t i = 0;

	ControlValue = RegisterAddress;
	// 写入地址
	SCLK_L();
	CS_L();
	for (i = 0; i < 8; i++)
	{
		SCLK_L();
		if (0x80 == (ControlValue & 0x80))
			SDIO0_H();
		else
			SDIO0_L();
		SCLK_H();
		ControlValue <<= 1;
	}
	SCLK_L();
	// 写入数据
	for (RegisterIndex = 0; RegisterIndex < NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i = 0; i < 8; i++)
		{
			SCLK_L();
			if (0x80 == (ValueToWrite & 0x80))
				SDIO0_H();
			else
				SDIO0_L();
			SCLK_H();
			ValueToWrite <<= 1;
		}
		SCLK_L();
	}
	if (temp == 1)
		IO_Update();
	CS_H();
}
/*---------------------------------------
函数功能：设置通道输出频率
Channel:  输出通道
Freq:     输出频率
---------------------------------------*/
void Write_frequence(uint8_t Channel, uint32_t Freq)
{
	uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00}; // 中间变量
	uint32_t Temp;
	Temp = (uint32_t)Freq * 8.589934592; // 将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
	CFTW0_DATA[3] = (uint8_t)Temp;
	CFTW0_DATA[2] = (uint8_t)(Temp >> 8);
	CFTW0_DATA[1] = (uint8_t)(Temp >> 16);
	CFTW0_DATA[0] = (uint8_t)(Temp >> 24);
	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);	   // 控制寄存器写入CH0通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH0设定频率
	}
	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);	   // 控制寄存器写入CH1通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH1设定频率
	}
	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);	   // 控制寄存器写入CH2通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 1); // CTW0 address 0x04.输出CH2设定频率
	}
	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);	   // 控制寄存器写入CH3通道
		WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA, 3); // CTW0 address 0x04.输出CH3设定频率
	}
}
/*---------------------------------------
函数功能：设置通道输出幅度
Channel:  输出通道
Ampli:    输出幅度
---------------------------------------*/
void Write_Amplitude(uint8_t Channel, uint16_t Ampli)
{
	uint16_t A_temp;						  //=0x23ff;
	uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00}; // default Value = 0x--0000 Rest = 18.91/Iout

	A_temp = Ampli | 0x1000;
	ACR_DATA[2] = (uint8_t)A_temp;		  // 低位数据
	ACR_DATA[1] = (uint8_t)(A_temp >> 8); // 高位数据
	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}
	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}
	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}
	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 1);
		WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
	}
}
/*---------------------------------------
函数功能：设置通道输出相位
Channel:  输出通道
Phase:    输出相位,范围：0~16383(对应角度：0°~360°)
---------------------------------------*/
void Write_Phase(uint8_t Channel, uint16_t Phase)
{
	uint16_t P_temp = 0;
	P_temp = (uint16_t)Phase;
	CPOW0_DATA[1] = (uint8_t)P_temp;
	CPOW0_DATA[0] = (uint8_t)(P_temp >> 8);
	if (Channel == 0)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
	}
	else if (Channel == 1)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
	}
	else if (Channel == 2)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
	}
	else if (Channel == 3)
	{
		WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);
		WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 0);
	}
}
