/*****************************************
* L3G4200D Driver for Arduino
*****************************************/

#include "L3G4200D.h"
#include <Wire.h>

L3G4200D::L3G4200D()
{
	status = L3G4200D_OK;
	errorCode = L3G4200D_NO_ERROR;
}

void L3G4200D::Init(int address)
{
	devAddress = address;
	GetDevID();
	if (errorCode == L3G4200D_NO_ERROR)
		PowerOn();
	else
		status = L3G4200D_ERROR;
}

byte L3G4200D::GetDevID()
{
	byte name;
	ReadFrom(L3G4200D_WHO_AM_I, 1, &name);
	if (name == L3G4200D_ID)
	{
		errorCode = L3G4200D_NO_ERROR;
		return name;
	}
	else
	{
		errorCode = L3G4200D_NO_DEV;
		return 0x00;
	}
}

void L3G4200D::PowerOn()
{
	WriteTo(L3G4200D_CTRL_REG1, 0x0F);	// turn on all axis
}

void L3G4200D::PowerDown()
{
	SetRegisterBit(L3G4200D_CTRL_REG1, 3, false);
}

void L3G4200D::SleepDev()
{
	EnableXAxis(false);
	EnableYAxis(false);
	EnableZAxis(false);
}

void L3G4200D::ReadGyro(int* xyz)
{
	ReadGyro(xyz, xyz + 1, xyz + 2);
}

void L3G4200D::ReadGyro(int* x, int* y, int* z)
{
	ReadFrom(L3G4200D_OUT_X_L | L3G4200D_ADDR_AUTO_INCREMENT, L3G4200D_TO_READ, buff);
	
	*x = (((int)buff[1]) << 8) | buff[0];  
	*y = (((int)buff[3]) << 8) | buff[2];
	*z = (((int)buff[5]) << 8) | buff[4]; 
}

void L3G4200D::WriteTo(byte address, byte val)
{
	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.write(address);             // send register address
	Wire.write(val);                 // send value to write
	Wire.endTransmission();         // end transmission
}

void L3G4200D::ReadFrom(byte address, int num, byte buff[])
{
	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.write(address);       	      // sends address to read from
	Wire.endTransmission();         // end transmission

	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.requestFrom(devAddress, num);    // request 6 bytes from device

	int i = 0;
	while(Wire.available())         // device may send less than requested (abnormal)
	{
		buff[i] = Wire.read();    // receive a byte
		i++;
	}
	if(i != num)
	{
		status = L3G4200D_ERROR;
		errorCode = L3G4200D_READ_ERROR;
	}
	Wire.endTransmission();         // end transmission
}

void L3G4200D::EnableXAxis(bool state)
{
	if (state)
		SetRegisterBit(L3G4200D_CTRL_REG1, 0, 1);
	else
		SetRegisterBit(L3G4200D_CTRL_REG1, 0, 0);
}

void L3G4200D::EnableYAxis(bool state)
{
	if (state)
		SetRegisterBit(L3G4200D_CTRL_REG1, 1, 1);
	else
		SetRegisterBit(L3G4200D_CTRL_REG1, 1, 0);
}

void L3G4200D::EnableZAxis(bool state)
{
	if (state)
		SetRegisterBit(L3G4200D_CTRL_REG1, 2, 1);
	else
		SetRegisterBit(L3G4200D_CTRL_REG1, 2, 0);
}

void L3G4200D::SetRate(int rate)
{
	switch (rate)
	{
		case L3G4200D_RATE_100:
			SetRegisterBit(L3G4200D_CTRL_REG1, 7, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 6, 0);
			break;
		case L3G4200D_RATE_200:
			SetRegisterBit(L3G4200D_CTRL_REG1, 7, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 6, 1);
			break;
		case L3G4200D_RATE_400:
			SetRegisterBit(L3G4200D_CTRL_REG1, 7, 1);
			SetRegisterBit(L3G4200D_CTRL_REG1, 6, 0);
			break;
		case L3G4200D_RATE_800:
			SetRegisterBit(L3G4200D_CTRL_REG1, 7, 1);
			SetRegisterBit(L3G4200D_CTRL_REG1, 6, 1);
			break;
		default:
			SetRegisterBit(L3G4200D_CTRL_REG1, 7, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 6, 0);
			break;
	}
}

void L3G4200D::SetBandwidth(int width)
{
	switch (width)
	{
		case L3G4200D_BANDWIDTH_1:
			SetRegisterBit(L3G4200D_CTRL_REG1, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 4, 0);
			break;
		case L3G4200D_BANDWIDTH_2:
			SetRegisterBit(L3G4200D_CTRL_REG1, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 4, 1);
			break;
		case L3G4200D_BANDWIDTH_3:
			SetRegisterBit(L3G4200D_CTRL_REG1, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG1, 4, 0);
			break;
		case L3G4200D_BANDWIDTH_4:
			SetRegisterBit(L3G4200D_CTRL_REG1, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG1, 4, 1);
			break;
		default:
			SetRegisterBit(L3G4200D_CTRL_REG1, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG1, 4, 0);
			break;
	}
}

