#ifndef __AD9910V2_H__
#define __AD9910V2_H__

#include "main.h"
/**
 * @brief
 * 
 */


struct ad9910_reg{
	uint32_t CFR1;						//0x00
	uint32_t CFR2;						//0x01
	uint32_t CFR3;						//0x02
	uint32_t Aux_DAC_Control;			//0x03
	uint32_t IO_UPDATE;					//0x04	
	uint32_t FTW;						//0x07 
	uint16_t POW;						//0x08
	uint32_t ASF;						//0x09 
	uint32_t Multichip_Sync; 			//0x0A
	uint64_t Digital_Ramp_Limit; 		//0x0B
	uint64_t Digital_Ramp_Step_Size; 	//0x0C
	uint32_t Digital_Ramp_Rate; 		//0x0D
	uint64_t Profile_0; 				//0x0E
	uint64_t Profile_1; 				//0x0F
	uint64_t Profile_2; 				//0x10
	uint64_t Profile_3; 				//0x11
	uint64_t Profile_4; 				//0x12
	uint64_t Profile_5;					//0x13
	uint64_t Profile_6; 				//0x14
	uint64_t Profile_7; 				//0x15
	uint32_t RAM;						//0x16
};


enum REG_ADDRESS{	_CFR1 , _CFR2, _CFR3, _AUX_DAC_CONTROL, _IO_UPDATE, _FTW = 7, _POW,
					_ASF, _MULTICHIP_SYNC, _DIGITAL_RAMP_LIMIT, _DIGITAL_RAMP_STEP_SIZE,
					_DIGITAL_RAMP_RATE, _PROFILE_0, _PROFILE_1, _PROFILE_2,
					_PROFILE_3, _PROFILE_4, _PROFILE_5, _PROFILE_6, _PROFILE_7,
					_RAM
				};

enum REG_BYTE_SIZE{	_CFR1_SIZE = 4, _CFR2_SIZE = 4, _CFR3_SIZE = 4, _AUX_ADC_CONTROL_SIZE = 4,
					_IO_UPDATE_SIZE  = 4, _FTW_SIZE = 4, _POW_SIZE = 2, _ASF_SIZE = 4,
					_MULTICHIP_SYNC_SIZE = 4, _DIGITAL_RAMP_LIMIT_SIZE = 8, _DIGITAL_RAMP_STEP_SIZE_SIZE = 8,
					_DIGITAL_RAMP_RATE_SIZE = 4, _PROFILE_0_SIZE = 8, _PROFILE_1_SIZE = 8, _PROFILE_2_SIZE = 8,
					_PROFILE_3_SIZE = 8, _PROFILE_4_SIZE = 8, _PROFILE_5_SIZE = 8, _PROFILE_6_SIZE = 8,
					_PROFILE_7_SIZE = 8, _RAM_SIZE = 1024
				};

#define AD9910_RAM_DATA_SIZE 1024

#define PI 3.14159265358979323846
#define AD9910_Freq_Max 400000000

#define FRE_LOW_HIGH 0
#define FRE_HIGH_LOW 1

#define OUTSIDE_TRIG 0 //外部触发
#define INSIDE_TRIG 1 //主控触发

void AD9910_Init(void);
void AD9910_Reg_Write(enum REG_ADDRESS reg_address, enum REG_BYTE_SIZE reg_byte_size, uint64_t content);
void AD9910_RAM_Load_Profile_0(void);
void AD9910_IO_UPDATE(void);
void AD9910_Profile_Set(int profile);
void AD9910_AMP_Convert(uint32_t Amp);
void AD9910_Freq_Convert(uint64_t Freq);
void AD9910_Phi_Convert(uint64_t Phi);

void AD9910_ReInit_PFx(void);
void AD9910_RAM_Chrip_generate(uint64_t start_freq, uint64_t stop_freq, uint64_t sweep_time,uint64_t sys_time, uint8_t direction);
void AD9910_Phase_Clean(void);
void AD9910_RAMP_Chrip_generate(uint64_t start_freq, uint64_t stop_freq, uint64_t sweep_time,uint64_t sys_time, uint8_t direction,uint8_t trig_mode);
void AD9910_Ramp_Play(uint16_t t_total,uint16_t t_chrip);

void REG_REFRESH(void);
#endif
