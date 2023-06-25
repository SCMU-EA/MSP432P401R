#include "systeminit.h"

/**********************************	
	时钟：          默认时钟源        默认频率          描述
	MCLK             DCO              3MHZ          主时钟，向CPU和外设提供时钟
  HSMCLK       	   DCO              3MHZ          子系统主时钟，向外设提供时钟源
	SMCLK            DCO              3MHZ          低速系统主时钟，向外设提供时钟源
	ACLK     LFXT(或REFO没有晶振时)   32.768kHz     辅助时钟，向外设提供时钟
	BCLK     LFXT(或REFO没有晶振时)   32.768kHz     低速后配域时钟，提供LPM外设
	
	********************************/
	
void SystemClockInit(u8 ClockSource ){
 /* Halting the Watchdog */
  MAP_WDT_A_holdTimer();

	if(ClockSource==LFXT ){
///////////////////////////////////////////////////////////	
	        /* 配置外部低速时钟引脚*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	
    /* Setting the external clock frequency. This API is optional, but will
     * come in handy if the user ever wants to use the getMCLK/getACLK/etc
     * functions
     */
    CS_setExternalClockSourceFrequency(32768,48000000);

    /* Starting LFXT in non-bypass mode without a timeout. */
    CS_startLFXT(false);

    /* Initializing MCLK to LFXT (effectively 32khz) */ //主时钟
    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Since we are operating at 32khz, we can operating in LF mode */
   // MAP_PCM_setPowerMode(PCM_LF_MODE);
	}
	else if (ClockSource==HFXT){
////////////////////////////////////////////////////////////////		
		
		    /* 配置外部高速时钟引脚 */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
     * let's set the clock frequency in the code. 
     */
    CS_setExternalClockSourceFrequency(32000,48000000);

    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false);//false

    /* Initializing MCLK to HFXT (effectively 48MHz) */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
		
	}
	else  if (ClockSource==DCO){
////////////////////////////////////////////////////////////////////////////
           /*DCO时钟源配置，内部数控振荡器*/
    /* Enabling FPU for DCO Frequency calculation */
    FPU_enableModule();
    /* Setting the DCO Frequency to a non-standard 12MHz */
    CS_setDCOFrequency(CS_12MHZ);
		CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);//8~16MHZ
		
//	  CS_setDCOFrequency(CS_48MHZ);
//		CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);//8~16MHZ
		CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
			CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);//告诉子系统时钟
	}
	else {
//////////////////////////////////////////////////////////////////////////////
        /*内部低功耗低频时钟*/
			     /*设置频率*/
    CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ); 
    /* Initializing MCLK to REFO */
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Since we are operating at 32khz, we can operating in LF mode */
    MAP_PCM_setPowerMode(PCM_LF_MODE);
 
	}

 
}



