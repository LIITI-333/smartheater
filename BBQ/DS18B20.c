#include "first.h"

// DS18B20 初始化
bit ds18b20_init() 
{
    bit presence = 0;
    DQ = 1;
    _nop_();
    DQ = 0;
    Delay_OneWire(80);  // 延时480us以上
    DQ = 1;
    Delay_OneWire(8);   // 延时70us
    presence = DQ;      // 如果presence=0则初始化成功
    Delay_OneWire(20);
    return presence;
}

// DS18B20 写一个字节
void ds18b20_write_byte(unsigned char dat) 
{
    unsigned char i;
    for (i = 0; i < 8; i++) 
    {
        DQ = 0;
        _nop_();        // 短延时
        DQ = dat & 0x01;
        Delay_OneWire(10);
        DQ = 1;
        dat >>= 1;
    }
    Delay_OneWire(5);
}

// DS18B20 读一个字节
unsigned char ds18b20_read_byte() 
{
    unsigned char i, dat = 0;
    for (i = 0; i < 8; i++) 
    {
        DQ = 0;
        _nop_();    // 短延时
        DQ = 1;     // 释放总线
        _nop_();    // 短延时
        if (DQ)     // 读取数据位
            dat |= (0x01 << i);
        Delay_OneWire(10);
    }
    return dat;
}

// DS18B20 读取温度
int ds18b20_read_temperature() 
{
    int temp;
    unsigned char LSB, MSB;
    
    if (ds18b20_init())    // 初始化失败
        return -999;
        
    ds18b20_write_byte(0xCC);    // Skip ROM
    ds18b20_write_byte(0x44);    // Convert T
    
    delay_ms(750);               // 等待转换完成
    
    if (ds18b20_init())    // 初始化失败
        return -999;
        
    ds18b20_write_byte(0xCC);    // Skip ROM
    ds18b20_write_byte(0xBE);    // Read Scratchpad
    
    LSB = ds18b20_read_byte();   // 温度低字节
    MSB = ds18b20_read_byte();   // 温度高字节
    
    temp = MSB;
    temp <<= 8;
    temp |= LSB;
    
    // 温度值转换
    if (temp & 0xF800)     // 负温度
    {
        temp = (~temp) + 1;    // 补码转换
        temp = -temp;
    }
    temp = temp * 6.25;    // 转换为实际温度值的100倍
    
    return temp;
}