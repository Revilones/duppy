#define _Digole_Serial_I2C_

#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <DigoleSerial.h>
#include <Wire.h>

#define DEBUG_PRINT(serial, msg) \
            Serial.print(msg);

#define DEBUG_PRINTLN(serial, msg) \
            Serial.println(msg);   \
            
#define DEBUG_PRINTLN(serial, msg) \
            Serial.println(msg);   \

#define SUCCESS 0
#define ERROR_INTERNAL -1

#define ID 0
#define OPEN 1
#define CLOSE 0

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define HUMIDITY_SENSOR_DIGITAL_PIN 3
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
 
DigoleSerialDisp mydisp(&Wire,'\x27');
MySensor gw;
DHT dht;
float lastTemp;
float lastHum;
boolean metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

void writeTemperatureSrv(MySensor gw, float temperature)
{
    gw.send(msgTemp.set(temperature, 1));
}

int readTemperature(float *Temperature)
{
    int error = 0;
    float temperature = 0;
    
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
    
    DEBUG_PRINT(Serial,"T: ");
    DEBUG_PRINTLN(Serial,temperature);
    
    *Temperature = temperature;

cleanup:
    
    return error;
    
error:

    goto cleanup;
}

void wrightHumidity(MySensor gw, float humidity)
{
    gw.send(msgHum.set(humidity, 1));
}

float readHumidity(float *Humidity)
{
    int error = 0;
    float humidity = 0;
    
    humidity = dht.getHumidity();
    if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
      error = ERROR_INTERNAL;
      goto error;
    } else {
      DEBUG_PRINT(Serial,"H: ");
      DEBUG_PRINTLN(Serial,humidity);
    }
    
    *Humidity = humidity;
    
    cleanup:
    
    return error;
    
error:

    goto cleanup;
}
 
void setup() 
{ 
    //Setup Display
    mydisp.begin();
    mydisp.displayConfig(1);
    mydisp.setI2CAddress(0x29);
    mydisp.clearScreen();
    mydisp.setBackLight(70);
       
    gw.begin();

    dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Humidity", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    gw.present(CHILD_ID_HUM, S_HUM);
    gw.present(CHILD_ID_TEMP, S_TEMP);

    metric = gw.getConfig().isMetric;
    
    mydisp.clearScreen();
    mydisp.setTextSize(4);
    mydisp.setTextColor(150, 0x0);
    mydisp.setCursor(10, 20);
    mydisp.println("Hum: ");
    mydisp.setCursor(10, 70);
    mydisp.println("Tmp: ");
}
 
void loop()
{
    int error = 0;
    float temperature = 0;
    float humidity = 0;
    delay(dht.getMinimumSamplingPeriod());

    error = readTemperature(&temperature);
    if (error == SUCCESS) {
        //gw.send(msgTemp.set(temperature, 1));
    }
    
    error = readHumidity(&humidity);
    if (error == SUCCESS) {
      //gw.send(msgHum.set(humidity, 1));
    }
    
    //mydisp.drawBox(80, 20, 60, 20, 60);
    //mydisp.drawBox(80, 60, 60, 20, 60);
    
    mydisp.setTextColor(75, 0);
    mydisp.setCursor(105, 20);
    mydisp.println((int)humidity);
    mydisp.setCursor(105, 70);
    mydisp.println((int)temperature);
    delay(5000);
}
