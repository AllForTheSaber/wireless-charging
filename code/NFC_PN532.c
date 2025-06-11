#include "headfile.h"
#include "OLED.h" 
#include "OLED_Font.h"


uint8_t start_check[4] = {0xC6, 0x5E, 0x4D, 0x3D}; // �洢��һ�ſ���UID
uint8_t change_check[4] = {0x15, 0xF9, 0xCF, 0xB1};// �洢�ڶ��ſ���UID
uint8_t start[4] ; // �洢��һ�ſ���UID
uint8_t change[4] ;// �洢�ڶ��ſ���UID

uint8_t aRxBuffer[15];          //���ջ�������
uint8_t bRxBuffer[25];          //����Ѱ������

uint8_t Enter[] = "\r\n"; //�س�����

uint8_t hello0[]="Your PN532 has woken up successfully";
uint8_t hello1[]="Your PN532 has been successfully found";
uint8_t hello2[]="UID";

uint8_t UID[4]; //�洢 UID
uint8_t UID_HOST[4]={0XC2,0X99,0X4A,0X1B}; //�洢 UID
uint8_t NFC_flag=6; 



// ��ʾ UID �� OLED
//void Display_UID(uint8_t *uid, uint8_t len) {
//    char buffer[20];
//    uint8_t i;

//    // ����
//    OLED_Clear();

//    // �� UID ת��Ϊ�ַ���
//    buffer[0] = '\0';  // ��ʼ���ַ���
//    for (i = 0; i < len; i++) {
//        sprintf(buffer + strlen(buffer), "%02X ", uid[i]);  // ��ÿ���ֽ�ת��Ϊ��λʮ�������ַ���
//    }

//    // ��ʾ UID
//    OLED_ShowString(0, 0, (uint8_t *)buffer, 12);  // ����ʹ�� 12x12 ����
//}
void Display_UID(uint8_t *uid, uint8_t len) {
    char buffer[20];
    uint8_t i;

    // ����
    OLED_Clear();
	OLED_Clearrow(0);
	OLED_Clearrow(1);
	OLED_Clearrow(2);
	OLED_Clearrow(3);
//	OLED_ShowString(0, 0, (uint8_t *)"Welcome", 12);
//    if (uid == NULL || len == 0) {
//        // û�д��� UID����ʾĬ����Ϣ
//        OLED_ShowString(0, 0, (uint8_t *)"Welcome", 12);
//    } else {
//        // �� UID ת��Ϊ�ַ���
//        buffer[0] = '\0';  // ��ʼ���ַ���
//        for (i = 0; i < len; i++) {
//            sprintf(buffer + strlen(buffer), "%02X ", uid[i]);  // ��ÿ���ֽ�ת��Ϊ��λʮ�������ַ���
//        }

//        // ��ʾ UID
//        OLED_ShowString(0, 0, (uint8_t *)buffer, 12);  // ��ʾ UID

        // ��� UID �Ƿ���Ԥ��� UID ƥ��
        if (memcmp(uid, start_check, 4) == 0) {
					if(NFC_flag==6){
            OLED_ShowString(0, 2, (uint8_t *)"Charge begin", 12);  // ����ڶ�����ʼλ��Ϊ (0, 2)
            OLED_ShowString(0, 3, (uint8_t *)"lower pattern", 12);  // �����������ʼλ��Ϊ (0, 3)
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2200);
						NFC_flag=0;
					}else{
						OLED_ShowString(0, 2, (uint8_t *)"Charging has begun", 12);  // ����ڶ�����ʼλ��Ϊ (0, 2)
					}
        } else if (memcmp(uid, change_check, 4) == 0) {
            OLED_ShowString(0, 2, (uint8_t *)"Charge begin", 12);  // ����ڶ�����ʼλ��Ϊ (0, 2)
						
						if(NFC_flag==0){
							OLED_ShowString(0, 3, (uint8_t *)"higher pattern", 12);  // �����������ʼλ��Ϊ (0, 3)
							NFC_flag++;
							NFC_flag%=2;
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 4500);
//							ChangePWM(&htim1, 3800);
						
						}else if(NFC_flag==1){
							OLED_ShowString(0, 3, (uint8_t *)"lower pattern", 12);  // �����������ʼλ��Ϊ (0, 3)
							NFC_flag++;
						NFC_flag%=2;
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2200);
//							ChangePWM(&htim1, 3000);
						}else{
							OLED_ShowString(0, 2, (uint8_t *)"System locked", 12);
						}
						
        }else{
					OLED_ShowString(0, 2, (uint8_t *)"NFC no found", 12);  // ����ڶ�����ʼλ��Ϊ (0, 2)
				}
