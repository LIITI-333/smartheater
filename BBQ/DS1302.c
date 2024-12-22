#include "ds1302.h"
#include "intrins.h"

#define  DS1302_SECOND		0X80
#define  DS1302_MINUTE  	0X82
#define  DS1302_HOUR		0X84
#define  DS1302_DATE        0X86
#define  DS1302_MONTH		0X88
#define  DS1302_DAY         0X8A
#define  DS1302_YEAR        0X8C
#define  DS1302_WP          0X8E 
 
 
 sbit DS1302_CE = P2^3;
// ʱ ӿ 
sbit DS1302_SCLK = P2^2;
// IO   
sbit DS1302_IO = P2^1;



unsigned char DS1302_Time[]={24,12, 16,15,42,59,6};

void  DS1302_Init()
{
	DS1302_CE=0;
	DS1302_SCLK=0;
}
 



void  DS1302_WriteBety(unsigned char command,Data)
{
			unsigned char i;
			DS1302_CE=1;
			for(i=0;i<8;i++)
		{
				DS1302_IO=command&(0x01<<i);
				DS1302_SCLK=1;
				DS1302_SCLK=0;
		}
		
				for(i=0;i<8;i++)
		{
				DS1302_IO=Data&(0x01<<i);
				DS1302_SCLK=1;
				DS1302_SCLK=0;
		}
			DS1302_CE=0;	
}
 
unsigned char DS1302_ReadBety( unsigned char command )
{
	unsigned char i,Data=0x00;
 
	command |= 0X01;
	DS1302_CE = 1;
	for(i=0;i<8;i++)
	{

		DS1302_IO = command & 0x01;
		command >>= 1;
		_nop_();
		DS1302_SCLK=1;
		_nop_();		
		DS1302_SCLK=0;
		_nop_();
	}
	for(i=0;i<8;i++)
	{
		if( DS1302_IO ) {
			Data |= ( 0X01 << i );
		}
		DS1302_SCLK=1;
		_nop_();
		DS1302_SCLK=0;
		_nop_();		

	}
	DS1302_CE=0;
	_nop_();
	return Data;
}
void DS1302_SetTime()
{
		DS1302_WriteBety(DS1302_WP,0x00);
		DS1302_WriteBety(DS1302_YEAR, DS1302_Time[0]/10*16+DS1302_Time[0]%10);
		DS1302_WriteBety(DS1302_MONTH, DS1302_Time[1]/10*16+DS1302_Time[1]%10);
		DS1302_WriteBety(DS1302_DATE, DS1302_Time[2]/10*16+DS1302_Time[2]%10);
		DS1302_WriteBety(DS1302_HOUR, DS1302_Time[3]/10*16+DS1302_Time[3]%10);
		DS1302_WriteBety(DS1302_MINUTE, DS1302_Time[4]/10*16+DS1302_Time[4]%10);
		DS1302_WriteBety(DS1302_SECOND, DS1302_Time[5]/10*16+DS1302_Time[5]%10);
		DS1302_WriteBety(DS1302_DAY, DS1302_Time[6]/10*16+DS1302_Time[6]%10);
		DS1302_WriteBety( DS1302_WP,0x80);
}
 
void DS1302_ReadTime()	
{
	unsigned char Temp;
    Temp=DS1302_ReadBety(DS1302_YEAR);
		DS1302_Time[0]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_MONTH);
		DS1302_Time[1]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_DATE);
		DS1302_Time[2]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_HOUR);
		DS1302_Time[3]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_MINUTE);
		DS1302_Time[4]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_SECOND);
		DS1302_Time[5]=Temp/16*10+Temp%16;
	 Temp=DS1302_ReadBety(DS1302_DAY);
		DS1302_Time[6]=Temp/16*10+Temp%16;
}
