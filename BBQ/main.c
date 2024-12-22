#include "first.h"
// 主函数
void main() 
{
    int temperature;
    bit heating = 1;

    lcd_init();       // 初始化 LCD
    RELAY = 1;        // 初始状态打开继电器

    while (1) 
    {
        temperature = ds18b20_read_temperature();
        
        if(temperature != -999)  // 确保读取成功
        {
            // 控制加热逻辑
            if (temperature >= 10000)    // 100.00度
            {
                RELAY = 0;  // 停止加热
                heating = 0;
            } 
            else if (temperature < 9000)  // 90.00度
            {
                RELAY = 1;  // 开始加热
                heating = 1;
            }

            // LCD 显示温度和状态
            lcd_command(0x80);  // 第一行
            lcd_display_string("Temp: ");
            lcd_display_integer(temperature/100);
            lcd_display_string(".");
            if((temperature%100) < 10)
                lcd_display_string("0");
            lcd_display_integer(temperature%100);
            lcd_display_string("C  ");

            lcd_command(0xC0);  // 第二行
            if (heating) 
            {
                lcd_display_string("Heating: ON ");
            } 
            else 
            {
                lcd_display_string("Heating: OFF");
            }
        }
        else 
        {
            // 显示错误信息
            lcd_command(0x80);
            lcd_display_string("Temp Error    ");
            lcd_command(0xC0);
            lcd_display_string("Check Sensor  ");
        }
        
        delay_ms(1);  // 读取间隔
    }
}
