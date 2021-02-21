#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t ax,ay,az,temp,gx,gy,gz,tempz = 0,tempy = 0, mx=0, diffY, diffZ, mn=0;
float DATA[6];
float deg;
boolean end_flag = false, start_flag = false, flag = false;
int counter;


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
//  if (flag){
//    tempy = ay;
//    tempz = az;
//  }
  ax=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  ay=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  az=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temp=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gx=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gy=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gz=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  az = az - 17000;
  diffZ = (az - tempz) / 320;
  diffY = (ay - tempy) / 250;
//  if (mx < diffZ){ mx = diffZ; }
//  if (mn > diffZ){ mn = diffZ; }
//  if (mx < diffY){ mx = diffY; }
//  if (mn > diffY){ mn = diffY; }
//  deg = atan2(diffZ, diffY);
//  Serial.print(gx);
//  Serial.print(", ");
//  Serial.print(gy);
//  Serial.print(", ");
//  Serial.println(gz);
//
  Serial.print("ax: ");
  Serial.print(ax);
  Serial.print(",   ay: ");
  Serial.print(ay);
  Serial.print(",  az: ");
  Serial.println(az);
//  Serial.print(",       counter: ");
//  Serial.print(counter);
//  Serial.print(",       flag: ");
//  Serial.println(flag);

  if ((ax > 15000) && (az < -15000)){ 
    if (counter == 0) 
    { 
      flag = true;
    } 
    if (flag)
    {
      counter = counter + 1;
    }
    if (counter > 20) 
    {
      Serial.println("start game!"); 
      flag = false, counter = 0;
    } 
  } else if ((az < -15000) && (ax < -15000)){     
    if (counter == 0)
    { 
      flag = true;
    } 
    if (flag) 
    {
      counter= counter + 1;
    }
    if (counter > 20) 
    {
      Serial.println("end game!"); 
      flag = false, counter = 0;
    } 
  } else { 
    counter = 0; 
    flag = false;
  } 
  

//  Serial.print("diffY: ");
//  Serial.print(diffY);
//  Serial.print(", diffZ: ");
//  Serial.println(diffZ);

  delay(100);

}
