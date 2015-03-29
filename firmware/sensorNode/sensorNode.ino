#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>  

#define SUCCESS 0
#define ERROR_INTERNAL -1

#define ID 0
#define OPEN 1
#define CLOSE 0

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define HUMIDITY_SENSOR_DIGITAL_PIN 3
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
 
MySensor gw;
DHT dht;
float lastTemp;
float lastHum;
boolean metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

int led = 4;

int readTemperature(float *Temperature)
{
    int error = 0;
    float temperature 0;
    
    temperature = dht.getTemperature();
    
    if (isnan(temperature)) {
        Serial.println("Failed reading temperature from DHT");
        error = ERROR_INTERNAL;
        goto error;
    } else {
#ifndef METRIC
        temperature = dht.toFahrenheit(temperature);
#endif
    }
    
    *temperature = temperature

cleanup:
    
    return error;
    
error:

    goto cleanup;
}

float readTemperature()
{
}
 
void setup() 
{ 
    pinMode(led, OUTPUT);     
    gw.begin();

    dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Humidity", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    gw.present(CHILD_ID_HUM, S_HUM);
    gw.present(CHILD_ID_TEMP, S_TEMP);

    metric = gw.getConfig().isMetric;
}
 
void loop()
{
    int error = 0;
    float temperature = 0;
    delay(dht.getMinimumSamplingPeriod());

    error = getTemperature(&temperature);
    if (error == SUCCESS) {
        gw.send(msgTemp.set(temperature, 1));
        Serial.print("T: ");
        Serial.println(temperature);
    }
    
    float humidity = dht.getHumidity();
    if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
    } else if (humidity != lastHum) {
      lastHum = humidity;
      gw.send(msgHum.set(humidity, 1));
      Serial.print("H: ");
      Serial.println(humidity);
    }
  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);               // wait for a second
}
