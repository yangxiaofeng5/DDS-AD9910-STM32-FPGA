#include "AD9910V2.h"
#include "main.h"
#include "spi.h"

uint8_t tran[9] = {0};
uint8_t ram_data[AD9910_RAM_DATA_SIZE] = {0};


//uint8_t ram_data[AD9910_RAM_DATA_SIZE] =
//{
//
//		0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00,
//		0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00,
//		0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00,
//		0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00, 0xff,0xfc,0x00,0x00,
//
//		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
//		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
//		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
//		0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
//
//};

struct ad9910_reg AD9910;

/**
 * @brief AD9910 reg parameter init 
 * 
 */
uint8_t cfr1[] = {0x00, 0x40, 0x00, 0x00};						  //0x00 cfr1 control from left to right ,from high bit to low bit
uint8_t cfr2[] = {0x01, 0x00, 0x00, 0x00};						  //0x01 cfr2 control
uint8_t cfr3[] = {0x05, 0x3D, 0x41, 0x32};						  //0x02 cfr3 control  40M閺夊牊鎸搁崣锟�  25闁稿﹤绉归。锟�  VC0=101   ICP=111: resister and capacitor is match with this parameter

uint8_t aux_dac[] = {0x00, 0x00, 0x00, 0x00};					  //0x03 aux_dac control
uint8_t io_update_rate[] = {0xff, 0xff, 0xff, 0xff};			  //0x04 io_update_rate control
uint8_t ftw[] = {0x00, 0x00, 0x00, 0x00};						  //0x07 ftw control
//uint8_t pow[] = {0x00, 0x00, 0x00, 0x00};  						//0x08 pow control
uint8_t asf[] = {0x00, 0x00, 0x00, 0x00};						  //0x09 asf control

uint8_t multichip_sync[] = {0x00, 0x00, 0x00, 0x00};			  //0x0a multichip_sync control
uint64_t digital_ramp_limit[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};		  //0x0b digital_ramp_limit control
uint64_t digital_ramp_step[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};		  //0x0c digital_ramp_step control
uint64_t digital_ramp_rate[] = {0x00, 0x00, 0x00, 0x00};		  						  //0x0d digital_ramp_rate control

