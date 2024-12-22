#include "reg52.h"


sbit SCL=P2^3; 
sbit SDA=P2^4; 

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void delay(unsigned int z);

void IIC_Start();

void IIC_Stop();

void Write_IIC_Byte(unsigned char IIC_Byte);

void OLED_WrDat(unsigned char IIC_Data);

void OLED_WrCmd(unsigned char IIC_Command);

void OLED_Set_Pos(unsigned char x, unsigned char y);

void OLED_Fill(unsigned char bmp_dat);

void OLED_CLS(void);

void OLED_Init(void);

void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]);

void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]);

void OLED_P16x16Ch(unsigned char x, y, N);

void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]);

void OLED_ShowNum6x8(unsigned char x, unsigned char y, int num);
