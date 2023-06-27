#include "hmi_driver.h"

#define TX_8(P1) SEND_DATA((P1)&0xFF)  //发送单个字节
#define TX_8N(P,N) SendNU8((uint8_t *)P,N)  //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)  //发送16位整数
#define TX_16N(P,N) SendNU16((uint16_t *)P,N)  //发送N个16位整数
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)  //发送32位整数

#if(CRC16_ENABLE)

static uint16_t _crc16 = 0xffff;
static void AddCRC16(uint8_t *buffer,uint16_t n,uint16_t *pcrc)
{
	uint16_t i,j,carry_flag,a;

	for (i=0; i<n; i++)
	{
		*pcrc=*pcrc^buffer[i];
		for (j=0; j<8; j++)
		{
			a=*pcrc;
			carry_flag=a&0x0001;
			*pcrc=*pcrc>>1;
			if (carry_flag==1)
				*pcrc=*pcrc^0xa001;
		}
	}
}

uint16_t CheckCRC16(uint8_t *buffer,uint16_t n)
{
	uint16_t crc0 = 0x0;
	uint16_t crc1 = 0xffff;

	if(n>=2)
	{
		crc0 = ((buffer[n-2]<<8)|buffer[n-1]);
		AddCRC16(buffer,n-2,&crc1);
	}

	return (crc0==crc1);
}

void SEND_DATA(uint8_t c)
{
	AddCRC16(&c,1,&_crc16);
	SendChar(c);
}

void BEGIN_CMD()
{
	TX_8(0XEE);
	_crc16 = 0XFFFF;//开始计算CRC16
}

void END_CMD()
{
	uint16_t crc16 = _crc16;
	TX_16(crc16);//发送CRC16
	TX_32(0XFFFCFFFF);
}

#else//NO CRC16

#define SEND_DATA(P) SendChar(P)
#define BEGIN_CMD() TX_8(0XEE)


#define END_CMD() TX_32(0XFFFCFFFF)

#endif

void DelayMS(unsigned int n) 
{
	int i,j;  
	for(i = n;i>0;i--)
		for(j=1000;j>0;j--) ; 
}

void SendStrings(uint8_t *str)
{
	while(*str)
	{
		TX_8(*str);
		str++;
	}
}

void SendNU8(uint8_t *pData,uint16_t nDataLen)
{
	uint16_t i = 0;
	for (;i<nDataLen;++i)
	{
		TX_8(pData[i]);
	}
}

void SendNU16(uint16_t *pData,uint16_t nDataLen)
{
	uint16_t i = 0;
	for (;i<nDataLen;++i)
	{
		TX_16(pData[i]);
	}
}

void SetHandShake()
{
	BEGIN_CMD();
	TX_8(0x00);
	END_CMD();
}

void SetFcolor(uint16_t color)
{
	BEGIN_CMD();
	TX_8(0x41);
	TX_16(color);
	END_CMD();
}

void SetBcolor(uint16_t color)
{
	BEGIN_CMD();
	TX_8(0x42);
	TX_16(color);
	END_CMD();
}

void ColorPicker(uint8_t mode, uint16_t x,uint16_t y)
{
	BEGIN_CMD();
	TX_8(0xA3);
	TX_8(mode);
	TX_16(x);
	TX_16(y);
	END_CMD();
}

void GUI_CleanScreen()
{
	BEGIN_CMD();
	TX_8(0x01);
	END_CMD();
}

void SetTextSpace(uint8_t x_w, uint8_t y_w)
{
	BEGIN_CMD();
	TX_8(0x43);
	TX_8(x_w);
	TX_8(y_w);
	END_CMD();
}

void SetFont_Region(uint8_t enable,uint16_t width,uint16_t height)
{
	BEGIN_CMD();
	TX_8(0x45);
	TX_8(enable);
	TX_16(width);
	TX_16(height);
	END_CMD();
}

void SetFilterColor(uint16_t fillcolor_dwon, uint16_t fillcolor_up)
{
	BEGIN_CMD();
	TX_8(0x44);
	TX_16(fillcolor_dwon);
	TX_16(fillcolor_up);
	END_CMD();
}

void DisText(uint16_t x, uint16_t y,uint8_t back,uint8_t font,uint8_t *strings )
{
	BEGIN_CMD();
	TX_8(0x20);
	TX_16(x);
	TX_16(y);
	TX_8(back);
	TX_8(font);
	SendStrings(strings);
	END_CMD();
}

