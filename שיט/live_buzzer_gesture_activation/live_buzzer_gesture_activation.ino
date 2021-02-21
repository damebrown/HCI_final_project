// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float C = 261.63;
float D = 293.66;
float E = 329.63;
float F = 349.23;
float G = 392.00;
float A = 440.00;
float B = 493.88;
float notes[8] = {C, D, E, F, G, A, B, 2 * C};
int Pin = 2;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

int convertBinToDec(boolean Bin[]) {
  int ReturnInt = 0;
  for (int i = 0; i < 3; i++) {
    if (Bin[2 - i]) {
      ReturnInt += (1<<i);
    }
  }
  return ReturnInt;
}

float getNote(float Acx, float AcY, float AcZ){
  boolean arr[3] = {Acx > 0, AcY > 0, AcZ > 0};
  int index = convertBinToDec(arr);
  return notes[index];
}

void plot(float AcX, float AcY, float AcZ, float norm){
  Serial.print(AcX);  
  Serial.print(",");
  Serial.print(AcY);
  Serial.print(",");
  Serial.print(AcZ);
  Serial.print(",");
  Serial.println(norm);
}

float norma(float AcX, float AcY, float AcZ){
  return sqrt(sq(AcX) + sq(AcY) + sq(AcZ));
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  float note = getNote(AcX, AcY, AcZ);
  float norm = norma(AcX, AcY, AcZ);
//  plot(AcX, AcY, AcZ, note);
  Serial.println(norm);
//  plot(AcX, AcY, AcZ, norm);
  if (norm > 24000){
    if (norm < 30000){
      tone(Pin, note, 500);
    } else if (norm < 40000){
      tone(Pin, 2 * note,500);
    }
    else {
      tone(Pin, 3 * note, 500);
    }
  }
  delay(500);
}