// this profile must be set as uint64_t instand of uint8_t or the data will be wrong
// the data is from left to right ,from low bit to high bit
uint64_t profile0[] = {0x3f, 0xff, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42};  //0x0e profile0 control /signal mode/ 01闁瑰壊鍨扮粻娆撳箳瑜嶉崺锟� 23闁烩晠鏅茬紞鍛村箳瑜嶉崺锟� 4567濡増鍨瑰鑲╂嫬閸愵厾姣� 0x25,0x09,0x7b,0x42
uint64_t profile1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x0f profile1 control
uint64_t profile2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x10 profile2 control
uint64_t profile3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x11 profile3 control
uint64_t profile4[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x12 profile4 control
uint64_t profile5[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x13 profile5 control
uint64_t profile6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x14 profile6 control
uint64_t profile7[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x15 profile7 control

/**
 * @brief set the register闁挎稑顔慹gbytesize,and the data
 * 
 * @param reg_address 
 * @param reg_byte_size 
 * @param content 
 */
void AD9910_Reg_Write(enum REG_ADDRESS reg_address, enum REG_BYTE_SIZE reg_byte_size, uint64_t content)
{
    uint8_t tran[9];

    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, GPIO_PIN_RESET);
	//PS_GPIO_SetPort(AD9910_SPI_CS, 0);
    switch (reg_byte_size)
    {
    case 2: 
		tran[2] = (uint8_t) content;
		tran[1] = (uint8_t)(content >> 8);
		tran[0] = reg_address;
		HAL_SPI_Transmit(&hspi1, tran, 3, 10);
        break;

    case 4: 
		tran[4] = (uint8_t) content;
		tran[3] = (uint8_t)(content >> 8);
		tran[2] = (uint8_t)(content >> 16);
		tran[1] = (uint8_t)(content >> 24);
		tran[0] = reg_address;
		HAL_SPI_Transmit(&hspi1, tran, 5, 10);
        break;

    case 8: 
		tran[8] = (uint8_t) content;
		tran[7] = (uint8_t)(content >> 8);
		tran[6] = (uint8_t)(content >> 16);
		tran[5] = (uint8_t)(content >> 24);
		tran[4] = (uint8_t)(content >> 32);
		tran[3] = (uint8_t)(content >> 40);
		tran[2] = (uint8_t)(content >> 48);
		tran[1] = (uint8_t)(content >> 56);
		tran[0] = reg_address;
		HAL_SPI_Transmit(&hspi1, tran, 9, 10);
        break;
	case AD9910_RAM_DATA_SIZE:
		HAL_SPI_Transmit(&hspi1, ram_data, AD9910_RAM_DATA_SIZE+1, 10);
        break;
    default:
        break;
    }
    HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, GPIO_PIN_SET);
    //PS_GPIO_SetPort(AD9910_SPI_CS, 1);
}


/**
 * @brief only when the IO_update is used ,will the AD9910 can get the settinigs to develop the waveform
 * 
 */
void AD9910_IO_UPDATE()
{
	HAL_GPIO_WritePin(IO_UPDATE_GPIO_Port, IO_UPDATE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(IO_UPDATE_GPIO_Port, IO_UPDATE_Pin, GPIO_PIN_RESET);
}


/**
 * @brief select the profile 0-7
 * 
 * @param profile 
 */
void AD9910_Profile_Set(int profile){
	switch(profile){
		case 0:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_RESET);
			break;
		case 5:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_RESET);
			break;
		default: //profile 7
			HAL_GPIO_WritePin(PF2_GPIO_Port, PF2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF1_GPIO_Port, PF1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(PF0_GPIO_Port, PF0_Pin, GPIO_PIN_SET);
			break;
	}
}

/**
 * @brief the AD9910_Init() function initial with IO port and parameters
 * 
 */
void AD9910_Init()
{
    //IO_port part initialization

		GPIO_InitTypeDef GPIO_IO_UPDATE = {0};
		HAL_GPIO_WritePin(IO_UPDATE_GPIO_Port, IO_UPDATE_Pin, GPIO_PIN_RESET);

		GPIO_IO_UPDATE.Pin = IO_UPDATE_Pin;
		GPIO_IO_UPDATE.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_IO_UPDATE.Pull = GPIO_NOPULL;
		GPIO_IO_UPDATE.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(IO_UPDATE_GPIO_Port, &GPIO_IO_UPDATE);


    // this  initialization is important
	HAL_GPIO_WritePin(GPIOA, EXT_PWR_OVER_Pin, GPIO_PIN_RESET);
	AD9910_Profile_Set(0); //select pf0 reg
	HAL_GPIO_WritePin(GPIOB, DRCTL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, DRHOLD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, MASTER_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOA, MASTER_RESET_Pin, GPIO_PIN_RESET);

	AD9910.CFR1 = (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910.CFR2 = (cfr2[0]<<24) + (cfr2[1]<<16) + (cfr2[2]<<8) + cfr2[3];
	AD9910.CFR3 = (cfr3[0]<<24) + (cfr3[1]<<16) + (cfr3[2]<<8) + cfr3[3];
	//AD9910.Aux_DAC_Control = 0x00007F7F;
	//AD9910.IO_UPDATE = 0x00000002;
	//AD9910.FTW = 0x0;
	//AD9910.POW = 0x0;
	//AD9910.ASF = 0x0;
	//AD9910.Multichip_Sync = 0x0;
	AD9910.Digital_Ramp_Limit = 0x0;
	AD9910.Digital_Ramp_Step_Size = 0x0;
	AD9910.Digital_Ramp_Rate = 0x0;

	//AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];

	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_Reg_Write(_CFR2, _CFR2_SIZE, (uint64_t)AD9910.CFR2);
	AD9910_Reg_Write(_CFR3, _CFR3_SIZE, (uint64_t)AD9910.CFR3);
	//AD9910_Reg_Write(_AUX_DAC_CONTROL, _AUX_ADC_CONTROL_SIZE, (uint64_t)AD9910.Aux_DAC_Control);
	//AD9910_Reg_Write(_IO_UPDATE, _IO_UPDATE_SIZE, (uint64_t)AD9910.IO_UPDATE);
	//AD9910_Reg_Write(_FTW, _FTW_SIZE, (uint64_t)AD9910.FTW);
	//AD9910_Reg_Write(_POW, _POW_SIZE, (uint64_t)AD9910.POW);
	//AD9910_Reg_Write(_ASF, _ASF_SIZE, (uint64_t)AD9910.ASF);
	//AD9910_Reg_Write(_MULTICHIP_SYNC, _MULTICHIP_SYNC_SIZE, AD9910.Multichip_Sync);
	AD9910_Reg_Write(_DIGITAL_RAMP_LIMIT, _DIGITAL_RAMP_LIMIT_SIZE, AD9910.Digital_Ramp_Limit);
	AD9910_Reg_Write(_DIGITAL_RAMP_STEP_SIZE, _DIGITAL_RAMP_STEP_SIZE_SIZE, AD9910.Digital_Ramp_Step_Size);
	AD9910_Reg_Write(_DIGITAL_RAMP_RATE, _DIGITAL_RAMP_RATE_SIZE, AD9910.Digital_Ramp_Rate);
	//AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
	AD9910_IO_UPDATE();

}

/********************************single tone mode*****************************************/
/**
 * @brief AD9910 single tone mode Amplitude convert
 * 
 * @param Amp 
 */
void AD9910_AMP_Convert(uint32_t Amp)
{
	uint64_t Temp;
	Temp = (uint64_t)Amp * 28.4829; //calulate ASF : 25.20615385=(2^14)/650
	if (Temp > 0x3fff)
		Temp = 0x3fff;
	Temp &= 0x3fff;
	profile0[1] = (uint8_t)Temp;
	profile0[0] = (uint8_t)(Temp >> 8);

	AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];
	AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
	AD9910_IO_UPDATE();
}
/**
 * @brief AD9910 single tone mode Freq convert
 * @param Freq 
 */
void AD9910_Freq_Convert(uint64_t Freq)
{
	uint64_t Temp;
	Temp = (uint64_t)Freq * 4.294967296; //calulate FTW : 4.294967296=(2^32)/1000000000
	profile0[7] = (uint8_t)Temp;
	profile0[6] = (uint8_t)(Temp >> 8);
	profile0[5] = (uint8_t)(Temp >> 16);
	profile0[4] = (uint8_t)(Temp >> 24);

	AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];
	AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
	AD9910_IO_UPDATE();
}