void DisCursor(uint8_t enable,uint16_t x, uint16_t y,uint8_t width,uint8_t height )
{
	BEGIN_CMD();
	TX_8(0x21);
	TX_8(enable);
	TX_16(x);
	TX_16(y);
	TX_8(width);
	TX_8(height);
	END_CMD();
}

void DisFull_Image(uint16_t image_id,uint8_t masken)
{
	BEGIN_CMD();
	TX_8(0x31);
	TX_16(image_id);
	TX_8(masken);
	END_CMD();
}

void DisArea_Image(uint16_t x,uint16_t y,uint16_t image_id,uint8_t masken)
{
	BEGIN_CMD();
	TX_8(0x32);
	TX_16(x);
	TX_16(y);
	TX_16(image_id);
	TX_8(masken);
	END_CMD();
}

void DisCut_Image(uint16_t x,uint16_t y,uint16_t image_id,uint16_t image_x,uint16_t image_y,uint16_t image_l, uint16_t image_w,uint8_t masken)
{
	BEGIN_CMD();
	TX_8(0x33);
	TX_16(x);
	TX_16(y);
	TX_16(image_id);
	TX_16(image_x);
	TX_16(image_y);
	TX_16(image_l);
	TX_16(image_w);
	TX_8(masken);
	END_CMD();
}

void DisFlashImage(uint16_t x,uint16_t y,uint16_t flashimage_id,uint8_t enable,uint8_t playnum)
{
	BEGIN_CMD();
	TX_8(0x80);
	TX_16(x);
	TX_16(y);
	TX_16(flashimage_id);
	TX_8(enable);
	TX_8(playnum);
	END_CMD();
}

void GUI_Dot(uint16_t x,uint16_t y)
{
	BEGIN_CMD();
	TX_8(0x50);
	TX_16(x);
	TX_16(y);
	END_CMD();
}

void GUI_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	BEGIN_CMD();
	TX_8(0x51);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void GUI_ConDots(uint8_t mode,uint16_t *dot,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x63);
	TX_8(mode);
	TX_16N(dot,dot_cnt*2);
	END_CMD();
}

void GUI_ConSpaceDots(uint16_t x,uint16_t x_space,uint16_t *dot_y,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x59);
	TX_16(x);
	TX_16(x_space);
	TX_16N(dot_y,dot_cnt);
	END_CMD();
}

void GUI_FcolorConOffsetDots(uint16_t x,uint16_t y,uint16_t *dot_offset,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x75);
	TX_16(x);
	TX_16(y);
	TX_16N(dot_offset,dot_cnt);
	END_CMD();
}

void GUI_BcolorConOffsetDots(uint16_t x,uint16_t y,uint8_t *dot_offset,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x76);
	TX_16(x);
	TX_16(y);
	TX_16N(dot_offset,dot_cnt);
	END_CMD();
}

void SetPowerSaving(uint8_t enable, uint8_t bl_off_level, uint8_t bl_on_level, uint8_t  bl_on_time)
{
	BEGIN_CMD();
	TX_8(0x77);
	TX_8(enable);
	TX_8(bl_off_level);
	TX_8(bl_on_level);
	TX_8(bl_on_time);
	END_CMD();
}

void GUI_FcolorConDots(uint16_t *dot,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x68);
	TX_16N(dot,dot_cnt*2);
	END_CMD();
}

void GUI_BcolorConDots(uint16_t *dot,uint16_t dot_cnt)
{
	BEGIN_CMD();
	TX_8(0x69);
	TX_16N(dot,dot_cnt*2);
	END_CMD();
}

void GUI_Circle(uint16_t x, uint16_t y, uint16_t r)
{
	BEGIN_CMD();
	TX_8(0x52);
	TX_16(x);
	TX_16(y);
	TX_16(r);
	END_CMD();
}

void GUI_CircleFill(uint16_t x, uint16_t y, uint16_t r)
{
	BEGIN_CMD();
	TX_8(0x53);
	TX_16(x);
	TX_16(y);
	TX_16(r);
	END_CMD();
}

void GUI_Arc(uint16_t x,uint16_t y, uint16_t r,uint16_t sa, uint16_t ea)
{
	BEGIN_CMD();
	TX_8(0x67);
	TX_16(x);
	TX_16(y);
	TX_16(r);
	TX_16(sa);
	TX_16(ea);
	END_CMD();
}

