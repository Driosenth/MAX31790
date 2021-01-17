#ifndef _MAX31790_h
#define _MAX31790_h
#define MAX_LIBRARY_VERSION	0.1.0


//Register Map
#define GLOBAL_CONFIG 		0x00
#define PWM_FREQ 			0x01
#define FAN_CONFIG(ch)		(0x02+(ch-1))
#define FAN_DYNAMICS(ch) 	(0x08+(ch-1))
#define TACH_COUNT(ch)		(0x18+(ch-1)*2)
#define PWM_DUTY_CYCLE(ch)	(0x30+(ch-1)*2)
#define PWMOUT(ch)			(0x40+(ch-1)*2)
#define TACH_TARGET(ch)		(0x50+(ch-1)*2)
#define FAN_FAULT			0x11
#define FAULT_MAX			0x13
#define FAIL_OPT			0x14



#define PWM_RATE_25		0b0000
#define PWM_RATE_30		0b0001
#define PWM_RATE_35		0b0010
#define PWM_RATE_100	0b0011
#define PWM_RATE_125	0b0100
#define PWM_RATE_149	0b0101
#define PWM_RATE_1250	0b0110
#define PWM_RATE_1470	0b0111
#define PWM_RATE_3570	0b1000
#define PWM_RATE_5000	0b1001
#define PWM_RATE_12500	0b1010
#define PWM_RATE_25000	0b1011



//Default Register Values
#define DEFAULT_FAN_CONFIG 	0b10001000
#define DEFAULT_FAN_DYN 	0b01001100



//#define NP 2 //number of TACH pulse per revolution

#include "I2Cdev.h"

class MAX31790
{
public:
	
	// Contructor
	MAX31790(int);
	
	void initialize();
	
	// Tune Function
	bool tuneSR(uint8_t, uint16_t);
	
	// Set functions
	bool setD(uint8_t, String, double);
	bool setPWM(uint8_t, uint16_t);
	bool setSPD(uint8_t, uint8_t);
	bool setSR(uint8_t, uint8_t);		// Set Speed Range (SR), [1,2,4,8,16,32]
	/*	
	
	|-----------------------------------------------------------------------------------|
	|                  Tach Output (2047 Max) vs Input and Speed Range                  |
	|-----------------------------------------------------------------------------------|
	|         Hz       |    4 |    8 |   16 |   32 |  64  |  128 |  256 |   512 |  1024 |
	|-----------------------------------------------------------------------------------|
	|   RPM (PPR = 2)  |  120 |  240 |  480 |  960 | 1920 | 3840 | 7680 | 15360 | 30720 |
	|-----------------------------------------------------------------------------------|
	| LPH (PPL = 1000) |   15 |   29 |   58 |  115 |  230 |  461 |  922 | 1843  | 3686  |
	|------------------|----------------------------------------------------------------|
	| Speed Range (SR) |//////|/////////////////////////////////////////////////////////|
	|-----------------------------------------------------------------------------------|
	|         1        | 2047 | 1024 |  512 |  256 |  128 |   64 |   32 |   16  |    8  | 
	|         2        | 2047 | 2047 | 1024 |  512 |  256 |  128 |   64 |   32  |   16  |
	|         4        | 2047 | 2047 | 2047 | 1024 |  512 |  256 |  128 |   64  |   32  |
	|         8        | 2047 | 2047 | 2047 | 2047 | 1024 |  512 |  256 |  128  |   64  |
	|        16        | 2047 | 2047 | 2047 | 2047 | 2047 | 1024 |  512 |  256  |  128  |
	|        32        | 2047 | 2047 | 2047 | 2047 | 2047 | 2047 | 1024 |  512  |  256  |
	|-----------------------------------------------------------------------------------|

	*/
	bool setAT(uint8_t, bool);
	bool setPPR(uint8_t, uint8_t);
	bool setPPM3(uint8_t, double);
	bool setPPL(uint8_t, double);
	bool setPPG(uint8_t, double);
	void setRPM(uint8_t, uint16_t);
	
	
	void setPWMMode(uint8_t);
	void setRPMMode(uint8_t);	
	
	// Get functions
	double getD(uint8_t, String);
	uint16_t getPWM(uint8_t);
	uint16_t getSPD(uint8_t);
	uint8_t  getSR (uint8_t);
	bool	getAT(uint8_t);
	uint8_t getPPR(uint8_t);
	double getPPM3(uint8_t);
	double getPPL(uint8_t);
	double getPPG(uint8_t);
	uint16_t getTAC(uint8_t);
	double getPPS(uint8_t);
	double getPPM(uint8_t);
	double getPPH(uint8_t);
	double getRPS(uint8_t);
	double getRPM(uint8_t);
	double getRPH(uint8_t);
	double getM3PS(uint8_t);
	double getM3PM(uint8_t);
	double getM3PH(uint8_t);
	double getLPS(uint8_t);
	double getLPM(uint8_t);
	double getLPH(uint8_t);
	double getGPS(uint8_t);
	double getGPM(uint8_t);
	double getGPH(uint8_t);
	
	uint16_t getRPMTarget(uint8_t);

	
private:

	//variables
	uint8_t devAddr;
	uint8_t buffer[2];
	uint8_t NP[6]={2,2,2,2,2,2};
	double PPM3[6]={1000000,1000000,1000000,1000000,1000000,1000000};
	uint8_t hold[2];
	bool autotune[6]={false,false,false,false,false,false};
	
	
	
	
	

};

#endif
