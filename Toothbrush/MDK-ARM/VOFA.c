#include "gpio.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "VOFA.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "math.h"


uint8_t RxBuffer[1];//
uint16_t RxLine = 0;//
uint8_t DataBuff[200];//
struct PID
{
	uint16_t P;
	uint16_t I;
	uint16_t D;
};
struct PID Speed_pid;

//VOFA调参

float Get_Data(void)//对获取VOFA发送的数据进行解析处理，
{
    uint8_t data_Start_Num = 0; // ??????????
    uint8_t data_End_Num = 0; // ??????????
    uint8_t data_Num = 0; // ??????
    uint8_t minus_Flag = 0; // ???????
    float data_return = 0; // ???????
    for(uint8_t i=0;i<200;i++) // ???????????
    {
        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1???????????
        if(DataBuff[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(DataBuff[data_Start_Num] == '-') // ?????
    {
        data_Start_Num += 1; // ????????
        minus_Flag = 1; // ??flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) // ???4?
    {
        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
                (DataBuff[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) // ???5?
    {
        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
                (DataBuff[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) // ???6?
    {
        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
//    printf("data=%.2f\r\n",data_return);
    return data_return;
}

void USART_PID_Adjust(uint8_t Motor_n)
{
    float data_Get = Get_Data(); // ????????
	   if(DataBuff[0]=='P' && DataBuff[1]=='2') // ???P
          Speed_pid.P = data_Get;
     else if(DataBuff[0]=='I' && DataBuff[1]=='2') // ???I
          Speed_pid.I = data_Get;
     else if(DataBuff[0]=='D' && DataBuff[1]=='2') // ???D
          Speed_pid.D = data_Get;
	
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)//回调函数获取VOFA发送的数据
{
    if(UartHandle->Instance==USART1)//?????2
    {
        RxLine++;                      //????????,?????????1
        DataBuff[RxLine-1]=RxBuffer[0];  //????????????????
        if(RxBuffer[0]==0x21)            //???????,?????????,??????,????????,????0x21
        {
//            printf("RXLen=%d\r\n",RxLine);
//            for(int i=0;i<RxLine;i++)
//                printf("UART DataBuff[%d] = %c\r\n",i,DataBuff[i]);
            USART_PID_Adjust(1);//???????????
            memset(DataBuff,0,sizeof(DataBuff));  //??????
            RxLine=0;  //??????
        }
        RxBuffer[0]=0;
        HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuffer, 1); //???????,???????????,???????????????
    }
}

