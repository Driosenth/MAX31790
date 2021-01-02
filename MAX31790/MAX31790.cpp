/*************************************************
 *	
 *	MAX31790 Device Class
 *	Based on Maxim MAX31790 datasheet rev. 2, 01/2015
 *
 *	Latest update on 07/24/2016 by Joao Paulo Porto <jpjporto(at)gmail.com>
 *
 *	Changelog:
 *		v0.0.1 - initial release.
 *		v0.0.2 - Bug fixes.
 *		v0.1.0 - Code stable enough for me to trust it on my loop.
 *
 *************************************************/

#include <MAX31790.h>
#include "I2Cdev.h"


MAX31790::MAX31790(int address)
{
	devAddr = (address >> 1);

}

void MAX31790::initialize()
{
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 7, 0);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 6, 1);
	I2Cdev::readBit(devAddr, GLOBAL_CONFIG, 6, hold);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 5, 1);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 5, 1);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 3, 1);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 2, 0);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 1, 0);
	I2Cdev::writeBit(devAddr, GLOBAL_CONFIG, 0, 0);
	I2Cdev::writeByte(devAddr, PWM_FREQ, 0b00000000);
	I2Cdev::writeByte(devAddr, FAIL_OPT, 0b00100111);
	for(int i=1; i<=6; i++)
	{
		setPWMMode(i);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 7, 0);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 6, 1);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 5, 1);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 4, 0);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 3, 1);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 2, 0);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 1, 0);
		I2Cdev::writeBit(devAddr, FAN_CONFIG(i), 0, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(i), 4, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(i), 3, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(i), 2, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(i), 1, 0);
		I2Cdev::writeBit(devAddr, FAULT_MAX, i-1, 1);
		I2Cdev::writeBit(devAddr, FAN_FAULT, i-1, 0);
		
		setSR(i,4);
		MAX31790::setSPD(i,100);
		setRPM(i,1000);
	}
}

// Set Functions
void MAX31790::setRPM(uint8_t fan_num, uint16_t rpm)
{
	uint16_t tach_count = 60*getSR(fan_num)*8192/(NP[fan_num-1]*rpm)<<5;
	if(rpm == 0)
	{
		tach_count = 0b1111111111111111;
	}
	buffer[0] = tach_count >> 8;
	buffer[1] = tach_count;
	
	I2Cdev::writeBytes(devAddr, TACH_TARGET(fan_num), 2, buffer);
	
}

bool MAX31790::setPWM(uint8_t fan_num, uint16_t pwm)
{	
	if(fan_num > 6 || pwm > 511) return(false);
	uint16_t pwm_bit = pwm<<7;
	buffer[0] = pwm_bit >> 8;
	buffer[1] = pwm_bit;
	
	I2Cdev::writeBytes(devAddr, PWMOUT(fan_num), 2, buffer);
	return(true);
	
}

bool MAX31790::setSPD(uint8_t fan_num, uint8_t SPD)
{
	if(fan_num > 6 || SPD > 100) return(false);
	MAX31790::setPWM(fan_num, map(SPD,0,100,0,511));
	return(true);
}

bool MAX31790::setSR(uint8_t fan_num, uint8_t SR)
{
	if(fan_num > 6) return(false);
	if (SR ==  1){
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 0);
		
	}
	else if (SR ==  2){
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 1);
	}
	else if (SR ==  4){
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 1);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 0);
	}
	else if (SR ==  8){
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 1);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 1);
	}
	else if (SR == 16) {
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 1);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 0);
	}
	else if (SR == 32) {
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 7, 1);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 6, 0);
		I2Cdev::writeBit(devAddr, FAN_DYNAMICS(fan_num), 5, 1);
	}
	else return(false);
	return(true);	
}

bool MAX31790::setPPR(uint8_t fan_num, uint8_t _PPR)
{
	if(fan_num > 6) return(false);
	NP[fan_num-1]=_PPR;
	return (true);	
}

bool MAX31790::setPPM3(uint8_t fan_num, double _PPM3)
{
	if(fan_num > 6 || _PPM3 <= 0) return(false);
	PPM3[fan_num-1]=_PPM3;
	return (true);	
}

bool MAX31790::setPPL(uint8_t fan_num, double _PPL)
{
	if(fan_num > 6 || _PPL <= 0) return(false);
	PPM3[fan_num-1]=_PPL*1000;
	return (true);	
}

bool MAX31790::setPPG(uint8_t fan_num, double _PPG)
{
	if(fan_num > 6 || _PPG <= 0) return(false);
	PPM3[fan_num-1]=_PPG/0.003785411784;
	return (true);	
}

void MAX31790::setRPMMode(uint8_t fan_num)
{
	I2Cdev::writeBit(devAddr, FAN_CONFIG(fan_num), 7, 1);
}

