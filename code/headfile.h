#ifndef __HEADFEIL_H__
#define __HEADFEIL_H__


#include "stm32g0xx_hal.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "i2c.h"
#include "OLED.h"
#include "fun.h"
#include "tim.h"

//#include "fonts.h"


extern uint8_t lcd_fag;

#include "gpio.h"
#include "usart.h"

// 
//typedef   uint32_t   u32;///32?
//typedef   uint16_t   u16;///16?
//typedef   uint8_t     u8;///8?
 
void Test_USART1_Communication(void);
void nfc_WakeUp(void);//??
void nfc_look(void);//??
int uid_check(void);
void test(void);//????
 
// extern const unsigned char F6x8[][6];
//extern const unsigned char F8x16[];
// 
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

extern uint8_t uid[30];

#endif

