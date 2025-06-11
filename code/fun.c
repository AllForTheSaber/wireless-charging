#include "headfile.h"

//uint32_t freA_meature;
//uint32_t freB_meature;
//uint32_t captureA;
//uint32_t captureB;

//int freA;
//int freB;

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
//    if(htim->Instance==TIM1){
//        captureA= HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1)+1;
//        freA_meature=1000000/captureA;
//    }
////    if(htim->Instance==TIM3){
////        captureB= HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1;
////        freB_meature=1000000/captureB;
////    }
//}
void ChangePWM(TIM_HandleTypeDef *htim, uint16_t newPeriod)
{
    // 首先停止PWM输出
    HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
    
    // 更新ARR值
    __HAL_TIM_SET_AUTORELOAD(htim, newPeriod - 1); // ARR值需要减1
    
    // 重新配置定时器，应用新的ARR值
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
}
void ChangePWMFrequencyFromUART(uint16_t newPeriod)
{
    ChangePWM(&htim1, newPeriod);
}