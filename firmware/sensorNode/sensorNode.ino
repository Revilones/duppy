#define _Digole_Serial_I2C_

#include <PinChangeInt.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>

#include <duppyTypes.h>
#include <kSeries.h>
#include <MySensor.h>
#include <DHT.h>
#include <DigoleSerial.h>

#include "duppySensor.h"
#include "duppyMenu.h"
#include "duppyButton.h"

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_CO2 2

#define HUMIDITY_SENSOR_DIGITAL_PIN A3
#define STATUS_LED A2
#define LCD_BL 50

#define SENSOR_INTERVAL 60000 //Delay between sensor readings

DigoleSerialDisp mydisp(&Wire,'\x27');
DuppyMenu menu(&mydisp);
MySensor gw;
DHT gDHT;
kSeries gK_30(3,4);
boolean metric = true; 
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgCo2(CHILD_ID_CO2, V_GAS);

void displayReadings()
{
    int error = 0;
    long now = 0;
    int button = NO_BUTTON;
    float temperature = 0;
    float fHumidity = 0;
    double co2 = 0;

    while(1)
    {
        error = readHumidity(&gDHT, &fHumidity);
        if (error == SUCCESS) {
            gw.send(msgHum.set(fHumidity, 1));
        }
        
        error = readTemperature(&gDHT, metric, &temperature);
        if (error == SUCCESS) {
            gw.send(msgTemp.set(temperature, 1));
        }
        
        error = readCo2(gK_30, &co2);
        if (error == SUCCESS) {
            gw.send(msgCo2.set(co2, 1));
        }
        
        digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    
        mydisp.setFont(10);
        mydisp.setPrintPos(0, 1, _TEXT_);
        mydisp.nextTextLine();
        mydisp.setFont(51);
        mydisp.print("Hum: ");
        mydisp.print(fHumidity);
        mydisp.nextTextLine();
        mydisp.setFont(18);
        mydisp.nextTextLine();
        mydisp.setFont(51);
        mydisp.print("Tmp: ");
        mydisp.print(temperature);
        mydisp.nextTextLine();
        mydisp.setFont(18);
        mydisp.nextTextLine();
        mydisp.setFont(51);
        mydisp.print("Co2: ");
        mydisp.print(co2);
    
        now = millis();
        while((now + SENSOR_INTERVAL) > millis())
        {
            gw.process();
            button = getButtonPress();
            if (button != NO_BUTTON)
            {
                mydisp.setBackLight(LCD_BL);
                return;
            }
        }
    }
}

void displaySensorId()
{
    int button = NO_BUTTON;
    long now = 0;

    mydisp.setPrintPos(1, 4, _TEXT_);
    mydisp.setFont(51);
    mydisp.print("Sensor ID ");
    mydisp.print(gw.nc.nodeId);

    now = millis();
    while((now + 20000) > millis())
    {
        button = getButtonPress();
        if (button != NO_BUTTON)
        {
            return;
        }
    }

    mydisp.setBackLight(50);
    return;
}

void displayOff()
{
    mydisp.setBackLight(0);
    displayReadings();
}

void clearEEPROM()
{
    for (int i=0;i<512;i++) {
        EEPROM.write(i, 0xff);
    }
}
    
void resetNode()
{
    clearEEPROM();
    asm volatile ("  jmp 0");
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
    mydisp.setBackLight(LCD_BL);

    gDHT.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

    //Initialize Gateway 
    gw.begin(NULL, AUTO, true/*Repeater Mode*/, AUTO, RF24_PA_LEVEL, RF24_CHANNEL, RF24_DATARATE);

    gw.setPayloadSize(16);
    
    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Bespin", "1.0");

    // Register all active sensors to gw
    int error = 0;
    float temperature = 0;
    float fHumidity = 0;
    double co2 = 0;
    error = readHumidity(&gDHT, &fHumidity);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_HUM, S_HUM);
    }

    error = readTemperature(&gDHT, metric, &temperature);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_TEMP, S_TEMP);
    }

    error = readCo2(gK_30, &co2);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_CO2, S_AIR_QUALITY);
    }

    metric = gw.getConfig().isMetric;

    // Register Menu Items
    menu.registerEntry("Readings", &displayReadings);
    menu.registerEntry("Sensor ID", &displaySensorId);
    menu.registerEntry("Display Off", &displayOff);
    menu.registerEntry("Reset", &resetNode);
    menu.registerEntry("Test", &displayOff);
    menu.registerEntry("Test2", &displayOff);
    
    delay(1000);
}
 
void loop()
{
    menu.runMenu();
}
