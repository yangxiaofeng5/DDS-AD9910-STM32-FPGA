#ifndef ACZ702_LIB_COMMON_H_
#define ACZ702_LIB_COMMON_H_


//系统头文�??
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "platform.h"
#include "xil_printf.h"

//Xilinx头文�??
#include "xil_types.h"
#include "sleep.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xscutimer.h"
#include "xgpiops.h"

//ACZ702用户头文�??
//#include "PS_SPI.h"
#include "PS_GPIO.h"
//#include "AD9910.h"
#include "PS_UART.h"


//#include "AD9910V1.h"
#include "AD9910V2.h"
//用户宏定�??

/**
 * @brief 该引脚定义为AD9910的引脚定义，IN或�?�OUT是对AD9910来说�??
 * 
 */
#define   AD9910_EXT_PWR_OVER   (54 + 0 )	//PWR 外部省电模式 INPUT
#define   AD9910_DROVER         (54 + 1 )  //数字谐波结束 OUTPUT
#define   AD9910_DRCTL          (54 + 2 )  //数字谐波方向控制 INPUT
#define   AD9910_DRHOLD         (54 + 3 )  //数字谐波保持 INPUT

#define   AD9910_IO_RESET       (54 + 4 )   //输入输出复位 INPUT
#define   AD9910_MASTER_REST    (54 + 5 )	//主机复位 INPUT
#define   AD9910_IO_UPATE       (54 + 6 )	//同步信号（LVDS）INPUT

#define   AD9910_OSK            (54 + 7 )   //输出振幅键控 INPUT
#define   AD9910_PF0            (54 + 8 )   
#define   AD9910_PF1            (54 + 9 )
#define   AD9910_PF2            (54 + 10)

#define   AD9910_RAM_SWP_OVER   (54 + 11)   //RSO RAM扫描完成 OUTPUT

#define   AD9910_SPI_SDO		(54 + 12)
#define   AD9910_SPI_SDIO		(54 + 13)
#define   AD9910_SPI_SCLK		(54 + 14)
#define	  AD9910_SPI_CS			(54 + 15)   //SPI通讯 片�?�引�??


#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPU鏃堕敓鏂ゆ嫹棰戦敓鏂ゆ嫹(閿熸枻鎷蜂綅Hz)
#define INPUT		1
#define OUTPUT		0

#define IO_SET      1
#define IO_RESET    0


//用户声明引用


#endif /* ACZ702_LIB_COMMON_H_ */