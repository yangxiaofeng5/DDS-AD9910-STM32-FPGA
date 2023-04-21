#include "AD9910V2.h"


uint8_t tran[8] = {0};
uint8_t ram_data[AD9910_RAM_DATA_SIZE] = {0};

struct ad9910_reg AD9910;

/**
 * @brief AD9910 reg parameter init 
 * 
 */
uint8_t cfr1[] = {0x00, 0x40, 0x00, 0x00};						  //0x00 cfr1 control，from left to right ,from low bit to high bit
uint8_t cfr2[] = {0x01, 0x00, 0x00, 0x00};						  //0x01 cfr2 control
uint8_t cfr3[] = {0x05, 0x3D, 0x41, 0x32};						  //0x02 cfr3 control  40M输入  25倍频  VC0=101   ICP=111: resister and capacitor is match with this parameter

uint8_t aux_dac[] = {0x00, 0x00, 0x00, 0x00};					  //0x03 aux_dac control
uint8_t io_update_rate[] = {0xff, 0xff, 0xff, 0xff};			  //0x04 io_update_rate control
uint8_t ftw[] = {0x00, 0x00, 0x00, 0x00};						  //0x07 ftw control
uint8_t pow[] = {0x00, 0x00, 0x00, 0x00};						  //0x08 pow control
uint8_t asf[] = {0x00, 0x00, 0x00, 0x00};						  //0x09 asf control

uint8_t multichip_sync[] = {0x00, 0x00, 0x00, 0x00};			  //0x0a multichip_sync control
uint64_t digital_ramp_limit[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};		  //0x0b digital_ramp_limit control
uint64_t digital_ramp_step[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};		  //0x0c digital_ramp_step control
uint64_t digital_ramp_rate[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00};		  //0x0d digital_ramp_rate control

