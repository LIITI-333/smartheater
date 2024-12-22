#include "first.h"

// LCD 初始化
void lcd_init() 
{
    delay_ms(15);
    lcd_command(0x38);  // 8位模式、2行、5x7点阵
    delay_ms(1);
    lcd_command(0x0C);  // 显示开启，光标关闭
    delay_ms(1);
    lcd_command(0x06);  // 光标右移
    delay_ms(1);
    lcd_command(0x01);  // 清屏
    delay_ms(2);
}

// 发送命令到 LCD
void lcd_command(unsigned char cmd) 
{
    RS = 0; 
    RW = 0;       
    LCD_DATA = cmd;
    EN = 1;
    delay_ms(1);
    EN = 0;
}

// 发送数据到 LCD
void lcd_data(unsigned char mata) 
{
    RS = 1; 
    RW = 0;       
    LCD_DATA = mata;
    EN = 1;
    delay_ms(1);
    EN = 0;
}

// 显示字符串
void lcd_display_string(char *str) 
{
    while (*str) 
    {
        lcd_data(*str++);
    }
}

// 显示整数
void lcd_display_integer(int value) 
{
    char buffer[6];
    unsigned char i = 0;
    unsigned char is_negative = 0;
    
    if (value < 0) 
    {
        is_negative = 1;
        value = -value;
    }
    
    // 转换数字到字符串
    do 
    {
        buffer[i++] = value % 10 + '0';
        value /= 10;
    } while (value);
    
    // 添加负号（如果是负数）
    if (is_negative) 
    {
        buffer[i++] = '-';
    }
    
    // 反转字符串并显示
    while (i > 0) 
    {
        lcd_data(buffer[--i]);
    }
}