#include <Wire.h>
#define Addr 0x44  // address is 0x44

void setup()
{
  Wire.begin();
  // Initialise I2C communication
  Serial.begin(9600);
  // Initialise serial communication, set baud rate = 9600
  delay(300);
}

void loop()

{
  unsigned int data[6];
  
  // Start I2C Transmission
   Wire.beginTransmission(Addr);
  // Send measurement command
  Wire.write(0x2C);
  Wire.write(0x06);
  Wire.endTransmission();
  // Stop I2C transmission
  delay(500);
  Wire.requestFrom(Addr, 6);
  // Request 6 bytes of data
  if (Wire.available() == 6)
  // Read 6 bytes of data
 
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  float cTemp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
  float humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
  // Convert the data
  
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature : ");
  Serial.print(cTemp);
  Serial.println(" C");
  delay(1000);
  // Output data to serial monitor
}
