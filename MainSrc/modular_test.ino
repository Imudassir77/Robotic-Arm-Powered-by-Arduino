/*  
* Robotic-Arm-Powered-by-Arduino
* roboticArm.ino
* https://github.com/Patchyst
* Created by Patrick Story
*/
#include <Wire.h>
#include <RF24.h> // RF24 driver library; author: TMRh20;Rowberg

#define MPUADDR 0x68

#define CEPIN 7
#define CSPIN 6

struct accel_coordinates{
  float x;
  float y;
  float z;
};

/* MPU6050 Registers */
const uint8_t PWR_MGMT_1 = 0x6B;
const uint8_t ACCEL_CONFIG = 0x1c;
enum ACCEL_REGISTERS {ACCEL_XOUT_H=0x3B, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L};
/* MPU6050 Variables */
const uint8_t config_map = 0b00001000;
float LSB_per_g = 8192;

/*RF24 variables */
const byte addr[6] = "Recv1";
const int pipe = 0;

RF24 RF24Chip(CEPIN, CSPIN, RF24_SPI_SPEED);

int setup_accelerometer(uint8_t mpu_addr, uint8_t pwr_mgmt, uint8_t accel_config, uint8_t config_bitmap, bool startI2C){
  if(startI2C){
    Wire.begin();
  }
  
  Wire.beginTransmission(mpu_addr);
  Wire.write(pwr_mgmt);
  Wire.write(0x00);
  while(Wire.endTransmission(true) != 0x00);

  Wire.beginTransmission(mpu_addr);
  Wire.write(accel_config);
  Wire.write(config_bitmap);
  while(Wire.endTransmission(true) != 0x00);

  return 0;
}

int read_accel_data(uint8_t mpu_addr, bool end_trans, float lsb_per_g, struct accel_coordinates *raw_xyz){

 Wire.beginTransmission(mpu_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);
 Wire.requestFrom(mpu_addr, 6, true);
  
  raw_xyz->x = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  raw_xyz->y = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  raw_xyz->z = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  if(end_trans){
    Wire.endTransmission(true);
   }
  return 0;
}


void setup() {
  Serial.begin(9600);
  /* MPU6050 setup */
  while(setup_accelerometer(MPUADDR, PWR_MGMT_1, ACCEL_CONFIG, config_map, true) != 0);
  /* RF24 setup */
  if(!RF24Chip.begin()){
  }
  if(!RF24Chip.isChipConnected()){
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.startListening();
   RF24Chip.openReadingPipe(pipe, addr);
}

void loop() {
  int x = 1;
  struct accel_coordinates xyz; 
  read_accel_data(MPUADDR, false, LSB_per_g, &xyz);
  
 
 if(RF24Chip.available(pipe)){

  }
 

}
