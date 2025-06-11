#include "headfile.h"
#include "OLED.h" 
#include "OLED_Font.h"


uint8_t start_check[4] = {0xC6, 0x5E, 0x4D, 0x3D}; // 存储第一张卡的UID
uint8_t change_check[4] = {0x15, 0xF9, 0xCF, 0xB1};// 存储第二张卡的UID
uint8_t start[4] ; // 存储第一张卡的UID
uint8_t change[4] ;// 存储第二张卡的UID

uint8_t aRxBuffer[15];          //接收唤醒数据
uint8_t bRxBuffer[25];          //接收寻卡数据

uint8_t Enter[] = "\r\n"; //回车换行

uint8_t hello0[]="Your PN532 has woken up successfully";
uint8_t hello1[]="Your PN532 has been successfully found";
uint8_t hello2[]="UID";

uint8_t UID[4]; //存储 UID
uint8_t UID_HOST[4]={0XC2,0X99,0X4A,0X1B}; //存储 UID
uint8_t NFC_flag=6; 



// 显示 UID 到 OLED
//void Display_UID(uint8_t *uid, uint8_t len) {
//    char buffer[20];
//    uint8_t i;

//    // 清屏
//    OLED_Clear();

//    // 将 UID 转换为字符串
//    buffer[0] = '\0';  // 初始化字符串
//    for (i = 0; i < len; i++) {
//        sprintf(buffer + strlen(buffer), "%02X ", uid[i]);  // 将每个字节转换为两位十六进制字符串
//    }

//    // 显示 UID
//    OLED_ShowString(0, 0, (uint8_t *)buffer, 12);  // 假设使用 12x12 字体
//}
void Display_UID(uint8_t *uid, uint8_t len) {
    char buffer[20];
    uint8_t i;

    // 清屏
    OLED_Clear();
	OLED_Clearrow(0);
	OLED_Clearrow(1);
	OLED_Clearrow(2);
	OLED_Clearrow(3);
//	OLED_ShowString(0, 0, (uint8_t *)"Welcome", 12);
//    if (uid == NULL || len == 0) {
//        // 没有传入 UID，显示默认消息
//        OLED_ShowString(0, 0, (uint8_t *)"Welcome", 12);
//    } else {
//        // 将 UID 转换为字符串
//        buffer[0] = '\0';  // 初始化字符串
//        for (i = 0; i < len; i++) {
//            sprintf(buffer + strlen(buffer), "%02X ", uid[i]);  // 将每个字节转换为两位十六进制字符串
//        }

//        // 显示 UID
//        OLED_ShowString(0, 0, (uint8_t *)buffer, 12);  // 显示 UID

        // 检查 UID 是否与预设的 UID 匹配
        if (memcmp(uid, start_check, 4) == 0) {
					if(NFC_flag==6){
            OLED_ShowString(0, 2, (uint8_t *)"Charge begin", 12);  // 假设第二行起始位置为 (0, 2)
            OLED_ShowString(0, 3, (uint8_t *)"lower pattern", 12);  // 假设第三行起始位置为 (0, 3)
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2200);
						NFC_flag=0;
					}else{
						OLED_ShowString(0, 2, (uint8_t *)"Charging has begun", 12);  // 假设第二行起始位置为 (0, 2)
					}
        } else if (memcmp(uid, change_check, 4) == 0) {
            OLED_ShowString(0, 2, (uint8_t *)"Charge begin", 12);  // 假设第二行起始位置为 (0, 2)
						
						if(NFC_flag==0){
							OLED_ShowString(0, 3, (uint8_t *)"higher pattern", 12);  // 假设第三行起始位置为 (0, 3)
							NFC_flag++;
							NFC_flag%=2;
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 4500);
//							ChangePWM(&htim1, 3800);
						
						}else if(NFC_flag==1){
							OLED_ShowString(0, 3, (uint8_t *)"lower pattern", 12);  // 假设第三行起始位置为 (0, 3)
							NFC_flag++;
						NFC_flag%=2;
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2200);
//							ChangePWM(&htim1, 3000);
						}else{
							OLED_ShowString(0, 2, (uint8_t *)"System locked", 12);
						}
						
        }else{
					OLED_ShowString(0, 2, (uint8_t *)"NFC no found", 12);  // 假设第二行起始位置为 (0, 2)
				}
//    }
}



