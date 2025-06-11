//#include "stm32g0xx_hal.h"
//#include "NFC_PN532.h"

//UART_HandleTypeDef huart1;

//int main(void)
//{
//    HAL_Init();
//    SystemClock_Config(); // 配置系统时钟
//    MX_GPIO_Init();       // 初始化GPIO
//    MX_USART1_UART_Init(); // 初始化USART1
//    serial3_init();       // 初始化串口

//    uint8_t UID[4];
//    uint8_t block = 0;

//    while (1)
//    {
//        if (awaken_nfc())
//        {
//            if (find_card(UID))
//            {
//                uint8_t data[16] = {0};
//                if (read_block(data, block))
//                {
//                    for (uint8_t i = 0; i < 16; i++)
//                    {
//                        printf("%02X ", data[i]);
//                    }
//                    printf("\r\n");
//                }
//            }
//        }
//        HAL_Delay(1000);
//    }
//}

//void SystemClock_Config(void)
//{
//    // 系统时钟配置代码
//}

//static void MX_GPIO_Init(void)
//{
//    // GPIO初始化代码
//}

//static void MX_USART1_UART_Init(void)
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