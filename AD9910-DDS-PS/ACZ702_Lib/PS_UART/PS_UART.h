#ifndef PS_UART_PS_UART_H_
#define PS_UART_PS_UART_H_

#include "COMMON.h"
#include "xuartps.h"

#define PS_UART0_IRQ_ID		XPAR_XUARTPS_0_INTR
#define PS_UART1_IRQ_ID		XPAR_XUARTPS_1_INTR

extern XUartPs UartPs1;	//UARTÉè±¸ÊµÀý

void PS_UART_Init(XUartPs *UartInstPtr,uint16_t DeviceId, uint8_t UART_Mode, uint32_t BaudRate);
void PS_UART_Intr_Init(XUartPs *UartInstPtr,uint16_t UartIntrId, uint8_t RecvTimeout, Xil_InterruptHandler Handler);
void PS_Uart_SendData(XUartPs *UartInstPtr,uint32_t data);
void PS_Uart_SendString(XUartPs *UartInstPtr, char *str);
void PS_Uart_RecvData(XUartPs *UartInstPtr, uint8_t *Buffer,uint32_t Len);

#endif /* PS_UART_PS_UART_H_ */
