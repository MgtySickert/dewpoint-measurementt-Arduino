#pragma once
#include <Wire.h>
#include <LiquidCrystal.h>

#define Addr 0x44   // address is 0x44
#define B_CONST 237.3
#define A_CONST 7.5

float exponent = 0;
float topright = 0;

void makeExponent(float temp)
{
  float up = A_CONST * temp;
  float down = B_CONST + temp;
  exponent = up / down;
}
 
void maketopright(float humidity)
{
  float high = pow(10, exponent);
  float first = humidity / 100;
  topright = log10((first * 6.1078 * high) / 6.1078);
}

float calculate(float cTemp, float humidity)
{
  makeExponent(cTemp);
  maketopright(humidity);
  float above = B_CONST * topright;  // above the fraction line
  float under = A_CONST - topright;  // under the fraction line
  float erg = above / under;
  return erg;
}

  const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  // Initialization of the LCD library

void setup()
{
  // Initialise I2C communication
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);

  lcd.begin(16, 4);
  // define columns and rows
  lcd.print("start");
  // Write "start" on the display
}

void loop() 
{
  unsigned int data[6];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);

  // Send measurement command
  Wire.write(0x2C);
  Wire.write(0x06);

  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data
  float cTemp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
  float humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
  float dewpoint = calculate(cTemp, humidity);

  // Output data to serial monitor
  Serial.print("humidity : ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("temperature : ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("dewpoint:");
  Serial.print(dewpoint);
  Serial.println(" C");
  Serial.println("");
  Serial.println("");
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("h:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("t:");
  lcd.print(cTemp);
  lcd.print("\337C");

  lcd.setCursor(9, 0);
  lcd.print("d:");
  lcd.print(dewpoint);
  lcd.print("\337C");

}