void MAX31790::setPWMMode(uint8_t fan_num)
{
	I2Cdev::writeBit(devAddr, FAN_CONFIG(fan_num), 7, 0);
}



// Get Functions
uint16_t MAX31790::getPWM(uint8_t fan_num)
{
	buffer[0] = 0;
	buffer[1] = 0;
	//Reads two 
	I2Cdev::readBytes(devAddr, PWMOUT(fan_num), 2, buffer);
	uint16_t pwm_out = ((((int16_t)buffer[0]) << 8) | buffer[1]) >> 7;
	
	return (pwm_out);
}
uint16_t MAX31790::getSPD(uint8_t fan_num)
{
	return (map(MAX31790::getPWM(fan_num),0,511,0,100));
}

uint16_t MAX31790::getTAC(uint8_t fan_num){
	buffer[0] = 0;
	buffer[1] = 0;
	//Reads two 
	I2Cdev::readBytes(devAddr, TACH_COUNT(fan_num), 2, buffer);
	return(((((int16_t)buffer[0]) << 8) | buffer[1]) >> 5);
}

uint8_t MAX31790::getSR(uint8_t fan_num)
{
	buffer[0] = 0;
	I2Cdev::readBit(devAddr, FAN_DYNAMICS(fan_num), 7, buffer);
	bool D7 = buffer[0];
	I2Cdev::readBit(devAddr, FAN_DYNAMICS(fan_num), 6, buffer);
	bool D6 = buffer[0];
	I2Cdev::readBit(devAddr, FAN_DYNAMICS(fan_num), 5, buffer);
	bool D5 = buffer[0];
	if ( !D7 && !D6 && !D5) return(1);
	if ( !D7 && !D6 && D5) return(2);
	if ( !D7 && D6 && !D5) return(4);
	if ( !D7 && D6 && D5) return(8);
	if ( D7 && !D6 && !D5) return(16);
	return (32);
	;
}

uint8_t MAX31790::getPPR(uint8_t fan_num)
{	
	return (NP[fan_num-1]);
}

double MAX31790::getPPM3(uint8_t fan_num)
{	
	return (PPM3[fan_num-1]);
}

double MAX31790::getPPS(uint8_t fan_num)
{
	return (double(MAX31790::getSR(fan_num))*8192/double(MAX31790::getTAC(fan_num)));
}

double MAX31790::getPPM(uint8_t fan_num)
{
	return (getPPS(fan_num)*60);
}

double MAX31790::getPPH(uint8_t fan_num)
{
	return (getPPS(fan_num)*3600);
}

double MAX31790::getRPS(uint8_t fan_num)
{
	return (getPPS(fan_num)/getPPR(fan_num));
}

double MAX31790::getRPM(uint8_t fan_num)
{

	return (60*getPPS(fan_num)/(getPPR(fan_num)));
}

double MAX31790::getRPH(uint8_t fan_num)
{
	return (3600*getPPS(fan_num)/(getPPR(fan_num)));
}

double MAX31790::getM3PS(uint8_t fan_num)
{
	return (getPPS(fan_num)/getPPM3(fan_num));
}

double MAX31790::getM3PM(uint8_t fan_num)
{
	return (60*getPPS(fan_num)/getPPM3(fan_num));
}

double MAX31790::getM3PH(uint8_t fan_num)
{
	return (3600*getPPS(fan_num)/getPPM3(fan_num));
}

double MAX31790::getLPS(uint8_t fan_num)
{
	return (getPPS(fan_num)/(getPPM3(fan_num)/1000));
}

double MAX31790::getLPM(uint8_t fan_num)
{
	return (60*getPPS(fan_num)/(getPPM3(fan_num)/1000));
}

double MAX31790::getLPH(uint8_t fan_num)
{
	return (3600*getPPS(fan_num)/(getPPM3(fan_num)/1000));
}

double MAX31790::getGPS(uint8_t fan_num)
{
	return (getPPS(fan_num)/(getPPM3(fan_num)*0.003785411784));
}

double MAX31790::getGPM(uint8_t fan_num)
{
	return (60*getPPS(fan_num)/(getPPM3(fan_num)*0.003785411784));
}

double MAX31790::getGPH(uint8_t fan_num)
{
	return (3600*getPPS(fan_num)/(getPPM3(fan_num)*0.003785411784));
}


uint16_t MAX31790::getRPMTarget(uint8_t fan_num)
{
	buffer[0] = 0;
	buffer[1] = 0;
	//Reads two 
	I2Cdev::readBytes(devAddr, TACH_TARGET(fan_num), 2, buffer);
	uint16_t tach_out = ((((int16_t)buffer[0]) << 8) | buffer[1]) >> 5;
	
	//convert to RPM
	return 60*getSR(fan_num)*8192/(NP[fan_num-1]*tach_out);
}






