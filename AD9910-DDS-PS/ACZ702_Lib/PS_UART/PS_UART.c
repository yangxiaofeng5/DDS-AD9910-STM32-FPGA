/**
  *****************************************************************************
  * 						PS部分UART相关库
  *****************************************************************************
  *
  * @File   : PS_UART.c
  * @By     : Sun
  * @Version: V1.3
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "PS_UART.h"

XUartPs UartPs1;			//UART设备实例



/***********************************************************************************************************
 * @brief:	初始化PS_UART设备
 * @param:	UartInstPtr		UART对象
 * @param:	DeviceId		UART设备ID
 * @param:	UART_Mode		UART模式选择
 * @param:	BaudRate		设置波特率
 * 模式有以下几种：	XUARTPS_OPER_MODE_NORMAL		标准模式
 * 					XUARTPS_OPER_MODE_AUTO_ECHO		自动回显模式
 * 					XUARTPS_OPER_MODE_LOCAL_LOOP	本地环回模式
 * 					XUARTPS_OPER_MODE_REMOTE_LOOP	远程环回模式
 * 			//初始化UART设备UartPs1，设为标准模式，波特率115200
 * Sample:	PS_UART_Init(&UartPs1,XPAR_PS7_UART_1_DEVICE_ID, XUARTPS_OPER_MODE_NORMAL, 115200);
***********************************************************************************************************/
void PS_UART_Init(XUartPs *UartInstPtr,uint16_t DeviceId, uint8_t UART_Mode, uint32_t BaudRate)
{
	XUartPs_Config *Config;
	Config = XUartPs_LookupConfig(DeviceId);
	XUartPs_CfgInitialize(UartInstPtr, Config, Config->BaseAddress);

	//设置波特率
	XUartPs_SetBaudRate(UartInstPtr, BaudRate);

	//设置UART模式
	XUartPs_SetOperMode(UartInstPtr, UART_Mode);
}

/***********************************************************************************************************
 * @brief:	初始化PS UART中断（初始化PS UART中断前，需先初始化PS UART）
 * @param:	UartInstPtr	UART对象
 * @param:	UartIntrId		UART中断ID
 * @param:	RecvTimeout		接收超时设置（设置为n时，如果超过nx4个字符时间仍未接收到数据，则进入超时中断；
 * 							设置为0时禁用该功能）
 * @return:	无
 * Sample:	PS_UART_Intr_Init(&UartPs1, PS_UART1_IRQ_ID, 8, (void *)PS_UART1_IRQ_Handler);
 * 			//初始化PS UART中断，UART对象为UartPs，中断ID为UART1_IRQ_ID，接收超时32字符
***********************************************************************************************************/
void PS_UART_Intr_Init(XUartPs *UartInstPtr,uint16_t UartIntrId, uint8_t RecvTimeout, Xil_InterruptHandler Handler)
{
	uint32_t IntrMask;

	//Set_ScuGic_Link(UartIntrId, 0xA8, HIGH_Level_Sensitive,
	//		(Xil_ExceptionHandler)XUartPs_InterruptHandler, (void *)UartInstPtr);

	//为 UART 设置处理程序，当数据发送和接收时，该处理程序将从中断上下文中调用，
	//指定一个指向 UART 驱动程序实例的指针作为回调引用，以便处理程序能够访问实例数据
	XUartPs_SetHandler(UartInstPtr, (XUartPs_Handler)Handler, UartInstPtr);

	//配置UART的中断模式
	IntrMask = XUARTPS_IXR_TOUT |	XUARTPS_IXR_PARITY | XUARTPS_IXR_FRAMING |
		XUARTPS_IXR_OVER | XUARTPS_IXR_TXEMPTY | XUARTPS_IXR_RXFULL | XUARTPS_IXR_RXOVR;
	XUartPs_SetInterruptMask(UartInstPtr, IntrMask);

	//Fifo阈值设置为1
	XUartPs_SetFifoThreshold(&UartPs1, 1);

	//接收超时设置
	XUartPs_SetRecvTimeout(UartInstPtr, RecvTimeout);

}

/***********************************************************************************************************
 * @brief:	使用PS UART发送数据，数据需在0~65535之间
 * @param:	UartInstPtr		UART对象
 * @param:	data			数据
 * @return:	无
 * Sample:	PS_Uart_SendData(&UartPs,12345); //发送数据12345
***********************************************************************************************************/
void PS_Uart_SendData(XUartPs *UartInstPtr,uint32_t data)
{
	uint8_t SendData[2];
	int data_H,data_L;
	data_H = data>>8;
	data_L = data&0x00ff;
	if(data_H == 0)//如果高位为0
	{
		SendData[0] = data;
		XUartPs_Send(UartInstPtr, SendData, 1);
	}
	else
	{
		SendData[0] = data_H;
		SendData[1] = data_L;
		XUartPs_Send(UartInstPtr, SendData, 2);
	}
}

/***********************************************************************************************************
 * @brief:	使用PS UART发送字符串
 * @param:	UartInstPtr		UART对象
 * @param:	str				字符串地址
 * @return:	无
 * Sample:	PS_Uart_SendString(&UartPs,"1234567"); //发送了7个字节
***********************************************************************************************************/
void PS_Uart_SendString(XUartPs *UartInstPtr, char *str)
{
	XUartPs_Send(UartInstPtr, (uint8_t *)str, strlen(str));
}

/***********************************************************************************************************
 * @brief:	使用PS UART接收数据
 * @param:	UartInstPtr		UART对象
 * @param:	Buffer			接收数据存储区
 * @param:	Len				接收数据的长度
 * @return:	无
 * Sample:	PS_Uart_RecvData(&UartPs, RecvBuffer,2);接收两个数据，并将数据存入RecvBuffer数组
***********************************************************************************************************/
void PS_Uart_RecvData(XUartPs *UartInstPtr, uint8_t *Buffer,uint32_t Len)
{
	XUartPs_Recv(UartInstPtr, Buffer, Len);
}