/**
 * @brief AD9910 single tone mode闁挎冻鎷� Phase convert
 * 
 * @param Phi 
 */
void AD9910_Phi_Convert(uint64_t Phi)
{
	uint64_t Temp;
	Temp = (uint64_t)Phi * 182.04444444; //calulate POW : 182.04444444=(2^16)/2pi
	profile0[3] = (uint8_t)Temp;
	profile0[2] = (uint8_t)(Temp >> 8);


	AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];
	AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
	AD9910_IO_UPDATE();
}

/**
 * @brief AD9910 profile set select
 * 
 * @param PFx 
 */
void AD9910_RAM_Profile_set(uint8_t PFx,uint16_t Sample_interval)
{
    uint64_t Temp;
      Temp = ((1000000000/(unsigned long int)(Sample_interval)/64/4));        //1GHz/4 sample time 4*(1~65536)ns
    if(Temp > 0xffff)
            Temp = 0xffff;

	switch (PFx)
	{
	case 0:
		profile0[7] = 0x24;	// ram profile mode 0x04 0100
		profile0[6] = 0x00; // start ram adress[1:0] 7: 6
		profile0[5] = 0x00; // start ram adress[9:2] 15锛�8
	
		profile0[4] = 0xc0; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile0[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile0[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile0[1] = (uint8_t)(Temp>>8);  	//step rate[15:8]

		profile0[0] = 0x00; 
		AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];
		AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
		break;
	case 1:
		profile1[7] = 0x24;	// ram profile mode 0x04 0100
		profile1[6] = 0x00; // start ram adress[1:0] 7: 6
		profile1[5] = 0x4a; // start ram adress[9:2] 15锛�8

		profile1[4] = 0x01; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile1[3] = 0x95; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile1[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile1[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile1[0] = 0x00;
		AD9910.Profile_1 = (profile1[0]<<56) + (profile1[1]<<48) + (profile1[2]<<40) + (profile1[3]<<32) + (profile1[4]<<24) + (profile1[5]<<16) + (profile1[6]<<8) + profile1[7];
		AD9910_Reg_Write(_PROFILE_1, _PROFILE_1_SIZE, AD9910.Profile_1);
		break;
	case 2:
		profile2[7] = 0x24;	// ram profile mode 0x04 0100
		profile2[6] = 0x02; // start ram adress[1:0] 7: 6
		profile2[5] = 0x95; // start ram adress[9:2] 15锛�82

		profile2[4] = 0x03; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile2[3] = 0xe0; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile2[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile2[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile2[0] = 0x00;
		AD9910.Profile_2 = (profile2[0]<<56) + (profile2[1]<<48) + (profile2[2]<<40) + (profile2[3]<<32) + (profile2[4]<<24) + (profile2[5]<<16) + (profile2[6]<<8) + profile2[7];
		AD9910_Reg_Write(_PROFILE_2, _PROFILE_2_SIZE, AD9910.Profile_2);
		break;
	case 3:
		profile3[7] = 0x24;	// ram profile mode 0x04 0100
		profile3[6] = 0x00; // start ram adress[1:0] 7: 6
		profile3[5] = 0x60; // start ram adress[9:2] 15锛�8

		profile3[4] = 0x01; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile3[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile3[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile3[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile3[0] = 0x00;
		AD9910.Profile_3 = (profile3[0]<<56) + (profile3[1]<<48) + (profile3[2]<<40) + (profile3[3]<<32) + (profile3[4]<<24) + (profile3[5]<<16) + (profile3[6]<<8) + profile3[7];
		AD9910_Reg_Write(_PROFILE_3, _PROFILE_3_SIZE, AD9910.Profile_3);
		break;
	case 4:
		profile4[7] = 0x24;	// ram profile mode 0x04 0100
		profile4[6] = 0x00; // start ram adress[1:0] 7: 6
		profile4[5] = 0x00; // start ram adress[9:2] 15锛�8

		profile4[4] = 0xc0; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile4[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile4[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile4[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile4[0] = 0x00;
		AD9910.Profile_4 = (profile4[0]<<56) + (profile4[1]<<48) + (profile4[2]<<40) + (profile4[3]<<32) + (profile4[4]<<24) + (profile4[5]<<16) + (profile4[6]<<8) + profile4[7];
		AD9910_Reg_Write(_PROFILE_4, _PROFILE_4_SIZE, AD9910.Profile_4);
		break;
	case 5:
		profile5[7] = 0x24;	// ram profile mode 0x04 0100
		profile5[6] = 0x00; // start ram adress[1:0] 7: 6
		profile5[5] = 0x00; // start ram adress[9:2] 15锛�8

		profile5[4] = 0xc0; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile5[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile5[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile5[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile5[0] = 0x00;
		AD9910.Profile_5 = (profile5[0]<<56) + (profile5[1]<<48) + (profile5[2]<<40) + (profile5[3]<<32) + (profile5[4]<<24) + (profile5[5]<<16) + (profile5[6]<<8) + profile5[7];
		AD9910_Reg_Write(_PROFILE_5, _PROFILE_5_SIZE, AD9910.Profile_5);
		break;
	case 6:
		profile6[7] = 0x24;	// ram profile mode 0x04 0100
		profile6[6] = 0x00; // start ram adress[1:0] 7: 6
		profile6[5] = 0x00; // start ram adress[9:2] 15锛�8

		profile6[4] = 0xc0; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile6[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile6[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile6[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile6[0] = 0x00;
		AD9910.Profile_6 = (profile6[0]<<56) + (profile6[1]<<48) + (profile6[2]<<40) + (profile6[3]<<32) + (profile6[4]<<24) + (profile6[5]<<16) + (profile6[6]<<8) + profile6[7];
		AD9910_Reg_Write(_PROFILE_6, _PROFILE_6_SIZE, AD9910.Profile_6);
		break;
	case 7:
		profile7[7] = 0x24;	// ram profile mode 0x04 0100
		profile7[6] = 0x00; // start ram adress[1:0] 7: 6
		profile7[5] = 0x00; // start ram adress[9:2] 15锛�8

		profile7[4] = 0xc0; // end ram adress[1:0] 7: 6  0xFF 0xc0//
		profile7[3] = 0xff; // end ram adress[9:2] 15锛�8 0x1F 1024//

		profile7[2] = (uint8_t)Temp;  		//step rate[7:0]
		profile7[1] = (uint8_t)(Temp>>8);  //step rate[15:8]

		profile7[0] = 0x00;
		AD9910.Profile_7 = (profile7[0]<<56) + (profile7[1]<<48) + (profile7[2]<<40) + (profile7[3]<<32) + (profile7[4]<<24) + (profile7[5]<<16) + (profile7[6]<<8) + profile7[7];
		AD9910_Reg_Write(_PROFILE_7, _PROFILE_7_SIZE, AD9910.Profile_7);
		break;
	default:
		break;
	}
	AD9910_IO_UPDATE();

}

/**
 * @brief AD9910 RAM mode play
 * 
 * @param PFx 
 */
void AD9910_Ram_Play(uint8_t PFx)
{
	cfr1[0] = 0xc0; //if you want to use ram play, you must set bit 7 to 1,which is 1000 0000(0x80)
	cfr1[1] = 0x40;
	cfr1[2] = 0x00;
	cfr1[3] = 0x02; // only allow SDIO
    AD9910.CFR1 =  (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_Profile_Set(PFx);//choose which waveform you want to play

	AD9910_IO_UPDATE();    // this is use for stop spi transfer
}

void AD9910_RAMP_Chrip_generate(uint64_t start_freq, uint64_t stop_freq, uint64_t sweep_time,uint64_t sys_time, uint8_t direction)
{
	//sweep_time单位为ns，必须是4的整数倍
	//产生时间20us；产生频率步进次1us/4ns=250次；步进频率=(230M-170M)/250=240000
	uint64_t Step_count = 0;
	uint64_t Step_freq = 0;

	Step_count = sweep_time/sys_time;
	Step_freq = (stop_freq-start_freq)/Step_count; //5000 = 20us/4ns

	//RAMP enable, set freq
	cfr2[0] = 0x01; //set amp
	cfr2[1] = (direction == FRE_LOW_HIGH) ? 0x0a: 0x0c;// CFR2[19] 1;CFR2[21:20] 00;CFR2 [18:17] 00; oe是双边啁啾 oc是低到高单边增加 0a是高到低
	cfr2[2] = 0x00;
	cfr2[3] = 0x00; // only allow SDIO

    AD9910.CFR2 =  (cfr2[0]<<24) + (cfr2[1]<<16) + (cfr2[2]<<8) + cfr1[3];
	AD9910_Reg_Write(_CFR2, _CFR2_SIZE, (uint64_t)AD9910.CFR2);

	/*****/
	uint64_t Temp_1, Temp_2, Temp_freq_step_P, Temp_freq_step_N, Temp_time_step_P, Temp_time_step_N;
	Temp_1 = (uint64_t)start_freq*4.294967296; //(2^32)/1000000000 = 4.294967296
	if(stop_freq > 400000000)
		stop_freq = 400000000;
	Temp_2 = (uint64_t)stop_freq*4.294967296;
	if(Step_freq > 400000000)
		Step_freq = 400000000;
	Temp_freq_step_P = (uint64_t)Step_freq*4.294967296;
	Temp_freq_step_N = Temp_freq_step_P;

	Temp_time_step_P = (uint64_t)(sys_time*0.250); // 1Ghz/4 ns
	if(Temp_time_step_P > 0xffff)
		Temp_time_step_P = 0xffff;
	Temp_time_step_N = Temp_time_step_P;

	//
	digital_ramp_limit[7]=(uint8_t)Temp_1;
	digital_ramp_limit[6]=(uint8_t)(Temp_1>>8);
	digital_ramp_limit[5]=(uint8_t)(Temp_1>>16);
	digital_ramp_limit[4]=(uint8_t)(Temp_1>>24);
	digital_ramp_limit[3]=(uint8_t)Temp_2;
	digital_ramp_limit[2]=(uint8_t)(Temp_2>>8);
	digital_ramp_limit[1]=(uint8_t)(Temp_2>>16);
	digital_ramp_limit[0]=(uint8_t)(Temp_2>>24);
	//
	digital_ramp_step[7]=(uint8_t)Temp_freq_step_P;
	digital_ramp_step[6]=(uint8_t)(Temp_freq_step_P>>8);
	digital_ramp_step[5]=(uint8_t)(Temp_freq_step_P>>16);
	digital_ramp_step[4]=(uint8_t)(Temp_freq_step_P>>24);
	digital_ramp_step[3]=(uint8_t)Temp_freq_step_N;
	digital_ramp_step[2]=(uint8_t)(Temp_freq_step_N>>8);
	digital_ramp_step[1]=(uint8_t)(Temp_freq_step_N>>16);
	digital_ramp_step[0]=(uint8_t)(Temp_freq_step_N>>24);
	//
	digital_ramp_rate[3]=(uint8_t)Temp_time_step_P;
	digital_ramp_rate[2]=(uint8_t)(Temp_time_step_P>>8);
	digital_ramp_rate[1]=(uint8_t)Temp_time_step_N;
	digital_ramp_rate[0]=(uint8_t)(Temp_time_step_N>>8);

	//AD9910.Digital_Ramp_Limit = (Temp_2 << 32) + Temp_1;
	//AD9910.Digital_Ramp_Step_Size = (Temp_freq_step_N << 32) + Temp_freq_step_P;
	//AD9910.Digital_Ramp_Rate = (Temp_freq_step_N << 16) + Temp_freq_step_P;

	AD9910.Digital_Ramp_Limit = (digital_ramp_limit[0]<<56) + (digital_ramp_limit[1]<<48) + (digital_ramp_limit[2]<<40)
			 + (digital_ramp_limit[3]<<32) + (digital_ramp_limit[4]<<24) + (digital_ramp_limit[5]<<16) + (digital_ramp_limit[6]<<8) + digital_ramp_limit[7];
	AD9910.Digital_Ramp_Step_Size = (digital_ramp_step[0]<<56) + (digital_ramp_step[1]<<48) + (digital_ramp_step[2]<<40)
					 + (digital_ramp_step[3]<<32) + (digital_ramp_step[4]<<24) + (digital_ramp_step[5]<<16) + (digital_ramp_step[6]<<8) + digital_ramp_step[7];
	AD9910.Digital_Ramp_Rate = (digital_ramp_rate[0]<<24) + (digital_ramp_rate[1]<<16) + (digital_ramp_rate[2]<<8) + digital_ramp_rate[3];


	AD9910_Reg_Write(_DIGITAL_RAMP_LIMIT,_DIGITAL_RAMP_LIMIT_SIZE,AD9910.Digital_Ramp_Limit);
	AD9910_Reg_Write(_DIGITAL_RAMP_STEP_SIZE,_DIGITAL_RAMP_STEP_SIZE_SIZE,AD9910.Digital_Ramp_Step_Size);
	AD9910_Reg_Write(_DIGITAL_RAMP_RATE,_DIGITAL_RAMP_RATE_SIZE,AD9910.Digital_Ramp_Rate);
	//AD9910_IO_UPDATE();
	/********/

	// OSK port enable
	cfr1[0] = 0x00; // RAM  disable
	cfr1[1] = 0xc0; // anti-sinx filter
	cfr1[2] = 0x42;
	cfr1[3] = 0x00; //
    AD9910.CFR1 =  (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);

	//ASF
	asf[0] = 0x00; //ASF ramp [15:8]
	asf[1] = 0x01; //ASF ramp [7:0]   set time 4ns
	asf[2] = 0xff; //ASF scale [13:6]  0xff
	asf[3] = 0xfc; //ASF scale [5:0] 0xff  ASF_STEP[1:0] 11 8step
	AD9910.ASF = (asf[0]<<24) + (asf[1]<<16) + (asf[2]<<8) + asf[3];
	AD9910_Reg_Write(_ASF, _ASF_SIZE, (uint64_t)AD9910.ASF);

}