void GUI_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 )
{
	BEGIN_CMD();
	TX_8(0x54);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void GUI_RectangleFill(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 )
{
	BEGIN_CMD();
	TX_8(0x55);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void GUI_Ellipse(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 )
{
	BEGIN_CMD();
	TX_8(0x56);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void GUI_EllipseFill(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 )
{
	BEGIN_CMD();
	TX_8(0x57);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void SetBackLight(uint8_t light_level)
{
	BEGIN_CMD();
	TX_8(0x60);
	TX_8(light_level);
	END_CMD();
}

void SetBuzzer(uint8_t time)
{
	BEGIN_CMD();
	TX_8(0x61);
	TX_8(time);
	END_CMD();
}

void GUI_AreaInycolor(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 )
{
	BEGIN_CMD();
	TX_8(0x65);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	END_CMD();
}

void SetTouchPaneOption(uint8_t enbale,uint8_t beep_on,uint8_t work_mode,uint8_t press_calibration)
{
	uint8_t options = 0;

	if(enbale)
		options |= 0x01;
	if(beep_on)
		options |= 0x02;
	if(work_mode)
		options |= (work_mode<<2);
	if(press_calibration)
		options |= (press_calibration<<5);

	BEGIN_CMD();
	TX_8(0x70);
	TX_8(options);
	END_CMD();
}

void CalibrateTouchPane()
{
	BEGIN_CMD();
	TX_8(0x72);
	END_CMD();
}

void TestTouchPane()
{
	BEGIN_CMD();
	TX_8(0x73);
	END_CMD();
}

void LockDeviceConfig(void)
{
	BEGIN_CMD();
	TX_8(0x09);
	TX_8(0xDE);
	TX_8(0xED);
	TX_8(0x13);
	TX_8(0x31);
	END_CMD();
}

void UnlockDeviceConfig(void)
{
	BEGIN_CMD();
	TX_8(0x08);
	TX_8(0xA5);
	TX_8(0x5A);
	TX_8(0x5F);
	TX_8(0xF5);
	END_CMD();
}

void SetCommBps(uint8_t option)
{
	BEGIN_CMD();
	TX_8(0xA0);
	TX_8(option);
	END_CMD();
}

void WriteLayer(uint8_t layer)
{
	BEGIN_CMD();
	TX_8(0xA1);
	TX_8(layer);
	END_CMD();
}

void DisplyLayer(uint8_t layer)
{
	BEGIN_CMD();
	TX_8(0xA2);
	TX_8(layer);
	END_CMD();
}

void CopyLayer(uint8_t src_layer,uint8_t dest_layer)
{
	BEGIN_CMD();
	TX_8(0xA4);
	TX_8(src_layer);
	TX_8(dest_layer);
	END_CMD();
}

void ClearLayer(uint8_t layer)
{
	BEGIN_CMD();
	TX_8(0x05);
	TX_8(layer);
	END_CMD();
}

void GUI_DispRTC(uint8_t enable,uint8_t mode,uint8_t font,uint16_t color,uint16_t x,uint16_t y)
{
	BEGIN_CMD();
	TX_8(0x85);
	TX_8(enable);
	TX_8(mode);
	TX_8(font);
	TX_16(color);
	TX_16(x);
	TX_16(y);
	END_CMD();
}

void WriteUserFlash(uint32_t startAddress,uint16_t length,uint8_t *_data)
{
	BEGIN_CMD();
	TX_8(0x87);
	TX_32(startAddress);
	TX_8N(_data,length);
	END_CMD();
}

void ReadUserFlash(uint32_t startAddress,uint16_t length)
{
	BEGIN_CMD();
	TX_8(0x88);
	TX_32(startAddress);
	TX_16(length);
	END_CMD();
}

void GetScreen()
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x01);
	END_CMD();
}

void SetScreen(uint16_t screen_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x00);
	TX_16(screen_id);
	END_CMD();
}

void SetScreenUpdateEnable(uint8_t enable)
{
	BEGIN_CMD();
	TX_8(0xB3);
	TX_8(enable);
	END_CMD();
}

void SetControlFocus(uint16_t screen_id,uint16_t control_id,uint8_t focus)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x02);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(focus);
	END_CMD();
}

//控制控件的显示与隐藏,1-显示;0-隐藏
void SetControlVisiable(uint16_t screen_id,uint16_t control_id,uint8_t visible)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x03);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(visible);
	END_CMD();
}

void SetControlEnable(uint16_t screen_id,uint16_t control_id,uint8_t enable)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x04);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(enable);
	END_CMD();
}

void SetButtonValue(uint16_t screen_id,uint16_t control_id,uint8_t state)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(state);
	END_CMD();
}

void SetTextValue(uint16_t screen_id,uint16_t control_id,uint8_t *str)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	SendStrings(str);
	END_CMD();
}

void SetProgressValue(uint16_t screen_id,uint16_t control_id,uint32_t value)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_32(value);
	END_CMD();
}

