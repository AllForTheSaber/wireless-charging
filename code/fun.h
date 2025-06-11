#ifndef __FUN_H_
#define __FUN_H_	
 
#include "stdint.h"
#include "i2c.h"
 void ChangePWM(TIM_HandleTypeDef *htim, uint16_t newPeriod);
#endif
