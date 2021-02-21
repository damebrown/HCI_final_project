#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050

float DATA[6];

void setup(){
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}


void loop(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  for(int i = 0; i < 5; i++)
  {
    Serial.print(Wire.read()<<8|Wire.read());
    Serial.print(",");
  }
  Serial.print(Wire.read()<<8|Wire.read());
  Serial.println();
  delay(50);
}