/*******************************************************************************
  * @函数名称   nfc_WakeUp
  * @函数说明   PN532自带一个休眠功能，要使用PN532对NFC卡片进行读写的时候要激活一下（唤醒），一般放在程序的开头，调用一次即可。
                55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00   (24)
                成功唤醒 返回给STM32   00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00        (15)                    
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void nfc_WakeUp(void)//唤醒
{
    uint8_t adata[24]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};
    HAL_UART_Transmit(&huart1,(uint8_t *)&adata,sizeof(adata),0xFFFF); 
 
    HAL_UART_Receive(&huart1,(uint8_t *)&aRxBuffer,15,0xFFFF);
    //UART1接收PN532返回给单片机的数据：       00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00
    
    if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)hello0,sizeof(hello0), 0xFFFF);//Your PN532 has woken up successfully
        HAL_UART_Transmit(&huart1,(uint8_t *)&aRxBuffer,sizeof(aRxBuffer),0xFFFF);
        HAL_UART_Transmit(&huart1, (uint8_t*)Enter,2, 0xFFFF);
    }       
}
 
 
 // 存储第一张卡的UID
void StoreStartUID(uint8_t *uid) {
    for (int i = 0; i < 4; i++) {
        start[i] = uid[i];
    }
}

// 存储第二张卡的UID
void StoreChangeUID(uint8_t *uid) {
    for (int i = 0; i < 4; i++) {
        change[i] = uid[i];
    }
}

 
 
/*******************************************************************************
  * @函数名称   nfc_look
  * @函数说明   寻卡是涉及到卡片的第一个步骤，nfc对卡的操作都是先寻找卡的，
                  若寻不到卡，则后续的读写操作将无法进行；
                                反之，若寻到卡，则后续的读写操作过程中将不再对卡片进行身份确认。
                                
                                寻卡命令：00 00 FF 04 FC D4 4A 01 00 E1 00 (11)
                成功找到 返回给STM32  00 00 FF 00 FF 00 00 00 FF 0C F4 D5 4B 01 01 00 04 08 04 C2 99 4A 1B 0E 00 (25)                      
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void nfc_look(void)//寻卡
{
    uint8_t bdata[11]={0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00};
    HAL_UART_Transmit(&huart1,(uint8_t *)&bdata,sizeof(bdata),0xFFFF); 
 
    HAL_UART_Receive(&huart1,(uint8_t *)&bRxBuffer,25,0xFFFF);
    
    if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)hello1,sizeof(hello1), 0xFFFF);//"Your PN532 has been successfully found";
        HAL_UART_Transmit(&huart1,(uint8_t *)&bRxBuffer,sizeof(bRxBuffer),0xFFFF);
        HAL_UART_Transmit(&huart1, (uint8_t*)Enter,2, 0xFFFF);
 
        UID[0]=bRxBuffer[19];
        UID[1]=bRxBuffer[20];
        UID[2]=bRxBuffer[21];
        UID[3]=bRxBuffer[22]; 
 
        HAL_UART_Transmit(&huart1, (uint8_t *)hello2,sizeof(hello2), 0xFFFF);
 
        HAL_UART_Transmit(&huart1,(uint8_t *)&UID,4,0xFFFF);
        HAL_UART_Transmit(&huart1, (uint8_t*)Enter,2, 0xFFFF);
			
			// 判断是哪张卡，并存储对应的UID
        if (memcmp(UID, start, 4) == 0) {
            // 如果是第一张卡，存储到start
            StoreStartUID(UID);
            HAL_UART_Transmit(&huart1, (uint8_t*)"Card 1 detected\r\n", 19, 0xFFFF);
        } else if (memcmp(UID, change, 4) == 0) {
            // 如果是第二张卡，存储到change
            StoreChangeUID(UID);
            HAL_UART_Transmit(&huart1, (uint8_t*)"Card 2 detected\r\n", 19, 0xFFFF);
        } else {
            // 如果是新卡，存储到start或change
            HAL_UART_Transmit(&huart1, (uint8_t*)"New card detected\r\n", 22, 0xFFFF);
            StoreStartUID(UID); // 假设新卡存储到start
        }
			
			
			// 显示 UID 到 OLED
        Display_UID(UID, 4);  // 假设 UID 长度为 4 字节
 
    }       
}
 
 
 
 
 
//int uid_check(void)//UID核验
//{
//    
//        for(int i = 0; i < sizeof(UID_HOST); i++)
//        {
//            if(UID[i]!=UID_HOST[i])
//            {
//                break;
//            }
//        } 
//        return 1;
//        
//}

int uid_check(void) {
    if (memcmp(UID, start, 4) == 0) {
        return 1; // 匹配第一张卡
    } else if (memcmp(UID, change, 4) == 0) {
        return 1; // 匹配第二张卡
    }
    return 0; // 不匹配
}



 
//void control(void)
//{
//    if(uid_check()==1)
//    {
//        test();
//    }
//}
// 
// 
//void test(void)//方便测试
//{
//    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//    HAL_Delay(500);
//    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//    HAL_Delay(500);
//    printf("test成功");
//}
 
 
/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
 
/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}
