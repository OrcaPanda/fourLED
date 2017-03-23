/*
  GPIO_INPUTS
    PB_OUT_1  41  A5
    PB_OUT_2  40  A4
    PB_OUT_3  39  A3
    PB_OUT_4  38  A2
    PB_OUT_5  37  A1
    PB_OUT_6  36  A0
  GPIO_INTERUPTS
    PB_INT  1 D7
    IMU_INT 20  D0
  GPIO_OUTPUTS
    SPI_CS_R  31  D5
    SPI_CS_G  32  D13
    PB_EN 30  D10
  I2C_PORT
    MCU_SCL 18  D3
    MCU_SDA 19  D2
  SPI_PORT
    SPI_CLK 9 ICSP_3
    SPI_DIN 10  ICSP_4
    SPI_MISO  11  ICSP_1
 */
//#include <SPI.h>
#include "LedControl.h"

// GPIO INPUTS
#define PB_OUT_1 A5
#define PB_OUT_2 A4
#define PB_OUT_3 A3
#define PB_OUT_4 A2
#define PB_OUT_5 A1
#define PB_OUT_6 A0
// GPIO OUTPUTS
#define SPI_CS_R 5
#define SPI_CS_G 13
#define PB_EN 10
// GPIO INTERUPTS
#define PB_INT 7
#define IMU_INT 0
// I2C PORT
#define MCU_SCL 3
#define MCU_SDA 2
// SPI PORT
#define SPI_CLK SCK
#define SPI_DIN MOSI
#define SPI_MISO MISO

LedControl lcg = LedControl(SPI_DIN, SPI_CLK, SPI_CS_G, 4);

void setup() {
  // put your setup code here, to run once:
  pinMode(PB_OUT_1, INPUT);
  pinMode(PB_OUT_2, INPUT);
  pinMode(PB_OUT_3, INPUT);
  pinMode(PB_OUT_4, INPUT);
  pinMode(PB_OUT_5, INPUT);
  pinMode(PB_OUT_6, INPUT);
  pinMode(SPI_CS_R, OUTPUT);
  pinMode(SPI_CS_G, OUTPUT);
  pinMode(PB_EN, OUTPUT);

  int i;
  for(i = 0; i < lcg.getDeviceCount(); i++){
    lcg.shutdown(i, false);
  }
  lcg.setIntensity(0, 12);
  lcg.setIntensity(1, 12);
  lcg.setIntensity(2, 12);
  lcg.setIntensity(3, 12);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Begin ...");
  int addr, row, col;
  for(addr = 0; addr < 4; addr++){
    for(row = 0; row < 8; row++){
      for(col = 0; col < 8; col++){
        lcg.setLed(addr, row, col, true);
        delay(20);
      }
    }
  }

  while(1);
}
