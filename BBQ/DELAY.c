#include "first.h"

// 单总线延时函数
void Delay_OneWire(unsigned int t)
{
    while(t--);
}

// 延时函数
void delay_ms(unsigned int ms) 
{
    unsigned int i, j;
    for (i = 0; i < ms; i++) 
    {
        for (j = 0; j < 120; j++);
    }
}