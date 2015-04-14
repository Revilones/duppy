#include "kSeries.h"
 
// Create K30 instance on pin 6 & 7
kSeries K_30(3,4);
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  // Get CO2 value from sensor
  double co2 = K_30.getCO2('p');
 
  // Print the value on Serial
  Serial.print("Co2 ppm = ");
  Serial.println(co2);
 
  // Wait 2 seconds
  delay(2000);
}