#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050

int GESTURE = 10000;
int BLOCK = 0;

boolean flag = false;
int counter = 0;
boolean new_game = false;

int cur_player = 0;


float hist[10];
float cur_acc[3];
float prev_acc[3];
float avg;
int cur = 0;
int start = 0;

int minVal=265;
int maxVal=402;
int ang[3];
float x_ang, ax, ay, az;
int send_ang = 0;

void setup()
{
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}


void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  ax=Wire.read()<<8|Wire.read();
  ay=Wire.read()<<8|Wire.read();
  az=Wire.read()<<8|Wire.read();
  az = az - 17000;
  start_or_end_game();
  read_acc();
  running_avg();
  x_angle();
  send_data();
  delay(200);
}

void send_data()
{
  Serial.print(avg / 100);
  Serial.print(",");
  Serial.print((send_ang));
  Serial.print(",");
  if(new_game)
  {
    Serial.print(-2);
    cur_player = 0;
    new_game = false;
  }
  else if (BLOCK)
  {
    BLOCK--;
    Serial.print(-1);
  }
  else if(avg > GESTURE)
  {
    Serial.print(cur_player);
    cur_player = (cur_player + 1) % 2;
    BLOCK = 20;
  }
  else
  {
    Serial.print(-1);
    
  }
    Serial.print(",");
  Serial.println(cur_player);
}

void read_acc()
{
  for(int i = 0; i < 3; i++)
  {
    prev_acc[i] = cur_acc[i];
  }
  cur_acc[0] = ax;
  cur_acc[1] = ay;
  cur_acc[2] = az;
}

void running_avg()
{
  float res = 0;
  for(int i = 0; i < 3;i++)
  {
    res += (cur_acc[i] - prev_acc[i]) * (cur_acc[i] - prev_acc[i]);
  }
  res = sqrt(res);
  hist[cur] = res;
  cur = (cur + 1) % 10;
  if(cur == start)
  {
    start = (start + 1) % 10;
  }
  avg = 0;
  for(int i = 0; i < 10; i++)
  {
    avg += hist[(start + i) % 10];
  }
  avg = avg/10;
}

void x_angle()
{
  cur_acc[2] = cur_acc[2] + 17000;
  for(int i = 0; i < 3; i++)
  {
    ang[i] = map(cur_acc[i],minVal,maxVal,-90,90);
  }
  x_ang = RAD_TO_DEG * (atan2(-ang[1], -ang[2])+PI);
  x_ang = (int((x_ang + 11.0)) % 360);
  if (x_ang < 240)
  { 
    x_ang = x_ang * 1.05; 
  }
  else 
  {
    x_ang = x_ang * 0.92; 
  } 

  if (x_ang > 50 && x_ang < 170)
  {
    send_ang = (send_ang + 5) % 360;
  }
  else if (x_ang < 310 && x_ang > 160)
  {
    send_ang = (send_ang - 5) % 360;
  }
  cur_acc[2] = cur_acc[2] - 17000;
}

void start_or_end_game()
{
    if ((cur_acc[0] > 15000) && (cur_acc[2] < -15000)){ 
    if (counter == 0) 
    { 
      flag = true;
    } 
    if (flag)
    {
      counter = counter + 1;
    }
    if (counter > 15) 
    {
      new_game = true;
      flag = false, counter = 0;
    } 
  } else if ((cur_acc[2] < -15000) && (cur_acc[0] < -15000)){     
    if (counter == 0)
    { 
      flag = true;
    } 
    if (flag) 
    {
      counter= counter + 1;
    }
    if (counter > 15) 
    {
//      new_game = true;
// end game!
      flag = false, counter = 0;
    } 
  } else { 
    counter = 0; 
    flag = false;
  } 
}
