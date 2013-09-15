/*****************************************
* L3G4200D Driver for Arduino
*****************************************/

#include "Arduino.h"

#ifndef L3G4200D_h
#define L3G4200D_h

// L3G4200D address
#define L3G4200D_ADDR_ALT_HIGH 			0x69	// 1101001
#define L3G4200D_ADDR_ALT_LOW 			0x68	// 1101000
#define L3G4200D_ADDR_AUTO_INCREMENT	0x80	// 10000000

#define L3G4200D_WHO_AM_I      0x0F
#define L3G4200D_CTRL_REG1     0x20
#define L3G4200D_CTRL_REG2     0x21
#define L3G4200D_CTRL_REG3     0x22
#define L3G4200D_CTRL_REG4     0x23
#define L3G4200D_CTRL_REG5     0x24
#define L3G4200D_REFERENCE     0x25
#define L3G4200D_OUT_TEMP      0x26
#define L3G4200D_STATUS_REG    0x27

#define L3G4200D_OUT_X_L       0x28
#define L3G4200D_OUT_X_H       0x29
#define L3G4200D_OUT_Y_L       0x2A
#define L3G4200D_OUT_Y_H       0x2B
#define L3G4200D_OUT_Z_L       0x2C
#define L3G4200D_OUT_Z_H       0x2D

#define L3G4200D_FIFO_CTRL_REG 0x2E
#define L3G4200D_FIFO_SRC_REG  0x2F
#define L3G4200D_INT1_CFG      0x30
#define L3G4200D_INT1_SRC      0x31
#define L3G4200D_INT1_THS_XH   0x32
#define L3G4200D_INT1_THS_XL   0x33
#define L3G4200D_INT1_THS_YH   0x34
#define L3G4200D_INT1_THS_YL   0x35
#define L3G4200D_INT1_THS_ZH   0x36
#define L3G4200D_INT1_THS_ZL   0x37
#define L3G4200D_INT1_DURATION 0x38

// For CTRL_REG1
#define L3G4200D_RATE_100			1
#define	L3G4200D_RATE_200			2
#define	L3G4200D_RATE_400			3
#define	L3G4200D_RATE_800			4
#define	L3G4200D_BANDWIDTH_1		1	// refer to data sheet for bandwidth settings
#define	L3G4200D_BANDWIDTH_2		2
#define	L3G4200D_BANDWIDTH_3		3
#define	L3G4200D_BANDWIDTH_4		4

// For CTRL_REG2
#define L3G4200D_NORMAL_RESET_MODE 	1
#define L3G4200D_REFERENCE_MODE 	2
#define L3G4200D_NORMAL_MODE 		3
#define L3G4200D_AUTORESET			4

// For CTRL_REG4
#define L3G4200D_SELF_TEST_NORMAL	1
#define L3G4200D_SELF_TEST_POSITIVE	2
#define L3G4200D_SELF_TEST_NEGATIVE	3

// For FIFO_CTRL_REG
#define L3G4200D_BYPASS_MODE		1
#define L3G4200D_FIFO_MODE			2
#define L3G4200D_STREAM_MODE		3
#define L3G4200D_STREAM_TO_FIFO		4
#define L3G4200D_BYPASS_TO_STREAM	5

#define L3G4200D_OK		1	// no error
#define L3G4200D_ERROR	0	// error predent

#define L3G4200D_NO_ERROR	0	// initial state
#define L3G4200D_READ_ERROR	1	// problem reading
#define L3G4200D_BAD_ARG	2	// bad method argument
#define L3G4200D_NO_DEV		3

#define L3G4200D_ID			0xD3
#define L3G4200D_TO_READ 	6	// num of byte to be read

class L3G4200D
{
public:
	bool status;
	byte errorCode;
	
	L3G4200D();
	void Init(int address);
	byte GetDevID();
	void PowerOn();
	void PowerDown();
	void SleepDev();
	void ReadGyro(int* xyz);
	void ReadGyro(int* x, int* y, int* z);
	void WriteTo(byte address, byte val);
	
	// custom function
	void EnableXAxis(bool state);	// true to enable, false to disable
	void EnableYAxis(bool state);
	void EnableZAxis(bool state);
	void SetRate(int rate);
	void SetBandwidth(int width);
	void SetHighPassFilterMode(int mode);
	void SetCutoffFreq(int cutoff);			// refer to data sheet for freguency settings
	void SetResolution(int res);
	void SetSelfTest(int mode);
	void EnableHighPassFilter(bool enable);
	void EnableFIFO(bool enable);
	void SetFIFOMode(int mode);
	void PrintAllRegister();
	
private:
	byte buff[L3G4200D_TO_READ];
	int devAddress;
	
	void ReadFrom(byte address, int num, byte buff[]);
	void SetRegisterBit(byte regAdress, int bitPos, bool state);
	bool GetRegisterBit(byte regAdress, int bitPos);
	void PrintByte(byte val);
};
#endif
