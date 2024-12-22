#ifndef LESSON11_DS1302_H
#define LESSON11_DS1302_H
#include <reg52.h>

typedef unsigned char u8;

extern unsigned char DS1302_Time[];
void  DS1302_Init();
void  DS1302_WriteBety(unsigned char command,Data);
unsigned char   DS1302_ReadBety(unsigned char command);
void DS1302_SetTime();
void DS1302_ReadTime();



#endif //LESSON11_DS1302_H