// this profile must be set as uint64_t instand of uint8_t or the data will be wrong
// the data is from left to right ,from low bit to high bit
uint64_t profile0[] = {0x3f, 0xff, 0x00, 0x00, 0x25, 0x09, 0x7b, 0x42};  //0x0e profile0 control /signal mode/ 01振幅控制 23相位控制 4567频率调谐 0x25,0x09,0x7b,0x42
uint64_t profile1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x0f profile1 control
uint64_t profile2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x10 profile2 control
uint64_t profile3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x11 profile3 control
uint64_t profile4[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x12 profile4 control
uint64_t profile5[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x13 profile5 control
uint64_t profile6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x14 profile6 control
uint64_t profile7[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00}; //0x15 profile7 control

/**
 * @brief transfer 8 bit data,it's the bottom code,SPI
 * 
 * @param txdat 
 */
void AD9910_TXD_8BIT(uint8_t txdat)
{
    uint8_t i,sbt;
	sbt = 0x80;
	PS_GPIO_SetPort(AD9910_SPI_SCLK, 0);
	for (i = 0; i < 8; i++)
	{
		if ((txdat & sbt) == 0)
		PS_GPIO_SetPort(AD9910_SPI_SDIO, 0);
		else
		PS_GPIO_SetPort(AD9910_SPI_SDIO, 1);
		PS_GPIO_SetPort(AD9910_SPI_SCLK, 1);
		sbt = sbt >> 1;
		PS_GPIO_SetPort(AD9910_SPI_SCLK, 0);
	}
}

/**
 * @brief set the register，regbytesize,and the data
 * 
 * @param reg_address 
 * @param reg_byte_size 
 * @param content 
 */
void AD9910_Reg_Write(enum REG_ADDRESS reg_address, enum REG_BYTE_SIZE reg_byte_size, uint64_t content)
{
    uint8_t tran[9];

	PS_GPIO_SetPort(AD9910_SPI_CS, 0);
    switch (reg_byte_size)
    {
    case 2: 
        AD9910_TXD_8BIT(reg_address); 
        tran[1] = (uint8_t)content;
        tran[0] = (uint8_t)(content >> 8);
        for(int i = 0; i < 2 ; i++)
            AD9910_TXD_8BIT(tran[i]);
        break;

    case 4: 
        AD9910_TXD_8BIT(reg_address); 
		tran[3] = (uint8_t) content;
		tran[2] = (uint8_t)(content >> 8);
		tran[1] = (uint8_t)(content >> 16);
		tran[0] = (uint8_t)(content >> 24);
        for(int i = 0; i < 4 ; i++)
            AD9910_TXD_8BIT(tran[i]);
        break;

    case 8: 
        AD9910_TXD_8BIT(reg_address); 
		tran[7] = (uint8_t) content;
		tran[6] = (uint8_t)(content >> 8);
		tran[5] = (uint8_t)(content >> 16);
		tran[4] = (uint8_t)(content >> 24);
		tran[3] = (uint8_t)(content >> 32);
		tran[2] = (uint8_t)(content >> 40);
		tran[1] = (uint8_t)(content >> 48);
		tran[0] = (uint8_t)(content >> 56);
        for(int i = 0; i < 8 ; i++)
            AD9910_TXD_8BIT(tran[i]);
        break;
    default:
        break;
    }
    PS_GPIO_SetPort(AD9910_SPI_CS, 1);
}


/**
 * @brief only when the IO_update is used ,will the AD9910 can get the settinigs to develop the waveform
 * 
 */
void AD9910_IO_UPDATE()
{
    PS_GPIO_SetPort(AD9910_IO_UPATE, 1);
	PS_GPIO_SetPort(AD9910_IO_UPATE, 0);
}


/**
 * @brief select the profile 0-7
 * 
 * @param profile 
 */
void AD9910_Profile_Set(int profile){
	switch(profile){
		case 0:
			PS_GPIO_SetPort(AD9910_PF2, 0);
			PS_GPIO_SetPort(AD9910_PF1, 0);
			PS_GPIO_SetPort(AD9910_PF0, 0);
			break;
		case 1:
			PS_GPIO_SetPort(AD9910_PF2, 0);
			PS_GPIO_SetPort(AD9910_PF1, 0);
			PS_GPIO_SetPort(AD9910_PF0, 1);
			break;
		case 2:
			PS_GPIO_SetPort(AD9910_PF2, 0);
			PS_GPIO_SetPort(AD9910_PF1, 1);
			PS_GPIO_SetPort(AD9910_PF0, 0);
			break;
		case 3:
			PS_GPIO_SetPort(AD9910_PF2, 0);
			PS_GPIO_SetPort(AD9910_PF1, 1);
			PS_GPIO_SetPort(AD9910_PF0, 1);
			break;
		case 4:
			PS_GPIO_SetPort(AD9910_PF2, 1);
			PS_GPIO_SetPort(AD9910_PF1, 0);
			PS_GPIO_SetPort(AD9910_PF0, 0);
			break;
		case 5:
			PS_GPIO_SetPort(AD9910_PF2, 1);
			PS_GPIO_SetPort(AD9910_PF1, 0);
			PS_GPIO_SetPort(AD9910_PF0, 1);
			break;
		case 6:
			PS_GPIO_SetPort(AD9910_PF2, 1);
			PS_GPIO_SetPort(AD9910_PF1, 1);
			PS_GPIO_SetPort(AD9910_PF0, 0);
			break;
		default: //profile 7
			PS_GPIO_SetPort(AD9910_PF2, 1);
			PS_GPIO_SetPort(AD9910_PF1, 1);
			PS_GPIO_SetPort(AD9910_PF0, 1);
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
    PS_GPIO_SetMode(AD9910_EXT_PWR_OVER,OUTPUT, 0); //PWR
	PS_GPIO_SetMode(AD9910_DROVER,		INPUT,  0);
	PS_GPIO_SetMode(AD9910_DRCTL,		OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_DRHOLD, 		OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_IO_UPATE,	OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_MASTER_REST, OUTPUT, 0);
	//PS_GPIO_SetMode(AD9910_IO_RESET,	OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_OSK,			OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_PF0,			OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_PF1,			OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_PF2,  		OUTPUT, 0);
	PS_GPIO_SetMode(AD9910_RAM_SWP_OVER,INPUT, 	0); //RSO

	PS_GPIO_SetMode(AD9910_SPI_CS,      OUTPUT, 0);
    PS_GPIO_SetMode(AD9910_SPI_SDIO,    OUTPUT, 0);
    PS_GPIO_SetMode(AD9910_SPI_SCLK,    OUTPUT, 0);

    // this  initialization is important
	PS_GPIO_SetPort(AD9910_EXT_PWR_OVER, 0); //AD9910 PWR set 0
	AD9910_Profile_Set(0); //select pf0 reg
	PS_GPIO_SetPort(AD9910_DRCTL, 0);
	PS_GPIO_SetPort(AD9910_DRHOLD, 0);
	PS_GPIO_SetPort(AD9910_MASTER_REST, 1);
	usleep(5000);
	PS_GPIO_SetPort(AD9910_MASTER_REST, 0);

	AD9910.CFR1 = (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910.CFR2 = (cfr2[0]<<24) + (cfr2[1]<<16) + (cfr2[2]<<8) + cfr2[3];
	AD9910.CFR3 = (cfr3[0]<<24) + (cfr3[1]<<16) + (cfr3[2]<<8) + cfr3[3];
	//AD9910.Aux_DAC_Control = 0x00007F7F;
	//AD9910.IO_UPDATE = 0x00000002;
	//AD9910.FTW = 0x0;
	//AD9910.POW = 0x0;
	//AD9910.ASF = 0x0;
	//AD9910.Multichip_Sync = 0x0;
	//AD9910.Digital_Ramp_Limit = 0x0;
	//AD9910.Digital_Ramp_Step_Size = 0x0;
	//AD9910.Digital_Ramp_Rate = 0x0;

	AD9910.Profile_0 = (profile0[0]<<56) + (profile0[1]<<48) + (profile0[2]<<40) + (profile0[3]<<32) + (profile0[4]<<24) + (profile0[5]<<16) + (profile0[6]<<8) + profile0[7];

	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_Reg_Write(_CFR2, _CFR2_SIZE, (uint64_t)AD9910.CFR2);
	AD9910_Reg_Write(_CFR3, _CFR3_SIZE, (uint64_t)AD9910.CFR3);
	//AD9910_Reg_Write(_AUX_DAC_CONTROL, _AUX_ADC_CONTROL_SIZE, (uint64_t)AD9910.Aux_DAC_Control);
	//AD9910_Reg_Write(_IO_UPDATE, _IO_UPDATE_SIZE, (uint64_t)AD9910.IO_UPDATE);
	//AD9910_Reg_Write(_FTW, _FTW_SIZE, (uint64_t)AD9910.FTW);
	//AD9910_Reg_Write(_POW, _POW_SIZE, (uint64_t)AD9910.POW);
	//AD9910_Reg_Write(_ASF, _ASF_SIZE, (uint64_t)AD9910.ASF);
	//AD9910_Reg_Write(_MULTICHIP_SYNC, _MULTICHIP_SYNC_SIZE, AD9910.Multichip_Sync);
	//AD9910_Reg_Write(_DIGITAL_RAMP_LIMIT, _DIGITAL_RAMP_LIMIT_SIZE, AD9910.Digital_Ramp_Limit);
	//AD9910_Reg_Write(_DIGITAL_RAMP_STEP_SIZE, _DIGITAL_RAMP_STEP_SIZE_SIZE, AD9910.Digital_Ramp_Step_Size);
	//AD9910_Reg_Write(_DIGITAL_RAMP_RATE, _DIGITAL_RAMP_RATE_SIZE, AD9910.Digital_Ramp_Rate);
	AD9910_Reg_Write(_PROFILE_0, _PROFILE_0_SIZE, AD9910.Profile_0);
	AD9910_IO_UPDATE();

}

/********************************single tone mode*****************************************/
/**
 * @brief AD9910 single tone mode， Amplitude convert
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
 * @brief AD9910 single tone mode， Freq convert 
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
 * @brief AD9910 single tone mode， Phase convert
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



void AD9910_Ram_Play(uint8_t PFx)
{
	cfr1[0] = 0x80; //if you want to use ram play, you must set bit 7 to 1,which is 1000 0000(0x80) 
    AD9910.CFR1 =  (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_IO_UPDATE();    // this is use for stop spi transfer

	AD9910_Profile_Set(PFx);//choose which waveform you want to play
}

/**
 * @brief AD9910 chrip mode， chrip generate,the rate is according to ram_data's size(4096)
 * 
 * @param start_freq generate start freq      Hz
 * @param stop_freq generate stop freq        Hz
 * @param keep_time signal keep time		  Hz	
 */
void AD9910_Chrip_generate(uint8_t start_freq, uint8_t stop_freq, uint8_t keep_time)
{
	//AD9910.CFR1 = 0x40000002;
	cfr1[0] = 0x40;
	cfr1[1] = 0x00;
	cfr1[3] = 0x02;
    AD9910.CFR1 =  (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	
	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_IO_UPDATE();    // this is use for stop spi transfer

	AD9910_Profile_Set(0);

	for (int i = 0; i < AD9910_RAM_DATA_SIZE; i++)
	{
		ram_data[i] = (uint8_t)sin(2 * PI * (start_freq + (stop_freq - start_freq) * (i / AD9910_RAM_DATA_SIZE) / keep_time ) * (i / AD9910_RAM_DATA_SIZE)) * 0x3fff;
	}
	//Set RAM Enable bit
	//AD9910.CFR1 = 0xC0000002;
	cfr1[0] = 0xC0;
	AD9910.CFR1 =  (cfr1[0]<<24) + (cfr1[1]<<16) + (cfr1[2]<<8) + cfr1[3];
	AD9910_Reg_Write(_CFR1, _CFR1_SIZE, (uint64_t)AD9910.CFR1);
	AD9910_IO_UPDATE();
	
}