#define _Digole_Serial_I2C_

#include <PinChangeInt.h>
#include <kSeries.h>
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

#define METRIC
            
#define SUCCESS 0
#define ERROR_INTERNAL -1

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_CO2 2
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

#define HUMIDITY_SENSOR_DIGITAL_PIN A3
#define STATUS_LED   A2
#define BUTTON_UP    5
#define BUTTON_DOWN  6
#define BUTTON_BACK  7
#define BUTTON_ENTER 8

DigoleSerialDisp mydisp(&Wire,'\x27');
MySensor gw;
DHT dht;
kSeries K_30(3,4);
float lastTemp;
float lastHum;
boolean metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgCo2(CHILD_ID_CO2, V_GAS);

void writeTemperatureSrv(MySensor gw, float temperature)
{
    gw.send(msgTemp.set(temperature, 1));
}

int readTemperature(float *Temperature)
{
    int error = SUCCESS;
    float temperature = 0;
    
    temperature = dht.getTemperature();
    
    if (isnan(temperature)) {
        //Serial.println("Failed reading temperature from DHT");
        error = ERROR_INTERNAL;
        goto error;
    } else {
#ifndef METRIC
        temperature = dht.toFahrenheit(temperature);
#endif
    }
    
    //DEBUG_PRINT(Serial,"T: ");
    //DEBUG_PRINTLN(Serial,temperature);
    
    *Temperature = temperature;

cleanup:
    
    return error;
    
error:

    goto cleanup;
}

void writeHumidity(MySensor gw, float humidity)
{
    gw.send(msgHum.set(humidity, 1));
}

int readHumidity(float *Humidity)
{
    int error = SUCCESS;
    float humidity = 0;
    
    humidity = dht.getHumidity();
    if (isnan(humidity)) {
      //Serial.println("Failed reading humidity from DHT");
      error = ERROR_INTERNAL;
      goto error;
    } else {
      //DEBUG_PRINT(Serial,"H: ");
      //DEBUG_PRINTLN(Serial,humidity);
    }
    
    *Humidity = humidity;
    
cleanup:
    
    return error;
    
error:

    goto cleanup;
}

int readCo2(double *Co2)
{
    int error = SUCCESS;
    double co2 = 0;
    
    co2 = K_30.getCO2('p');
    
    if (co2 < 0)
    {
        error = ERROR_INTERNAL;
        goto error;
    }
    
    *Co2 = co2;

cleanup:

    return error;
    
error:

    goto cleanup;
}

void setup() 
{ 
    //Delay for Second to allow DHT to initialize
    delay(1000);

    //Initialize Buttons
    pinMode(BUTTON_UP, INPUT); 
    pinMode(BUTTON_DOWN, INPUT); 
    pinMode(BUTTON_BACK, INPUT); 
    pinMode(BUTTON_ENTER, INPUT); 

    //Initialize Status led
    pinMode(STATUS_LED, OUTPUT);

    //Setup Display
    mydisp.begin();
    mydisp.displayConfig(1);
    mydisp.setI2CAddress(0x29);
    mydisp.clearScreen();
    mydisp.setBackLight(70);

    dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

    //Initialize Gateway 
    gw.begin(NULL, 5, false, AUTO, RF24_PA_LEVEL, RF24_CHANNEL, RF24_DATARATE);
    
    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Humidity", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    gw.present(CHILD_ID_HUM, S_HUM);
    gw.present(CHILD_ID_TEMP, S_TEMP);
    gw.present(CHILD_ID_CO2, S_AIR_QUALITY);

    metric = gw.getConfig().isMetric;
    
    //Initialize Display
    mydisp.clearScreen();
    mydisp.setTextSize(3);
    mydisp.setTextColor(252, 0x0);
    mydisp.setCursor(10, 10);
    mydisp.println("Hum: ");
    mydisp.setCursor(10, 50);
    mydisp.println("Tmp: ");
    mydisp.setCursor(10, 90);
    mydisp.println("Co2: ");
}
 
void loop()
{
    int error = 0;
    float temperature = 0;
    float humidity = 0;
    double co2 = 0;
    
    error = readTemperature(&temperature);
    if (error == SUCCESS) {
        gw.send(msgTemp.set(temperature, 1));
    }
    
    error = readHumidity(&humidity);
    if (error == SUCCESS) {
        gw.send(msgHum.set(humidity, 1));
    }
    
    // Get CO2 value from sensor
    error = readCo2(&co2);
    if (error == SUCCESS) {
        gw.send(msgCo2.set(co2, 1));
    }
    
    //mydisp.drawBox(80, 10, 60, 20, 0);
    //mydisp.drawBox(80, 50, 60, 20, 0);
    //mydisp.drawBox(80, 90, 80, 20, 0);

    mydisp.setTextColor(75, 0);
    mydisp.setCursor(85, 10);
    mydisp.println((int)humidity);
    mydisp.setCursor(85, 50);
    mydisp.println((int)temperature);
    mydisp.setCursor(85, 90);
    mydisp.println((int)co2);
    
    digitalWrite(STATUS_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(STATUS_LED, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);               // wait for a second
}
