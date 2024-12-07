#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "beep.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "malloc.h"
#include "string.h"
#include "timers.h"
#include "event_groups.h"
#include "exti.h"
#include "usart3.h"
#include "esp8266.h"
#include "rc522.h"
#include "usmart.h"
#include "sdio_sdcard.h"
#include "w25qxx.h"
#include "touch.h"
#include "ff.h"
#include "exfuns.h"
#include "fontupd.h"
#include "bjdj.h"
#include "pwm.h"

//�������ȼ�
#define START_TASK_PRIO		1      //��ʼ����
//�����ջ��С
#define START_STK_SIZE 		512
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void* pvParameters);

//�������ȼ�
#define SG90_TASK_PRIO		4       //�������
//�����ջ��С
#define SG90_STK_SIZE 		512
//������
TaskHandle_t SG90Task_Handler;
//������
void SG90_task(void* pvParameters);

//�������ȼ�
#define LCD_TASK_PRIO		3         //LCD����
//�����ջ��С
#define LCD_STK_SIZE 		512
//������
TaskHandle_t LCDTask_Handler;
//������
void LCD_task(void* pvParameters);


//�������ȼ�
#define RFID_TASK_PRIO		3      //��Ƶʶ������
//�����ջ��С
#define RFID_STK_SIZE 		512
//������
TaskHandle_t RFIDTask_Handler;
//������
void RFID_task(void* pvParameters);

//�������ȼ�
#define ESP8266_TASK_PRIO		3    //WIFIģ������
//�����ջ��С
#define ESP8266_STK_SIZE 		512
//������
TaskHandle_t ESP8266Task_Handler;
//������
void ESP8266_task(void* pvParameters);

EventGroupHandle_t EventGroupHandler;	//�¼���־����

#define EVENTBIT_0	(1<<0)				//�¼�λ
#define EVENTBIT_1	(1<<1)
#define EVENTBIT_2	(1<<2)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2)


const  u8* kbd_menu[15] = {"coded", " : ", "lock", "1", "2", "3", "4", "5", "6", "7", "8", "9", "DEL", "0", "Enter",}; //������
u8 err = 0;
u8 key;


int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����4
    delay_init(168);					//��ʼ����ʱ����
    uart_init(115200);     				//��ʼ������
    usart3_init(115200);  //��ʼ������3������Ϊ115200
    LED_Init();		        			//��ʼ��LED�˿�
    KEY_Init();							//��ʼ������
    TIM2_PWM_Init(200 - 1, 7200 - 1);	 //����ƵPWMƵ��=72000000/1000/72=1Khz      ����20ms
    set_Angle(0);
    LCD_Init();							//��ʼ��LCD
    tp_dev.init();			//��ʼ��������
    RC522_Init();
    BEEP_Init();

    esp8266_start_trans();

    if(!(tp_dev.touchtype & 0x80)) //����ǵ�����
    {
        LCD_ShowString(0, 30, 200, 16, 16, "Adjust the LCD ?");
        POINT_COLOR = BLUE;
        LCD_ShowString(0, 60, 200, 16, 16, "yes:KEY1 no:KEY0");
        while(1)
        {
            key = KEY_Scan(0);
            if(key == KEY0_PRES)
                break;
            if(key == KEY1_PRES)
            {
                LCD_Clear(WHITE);
                TP_Adjust();  	 //��ĻУ׼
                TP_Save_Adjdata();//����У׼����
                break;
            }
        }
    }
    AS608_load_keyboard(0, 170, (u8**)kbd_menu); //�����������

    //������ʼ����
    xTaskCreate((TaskFunction_t)start_task,             //������
                (const char*)"start_task",              //��������
                (uint16_t)START_STK_SIZE,               //�����ջ��С
                (void*)NULL,                            //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,           //�������ȼ�
                (TaskHandle_t*)&StartTask_Handler);     //������
    vTaskStartScheduler();//�����������
}

