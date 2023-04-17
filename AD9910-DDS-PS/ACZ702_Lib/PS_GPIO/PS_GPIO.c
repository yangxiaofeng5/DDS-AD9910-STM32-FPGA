/**
  *****************************************************************************
  * 						PS部分GPIO相关应用库
  *****************************************************************************
  *
  * @File   : PS_GPIO.c
  * @By     : Sun
  * @Version: V1.2
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "PS_GPIO.h"

XGpioPs GpioPs;	//GPIO实例对象


/**
  *****************************************************
  * @brief	初始化PS端MIO和EMIO
  * @usage	PS_GPIO_Init();	//初始化PS端GPIO
  *****************************************************
**/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&GpioPs, ConfigPtr, ConfigPtr->BaseAddr);
}


/**
  *****************************************************
  * @brief	初始化PS GPIO的中断功能
  * @usage	//初始化PS GPIO的中断功能
  * 		PS_GPIO_Int_Init();
  *****************************************************
**/
//void PS_GPIO_Int_Init()
//{
    //链接到中断控制器
//    Set_ScuGic_Link(XPAR_XGPIOPS_0_INTR, 0xA0, HIGH_Level_Sensitive,
//    		(Xil_InterruptHandler)XGpioPs_IntrHandler, (void *)&GpioPs);

	//为 gpio中断设置处理程序
//	XGpioPs_SetCallbackHandler(&GpioPs, (void *)&GpioPs, PS_GPIO_IRQ_Handler);
//}


/**
  *****************************************************
  * @brief	启用GPIO某Pin的中断功能
  * @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
  * @param	IntcMode	GPIO中断触发模式
  * 					XGPIOPS_IRQ_TYPE_EDGE_RISING	上升沿触发
  * 					XGPIOPS_IRQ_TYPE_EDGE_FALLING	下降沿触发
  * 					XGPIOPS_IRQ_TYPE_EDGE_BOTH		上升或下降沿都触发
  * 					XGPIOPS_IRQ_TYPE_LEVEL_HIGH		高电平触发
  * 					XGPIOPS_IRQ_TYPE_LEVEL_LOW		低电平触发
  * @usage	//设置MIO47为下降沿中断
  * 		PS_GPIO_SetInt(47,XGPIOPS_IRQ_TYPE_EDGE_FALLING);
  *****************************************************
**/
void PS_GPIO_SetInt(uint8_t GPIO_Num,uint8_t IntcMode)
{
	//设置引脚中断模式
	XGpioPs_SetIntrTypePin(&GpioPs, GPIO_Num, IntcMode);

	//使能Bank的GPIO Pin中断
	XGpioPs_IntrEnablePin(&GpioPs, GPIO_Num);
}

/**
  *****************************************************
  * @brief	初始化特定GPIO口的模式与状态
  * @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
  * @param	Dir			输入/输出：OUTPUT为输出，INPUT为输入
  * @param	Data		输出电平高低：0为低，1为高（若设置为输入则此处数据无影响）
  * @usage	//设置GPIO46为输出模式，输出为高电平
  * 		GPIO_SetMode(46, OUTPUT, 1);
  *****************************************************
**/
void PS_GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dir, uint8_t Data)
{
	if(Dir == INPUT) {
		XGpioPs_SetDirectionPin(&GpioPs, GPIO_Num, 0);
	} else if(Dir == OUTPUT){
		XGpioPs_SetDirectionPin(&GpioPs, GPIO_Num, 1);
		XGpioPs_SetOutputEnablePin(&GpioPs, GPIO_Num, 1);
		//XGpioPs_WritePin(&GpioPs, GPIO_Num, Data);
	}
}

/**
  *****************************************************
  * @brief	控制特定GPIO口的输出高/低电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输出"模式
  * @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
  * @param	Data		输出电平高低：0为低，1为高
  * @usage	//控制GPIO46输出高电平
  * 		GPIO_SetPort(46, 1);
  *****************************************************
**/
void PS_GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&GpioPs, GPIO_Num, Data);	//输出高/低电平，0为低，1为高
}

/**
  *****************************************************
  * @brief	读取特定GPIO口的输入电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输入"模式
  * @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
  * @return	返回采集的GPIO电平值，0为低，1为高
  * @usage	//采集GPIO46的电平，存放在state
  * 		state = GPIO_GetPort(46, state);
  *****************************************************
**/
uint8_t PS_GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&GpioPs, GPIO_Num);		//输出高/低电平，0为低，1为高
}

