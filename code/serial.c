//#include "serial.h"
//#include "headfile.h"

//USART_HandleTypeDef huart1;

//void serial3_init(void)
//{
//    huart1.Instance = USART1;
//    huart1.Init.BaudRate = 115200;
//    huart1.Init.WordLength = USART_WORDLENGTH_8B;
//    huart1.Init.StopBits = USART_STOPBITS_1;
//    huart1.Init.Parity = USART_PARITY_NONE;
//    huart1.Init.Mode = USART_MODE_TX_RX;
//    huart1.Init.HwFlowCtl = USART_HWCONTROL_NONE;
//    huart1.Init.OverSampling = USART_OVERSAMPLING_16;
//    HAL_UART_Init(&huart1);
//}

//void serial3_send_char(char ch)
//{
//    HAL_USART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//}