void L3G4200D::SetHighPassFilterMode(int mode)
{
	switch (mode)
	{
		case L3G4200D_NORMAL_RESET_MODE:
			SetRegisterBit(L3G4200D_CTRL_REG2, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG2, 4, 0);
			break;
		case L3G4200D_REFERENCE_MODE:
			SetRegisterBit(L3G4200D_CTRL_REG2, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG2, 4, 1);
			break;
		case L3G4200D_NORMAL_MODE:
			SetRegisterBit(L3G4200D_CTRL_REG2, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG2, 4, 0);
			break;
		case L3G4200D_AUTORESET:
			SetRegisterBit(L3G4200D_CTRL_REG2, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG2, 4, 1);
			break;
		default:
			SetRegisterBit(L3G4200D_CTRL_REG2, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG2, 4, 0);
			break;
	}
}

void L3G4200D::SetCutoffFreq(int cutoff)
{
	if (cutoff >= 0 && cutoff <= 10)
	{
		byte _b;
		ReadFrom(L3G4200D_CTRL_REG2, 1, &_b);
		_b &= ~(0x0F);
		_b |= 0x0F & cutoff;
		WriteTo(L3G4200D_CTRL_REG2, _b);
	}
	else
	{
		errorCode = L3G4200D_BAD_ARG;
	}
}

void L3G4200D::SetResolution(int res)
{
	switch (res)
	{
		case 250:
			SetRegisterBit(L3G4200D_CTRL_REG4, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 4, 0);
			break;
		case 500:
			SetRegisterBit(L3G4200D_CTRL_REG4, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 4, 1);
			break;
		case 1000:
			SetRegisterBit(L3G4200D_CTRL_REG4, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG4, 4, 0);
			break;
		case 2000:
			SetRegisterBit(L3G4200D_CTRL_REG4, 5, 1);
			SetRegisterBit(L3G4200D_CTRL_REG4, 4, 1);
			break;
		default:			
			SetRegisterBit(L3G4200D_CTRL_REG4, 5, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 4, 0);
			break;
	}
}

void L3G4200D::SetSelfTest(int mode)
{
	switch (mode)
	{
		case L3G4200D_SELF_TEST_NORMAL:		
			SetRegisterBit(L3G4200D_CTRL_REG4, 2, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 1, 0);
			break;
		case L3G4200D_SELF_TEST_POSITIVE:		
			SetRegisterBit(L3G4200D_CTRL_REG4, 2, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 1, 1);
			break;
		case L3G4200D_SELF_TEST_NEGATIVE:		
			SetRegisterBit(L3G4200D_CTRL_REG4, 2, 1);
			SetRegisterBit(L3G4200D_CTRL_REG4, 1, 1);
			break;
		default:		
			SetRegisterBit(L3G4200D_CTRL_REG4, 2, 0);
			SetRegisterBit(L3G4200D_CTRL_REG4, 1, 0);
			break;
	}
}

void L3G4200D::EnableHighPassFilter(bool enable)
{
	if (enable)
	{
		SetRegisterBit(L3G4200D_CTRL_REG5, 4, 1);
	}
	else
	{
		SetRegisterBit(L3G4200D_CTRL_REG5, 4, 0);
	}
}

void L3G4200D::EnableFIFO(bool enable)
{
	if (enable)
	{
		SetRegisterBit(L3G4200D_CTRL_REG5, 6, 1);
	}
	else
	{
		SetRegisterBit(L3G4200D_CTRL_REG5, 6, 0);
	}
}

void L3G4200D::SetFIFOMode(int mode)
{
	switch (mode)
	{
		case L3G4200D_BYPASS_MODE:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 0);
			break;
		case L3G4200D_FIFO_MODE:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 1);
			break;
		case L3G4200D_STREAM_MODE:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 1);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 0);
			break;
		case L3G4200D_STREAM_TO_FIFO:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 1);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 1);
			break;
		case L3G4200D_BYPASS_TO_STREAM:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 1);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 0);
			break;
		default:
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 7, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 6, 0);
			SetRegisterBit(L3G4200D_FIFO_CTRL_REG, 5, 0);
			break;
	}
}


void L3G4200D::SetRegisterBit(byte regAdress, int bitPos, bool state)
{
	byte _b;
	ReadFrom(regAdress, 1, &_b);
	if (state)
	{
		_b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
	}
	else
	{
		_b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
	}
	WriteTo(regAdress, _b);
}

bool L3G4200D::GetRegisterBit(byte regAdress, int bitPos)
{
	byte _b;
	ReadFrom(regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void L3G4200D::PrintAllRegister()
{
	byte _b;
	Serial.print("0x0F: ");
	ReadFrom(0x0F, 1, &_b);
	PrintByte(_b);
	Serial.println();
	int i;
	for (i=32; i<=56; i++)
	{
		Serial.print("0x");
		Serial.print(i, HEX);
		Serial.print(": ");
		ReadFrom(i, 1, &_b);
		PrintByte(_b);
		Serial.println();    
	}
}

void L3G4200D::PrintByte(byte val)
{
	int i;
	Serial.print("B");
	for(i=7; i>=0; i--)
	{
		Serial.print(val >> i & 1, BIN);
	}
}
