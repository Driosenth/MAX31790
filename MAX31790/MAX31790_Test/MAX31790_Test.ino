//#include <i2c_t3.h>
#include "MAX31790.h"
//#include "I2Cdev.h"

//Pin definitions

int timeSRT = 0;
int timeSET = 0;
int timeGS  = 0;
int timeGE  = 0;
int tempSPD = 0;
int tempPWM = 0;
int tempTAC = 0;
int tempSR  = 0;
int tempPPR = 0;
double tempPPS = 0;
double tempRPM = 0;
double tempLPH = 0;

MAX31790 MAX31790(0x58);

void setup() {
  Wire.begin();
    delay(500);
  Serial.begin(115200);
    Serial.println("Serial Communication Established.");
    Serial.println("Initializing MAX31790");
    MAX31790.initialize();
    Serial.println("MAX31790 Initialized");
}
void loop() {
  for(int j = 511; j>=32;j=j-32){
    timeSRT=micros();
    for(int i=1; i<=6;i++){
      
      MAX31790.setPWM(i,j);
    }
    timeSET=micros();
    delay(5000);
    Serial.println(" | Ch  |  j  | SPD | PWM | TACH | SR | PPR |  PPS  |  RPM  | L/HR | TIME |"); 
    Serial.println(" |-----|-----|-----|-----|------|----|-----|-------|-------|------|------|"); 
    for(int i=1; i<=6;i++){
      Serial.print(" |  "); Serial.print(i); Serial.print("  | "); 
      if(j<100)Serial.print(" ");
      if(j<10)Serial.print(" ");
      Serial.print(j);
      Serial.print(" | "); 
      timeGS = micros();
      tempRPM = MAX31790.getRPM(i);
      tempPWM = MAX31790.getPWM(i);
      tempSPD = MAX31790.getSPD(i);
      tempTAC = MAX31790.getTAC(i);
      tempSR  = MAX31790.getSR(i);
      tempPPR = MAX31790.getPPR(i);
      tempPPS = MAX31790.getPPS(i);
      tempLPH = MAX31790.getLPH(i);
      timeGE = micros();
      if(tempSPD<100)Serial.print(" ");
      if(tempSPD<10) Serial.print(" ");
      Serial.print(tempSPD);
      Serial.print(" | "); 
      if(tempPWM<100)Serial.print(" ");
      if(tempPWM<10) Serial.print(" ");
      Serial.print(tempPWM);
      Serial.print(" | "); 
      if(tempTAC<1000)Serial.print(" ");
      if(tempTAC<100) Serial.print(" ");
      if(tempTAC<10)  Serial.print(" ");
      Serial.print(tempTAC);
      Serial.print(" | "); 
      if(tempSR<10)  Serial.print(" ");
      Serial.print(tempSR);
      Serial.print(" | "); 
      if(tempPPR<10)  Serial.print(" ");
      Serial.print(tempPPR);
      Serial.print("  | "); 
      if(tempPPS<100)Serial.print(" ");
      if(tempPPS<10) Serial.print(" ");
      if(tempPPS<1)  Serial.print(" ");
      Serial.print(tempPPS,1);
      Serial.print(" | "); 
      if(tempRPM<10000)Serial.print(" ");
      if(tempRPM<1000) Serial.print(" ");
      if(tempRPM<100)  Serial.print(" ");
      if(tempRPM<10)   Serial.print(" ");
      Serial.print(tempRPM,0);
      Serial.print(" | "); 
      if(tempLPH<1000) Serial.print(" ");
      if(tempLPH<100)  Serial.print(" ");
      if(tempLPH<10)   Serial.print(" ");
      Serial.print(tempLPH,0);
      Serial.print(" | "); 
      Serial.print(timeGE-timeGS);
      Serial.println(" |"); 
      
    }  
    Serial.println(" |-----|-----|-----|-----|------|----|-----|-------|-------|------|------|"); 
//    Serial.print  (" | "); Serial.println  (timeSET-timeSRT);
//    Serial.println(" |----------------------------------------------------------------|"); 
  }
}