void SetMeterValue(uint16_t screen_id,uint16_t control_id,uint32_t value)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_32(value);
	END_CMD();
}

void SetSliderValue(uint16_t screen_id,uint16_t control_id,uint32_t value)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_32(value);
	END_CMD();
}

void SetSelectorValue(uint16_t screen_id,uint16_t control_id,uint8_t item)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(item);
	END_CMD();
}

void GetControlValue(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x11);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void AnimationStart(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x20);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void AnimationStop(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x21);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void AnimationPause(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x22);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void AnimationPlayFrame(uint16_t screen_id,uint16_t control_id,uint8_t frame_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x23);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(frame_id);
	END_CMD();
}

void AnimationPlayPrev(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x24);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void AnimationPlayNext(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x25);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void GraphChannelAdd(uint16_t screen_id,uint16_t control_id,uint8_t channel,uint16_t color)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x30);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(channel);
	TX_16(color);
	END_CMD();
}

void GraphChannelDel(uint16_t screen_id,uint16_t control_id,uint8_t channel)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x31);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(channel);
	END_CMD();
}

void GraphChannelDataAdd(uint16_t screen_id,uint16_t control_id,uint8_t channel,uint8_t *pData,uint16_t nDataLen)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x32);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(channel);
	TX_16(nDataLen);
	TX_8N(pData,nDataLen);
	END_CMD();
}


void GraphChannelDataClear(uint16_t screen_id,uint16_t control_id,uint8_t channel)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x33);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(channel);
	END_CMD();
}

void GraphSetViewport(uint16_t screen_id,uint16_t control_id,int16_t x_offset,uint16_t x_mul,int16_t y_offset,uint16_t y_mul)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x34);
	TX_16(screen_id);
	TX_16(control_id);
	TX_16(x_offset);
	TX_16(x_mul);
	TX_16(y_offset);
	TX_16(y_mul);
	END_CMD();
}

void BatchBegin(uint16_t screen_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x12);
	TX_16(screen_id);
}

void BatchSetButtonValue(uint16_t control_id,uint8_t state)
{
	TX_16(control_id);
	TX_16(1);
	TX_8(state);
}

void BatchSetProgressValue(uint16_t control_id,uint32_t value)
{
	TX_16(control_id);            
	TX_16(4);
	TX_32(value);
}

void BatchSetSliderValue(uint16_t control_id,uint32_t value)
{
	TX_16(control_id);
	TX_16(4);
	TX_32(value);
}

void BatchSetMeterValue(uint16_t control_id,uint32_t value)
{
	TX_16(control_id);
	TX_16(4);
	TX_32(value);
}

uint32_t GetStringLen(uint8_t *str)
{
	uint8_t *p = str;
	while(*str)
	{
		str++;
	}

	return (str-p);
} 

void BatchSetText(uint16_t control_id,uint8_t *strings)
{
	TX_16(control_id);
	TX_16(GetStringLen(strings));
	SendStrings(strings);
}

void BatchSetFrame(uint16_t control_id,uint16_t frame_id)
{
	TX_16(control_id);
	TX_16(2);
	TX_16(frame_id);
}

void BatchEnd()
{
	END_CMD();
}

void SeTimer(uint16_t screen_id,uint16_t control_id,uint32_t timeout)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x40);
	TX_16(screen_id);
	TX_16(control_id);
	TX_32(timeout);
	END_CMD();
}

void StartTimer(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x41);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void StopTimer(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x42);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void PauseTimer(uint16_t screen_id,uint16_t control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x44);
	TX_16(screen_id);
	TX_16(control_id);
	END_CMD();
}

void SetControlBackColor(uint16_t screen_id,uint16_t control_id,uint16_t color)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x18);
	TX_16(screen_id);
	TX_16(control_id);
	TX_16(color);
	END_CMD();
}

void SetControlForeColor(uint16_t screen_id,uint16_t control_id,uint16_t color)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x19);
	TX_16(screen_id);
	TX_16(control_id);
	TX_16(color);
	END_CMD();
}

void ShowPopupMenu(uint16_t screen_id,uint16_t control_id,uint8_t show,uint16_t focus_control_id)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x13);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(show);
	TX_16(focus_control_id);
	END_CMD();
}

void ShowKeyboard(uint8_t show,uint16_t x,uint16_t y,uint8_t type,uint8_t option,uint8_t max_len)
{
	BEGIN_CMD();
	TX_8(0x86);
	TX_8(show);
	TX_16(x);
	TX_16(y);
	TX_8(type);
	TX_8(option);
	TX_8(max_len);
	END_CMD();
}