//    }
}



/*******************************************************************************
  * @��������   nfc_WakeUp
  * @����˵��   PN532�Դ�һ�����߹��ܣ�Ҫʹ��PN532��NFC��Ƭ���ж�д��ʱ��Ҫ����һ�£����ѣ���һ����ڳ���Ŀ�ͷ������һ�μ��ɡ�
                55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00   (24)
                �ɹ����� ���ظ�STM32   00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00        (15)                    
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void nfc_WakeUp(void)//����
{
    uint8_t adata[24]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};
    HAL_UART_Transmit(&huart1,(uint8_t *)&adata,sizeof(adata),0xFFFF); 
 
    HAL_UART_Receive(&huart1,(uint8_t *)&aRxBuffer,15,0xFFFF);
    //UART1����PN532���ظ���Ƭ�������ݣ�       00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00
    
    if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)hello0,sizeof(hello0), 0xFFFF);//Your PN532 has woken up successfully
        HAL_UART_Transmit(&huart1,(uint8_t *)&aRxBuffer,sizeof(aRxBuffer),0xFFFF);
        HAL_UART_Transmit(&huart1, (uint8_t*)Enter,2, 0xFFFF);
    }       
}
 
 
 // �洢��һ�ſ���UID
void StoreStartUID(uint8_t *uid) {
    for (int i = 0; i < 4; i++) {
        start[i] = uid[i];
    }
}

// �洢�ڶ��ſ���UID
void StoreChangeUID(uint8_t *uid) {
    for (int i = 0; i < 4; i++) {
        change[i] = uid[i];
    }
}

 
 
/*******************************************************************************
  * @��������   nfc_look
  * @����˵��   Ѱ�����漰����Ƭ�ĵ�һ�����裬nfc�Կ��Ĳ���������Ѱ�ҿ��ģ�
                  ��Ѱ��������������Ķ�д�������޷����У�
                                ��֮����Ѱ������������Ķ�д���������н����ٶԿ�Ƭ�������ȷ�ϡ�
                                
                                Ѱ�����00 00 FF 04 FC D4 4A 01 00 E1 00 (11)
                �ɹ��ҵ� ���ظ�STM32  00 00 FF 00 FF 00 00 00 FF 0C F4 D5 4B 01 01 00 04 08 04 C2 99 4A 1B 0E 00 (25)                      
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void nfc_look(void)//Ѱ��
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
			
			// �ж������ſ������洢��Ӧ��UID
        if (memcmp(UID, start, 4) == 0) {
            // ����ǵ�һ�ſ����洢��start
            StoreStartUID(UID);
            HAL_UART_Transmit(&huart1, (uint8_t*)"Card 1 detected\r\n", 19, 0xFFFF);
        } else if (memcmp(UID, change, 4) == 0) {
            // ����ǵڶ��ſ����洢��change
            StoreChangeUID(UID);
            HAL_UART_Transmit(&huart1, (uint8_t*)"Card 2 detected\r\n", 19, 0xFFFF);
        } else {
            // ������¿����洢��start��change
            HAL_UART_Transmit(&huart1, (uint8_t*)"New card detected\r\n", 22, 0xFFFF);
            StoreStartUID(UID); // �����¿��洢��start
        }
			
			
			// ��ʾ UID �� OLED
        Display_UID(UID, 4);  // ���� UID ����Ϊ 4 �ֽ�
 
    }       
}
 
 
 
 
 
//int uid_check(void)//UID����
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
        return 1; // ƥ���һ�ſ�
    } else if (memcmp(UID, change, 4) == 0) {
        return 1; // ƥ��ڶ��ſ�
    }
    return 0; // ��ƥ��
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
//void test(void)//�������
//{
//    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//    HAL_Delay(500);
//    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//    HAL_Delay(500);
//    printf("test�ɹ�");
//}
 
 
/**
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
 
/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}