//��ʼ����������
void start_task(void* pvParameters)
{
    BaseType_t xReturn;
    taskENTER_CRITICAL();           //�����ٽ���

    EventGroupHandler = xEventGroupCreate();
    if(NULL != EventGroupHandler)
        printf("EventGroupHandler�¼������ɹ�\r\n");

    //�����ĵ�һ���������������������
		xReturn = xTaskCreate((TaskFunction_t)SG90_task,
                          (const char*)"SG90_task",
                          (uint16_t)SG90_STK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)SG90_TASK_PRIO,
                          (TaskHandle_t*)&SG90Task_Handler);
    if(xReturn == pdPASS)
        printf("SG90_TASK_PRIO���񴴽��ɹ�\r\n");



    xReturn = xTaskCreate((TaskFunction_t)LCD_task,
                          (const char*)"LCD_task",
                          (uint16_t)LCD_STK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)LCD_TASK_PRIO,
                          (TaskHandle_t*)&LCDTask_Handler);
    if(xReturn == pdPASS)
        printf("LCD_TASK_PRIO���񴴽��ɹ�\r\n");

    xReturn = xTaskCreate((TaskFunction_t)RFID_task,
                          (const char*)"RFID_task",
                          (uint16_t)RFID_STK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)RFID_TASK_PRIO,
                          (TaskHandle_t*)&RFIDTask_Handler);
    if(xReturn == pdPASS)
        printf("RFID_TASK_PRIO���񴴽��ɹ�\r\n");


    xReturn = xTaskCreate((TaskFunction_t)ESP8266_task,
                          (const char*)"ESP8266_task",
                          (uint16_t)ESP8266_STK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)ESP8266_TASK_PRIO,
                          (TaskHandle_t*)&ESP8266Task_Handler);
    if(xReturn == pdPASS)
        printf("ESP8266_TASK_PRIO���񴴽��ɹ�\r\n");


    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void SG90_task(void* pvParameters)
{
    volatile EventBits_t EventValue;
    while(1)
    {
        EventValue = xEventGroupWaitBits(EventGroupHandler, EVENTBIT_ALL, pdTRUE, pdFALSE, portMAX_DELAY);

        printf("�����¼��ɹ�\r\n");
        set_Angle(180);
        delay_xms(1000);
        set_Angle(0);
        LCD_ShowString(80, 150, 260, 16, 16, "���ųɹ�");
        printf("���ųɹ�\r\n");

        vTaskDelay(100); //��ʱ10ms��Ҳ����10��ʱ�ӽ���
    }
}


void LCD_task(void* pvParameters)
{
    while(1)
    {
        if(GET_NUM())
        {
            LED1 = 0;
            BEEP = 1;
            delay_xms(50);
            LED1 = 1;
            BEEP = 0;
            printf("����������ȷ\r\n");
            LCD_ShowString(80, 150, 260, 16, 16, "password match");
            xEventGroupSetBits(EventGroupHandler, EVENTBIT_0);
        }
        else if(!GET_NUM())
        {
            LED0 = 0;
            BEEP = 1;
            delay_xms(150);
            LED0 = 1;
            BEEP = 0;
            printf("�����������\r\n");
            LCD_ShowString(80, 150, 260, 16, 16, "password error");
            err++;
            if(err == 3)
            {
                vTaskSuspend(SG90Task_Handler);
                printf("����������\r\n");
                LCD_ShowString(0, 100, 260, 16, 16, "Task has been suspended");
            }
        }
        vTaskDelay(100); //��ʱ10ms��Ҳ����10��ʱ�ӽ���
    }
}

void RFID_task(void* pvParameters)
{
    while(1)
    {
        int i = 0;
        i = RC522_Handel();

        if(i == 1)
        {
            BEEP = 1;
            LED1 = 0;
            delay_xms(50);
            LED1 = 1;
            BEEP = 0;
            xEventGroupSetBits(EventGroupHandler, EVENTBIT_1);
            printf("ʶ�𿨺ųɹ�\r\n");
        }
        else if(i == 2)
        {
            BEEP = 1;
            LED0 = 0;
            delay_xms(150);
            BEEP = 0;
            LED0 = 1;
            printf("ʶ�𿨺�ʧ��\r\n");
            err++;
            if(err == 3)
            {
                vTaskSuspend(SG90Task_Handler);
                printf("����������\r\n");
                LCD_ShowString(0, 100, 260, 16, 16, "Task has been suspended");
            }
        }
        vTaskDelay(100); //��ʱ10ms��Ҳ����10��ʱ�ӽ���
    }
}

void ESP8266_task(void* pvParameters)
{
    while(1)
    {
        if(USART3_RX_STA)
        {
            if(strstr((const char*)USART3_RX_BUF, "123456"))
            {
                BEEP = 1;
                LED1 = 0;
                delay_xms(50);
                BEEP = 0;
                LED1 = 1;
                printf("���ųɹ�\r\n");
                xEventGroupSetBits(EventGroupHandler, EVENTBIT_2);
                memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
            }
            else
            {
                BEEP = 1;
                LED0 = 0;
                delay_xms(150);
                BEEP = 0;
                LED0 = 1;
                printf("�������\r\n");
                memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
            }
            USART3_RX_STA = 0;
        }
        vTaskDelay(100);
    }
}
