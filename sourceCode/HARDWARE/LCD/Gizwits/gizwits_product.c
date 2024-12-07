/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart3.h"
static uint32_t timerMsCount;

/** Current datapoint */
extern dataPoint_t currentDataPoint;

extern u8 wifi_sta;
/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_Yaobi:
        currentDataPoint.valueYaobi = dataPointPtr->valueYaobi;
        GIZWITS_LOG("Evt: EVENT_Yaobi %d \n", currentDataPoint.valueYaobi);
        if(0x01 == currentDataPoint.valueYaobi)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_Y_qianjin:
        currentDataPoint.valueY_qianjin = dataPointPtr->valueY_qianjin;
        GIZWITS_LOG("Evt: EVENT_Y_qianjin %d \n", currentDataPoint.valueY_qianjin);
        if(0x01 == currentDataPoint.valueY_qianjin)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_Y_houtui:
        currentDataPoint.valueY_houtui = dataPointPtr->valueY_houtui;
        GIZWITS_LOG("Evt: EVENT_Y_houtui %d \n", currentDataPoint.valueY_houtui);
        if(0x01 == currentDataPoint.valueY_houtui)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_Yeya:
        currentDataPoint.valueYeya = dataPointPtr->valueYeya;
        GIZWITS_LOG("Evt: EVENT_Yeya %d \n", currentDataPoint.valueYeya);
        if(0x01 == currentDataPoint.valueYeya)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_X_qianjin:
        currentDataPoint.valueX_qianjin = dataPointPtr->valueX_qianjin;
        GIZWITS_LOG("Evt: EVENT_X_qianjin %d \n", currentDataPoint.valueX_qianjin);
        if(0x01 == currentDataPoint.valueX_qianjin)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_X_houtui:
        currentDataPoint.valueX_houtui = dataPointPtr->valueX_houtui;
        GIZWITS_LOG("Evt: EVENT_X_houtui %d \n", currentDataPoint.valueX_houtui);
        if(0x01 == currentDataPoint.valueX_houtui)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_LED_ON:
        currentDataPoint.valueLED_ON = dataPointPtr->valueLED_ON;
        GIZWITS_LOG("Evt: EVENT_LED_ON %d \n", currentDataPoint.valueLED_ON);
        if(0x01 == currentDataPoint.valueLED_ON)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;
      case EVENT_LED_OFF:
        currentDataPoint.valueLED_OFF = dataPointPtr->valueLED_OFF;
        GIZWITS_LOG("Evt: EVENT_LED_OFF %d \n", currentDataPoint.valueLED_OFF);
        if(0x01 == currentDataPoint.valueLED_OFF)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;


      case EVENT_Y_qianjin_value:
        currentDataPoint.valueY_qianjin_value = dataPointPtr->valueY_qianjin_value;
        GIZWITS_LOG("Evt:EVENT_Y_qianjin_value %d\n",currentDataPoint.valueY_qianjin_value);
        //user handle
        break;
      case EVENT_Y_houtui_value:
        currentDataPoint.valueY_houtui_value = dataPointPtr->valueY_houtui_value;
        GIZWITS_LOG("Evt:EVENT_Y_houtui_value %d\n",currentDataPoint.valueY_houtui_value);
        //user handle
        break;
      case EVENT_X_qianjin_value:
        currentDataPoint.valueX_qianjin_value = dataPointPtr->valueX_qianjin_value;
        GIZWITS_LOG("Evt:EVENT_X_qianjin_value %d\n",currentDataPoint.valueX_qianjin_value);
        //user handle
        break;
      case EVENT_X_houtui_value:
        currentDataPoint.valueX_houtui_value = dataPointPtr->valueX_houtui_value;
        GIZWITS_LOG("Evt:EVENT_X_houtui_value %d\n",currentDataPoint.valueX_houtui_value);
        //user handle
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:wifi_sta=1;
 
        break;
      case WIFI_DISCON_M2M:wifi_sta=0;
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
/*
void userHandle(void)
{
 

    
    
}
*/
/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueYaobi = ;
      currentDataPoint.valueY_qianjin = ;
      currentDataPoint.valueY_houtui = ;
      currentDataPoint.valueYeya = ;
      currentDataPoint.valueX_qianjin = ;
      currentDataPoint.valueX_houtui = ;
      currentDataPoint.valueLED_ON = ;
      currentDataPoint.valueLED_OFF = ;
      currentDataPoint.valueY_qianjin_value = ;
      currentDataPoint.valueY_houtui_value = ;
      currentDataPoint.valueX_qianjin_value = ;
      currentDataPoint.valueX_houtui_value = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);//关闭所有中断
    NVIC_SystemReset();//复位
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
				USART_SendData(USART3,buf[i]);
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
					USART_SendData(USART3,0x55);
          while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
        }
    }


    
    return len;
}


