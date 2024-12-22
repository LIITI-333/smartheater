#include <reg51.h>
#include <intrins.h>

// 定义引脚
sbit DQ = P1^0;           // DS18B20 数据引脚
sbit RELAY = P1^1;        // 继电器控制引脚
sbit RS = P2^0;           // LCD RS 引脚
sbit RW = P2^1;           // LCD RW 引脚
sbit EN = P2^2;           // LCD EN 引脚
#define LCD_DATA P0       // LCD 数据引脚

// 函数声明
void Delay_OneWire(unsigned int t);
void delay_ms(unsigned int ms);
void lcd_init();
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char mata);
void lcd_display_string(char *str);
void lcd_display_integer(int value);
bit ds18b20_init();
void ds18b20_write_byte(unsigned char dat);
unsigned char ds18b20_read_byte();
int ds18b20_read_temperature();