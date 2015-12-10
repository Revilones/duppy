#define _Digole_Serial_I2C_

#include <duppyTypes.h>

#include <PinChangeInt.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>

#include <MySensor.h>
#include <kSeries.h>
#include <DigoleSerial.h>
#include <SI7021.h>
#include <Adafruit_TSL2561_U.h>

#include "duppySensor.h"
#include "duppyMenu.h"
#include "duppyButton.h"

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_CO2 2
#define CHILD_ID_CO 3
#define CHILD_ID_LUX 4

#define STATUS_LED A2
#define LCD_BL 50

DigoleSerialDisp mydisp(&Wire,'\x27');
MySensor gw;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgCo2(CHILD_ID_CO2, V_GAS);
MyMessage msgCO(CHILD_ID_CO, V_GAS);
MyMessage msgLux(CHILD_ID_LUX, V_LIGHT);

SI7021 gDHT;
kSeries gK_30(3,4);
Adafruit_TSL2561_Unified gTSL = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW);
int CO_PIN = A0;

DuppyMenu menu(&mydisp);
DuppySensor sensors(&mydisp, &gw);

void displayReadings()
{
    sensors.sensorView();
    mydisp.setBackLight(LCD_BL);
    return;
}

void displaySensorId()
{
    int button = NO_BUTTON;
    long now = 0;

    mydisp.setPrintPos(1, 4, _TEXT_);
    mydisp.setFont(51);
    mydisp.print("Sensor ID ");
    mydisp.print(gw.getNodeId());

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
    hw_reboot();
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
    //mydisp.setI2CAddress(0x27);
    mydisp.clearScreen();
    mydisp.setBackLight(LCD_BL);

    gDHT.begin();

    //Initialize Lux Sensor
    if(gTSL.begin())
    {
        gTSL.enableAutoRange(true); /* Auto-gain ... switches automatically between 1x and 16x */
        gTSL.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
    }

    //Init CO Pin
    pinMode(CHILD_ID_CO, INPUT);

    //Initialize Gateway 
    gw.begin(NULL, AUTO/*Node Id*/, true/*Repeater Mode*/, AUTO/*Parent Node Id*/);

    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Bespin", "1.0");

    // Register all active sensors to gw
    int error = 0;
    double temperature = 0;
    double humidity = 0;
    double co2 = 0;
    double co = 0;
    double lux = 0;

    error = readHumidity((void*)&gDHT, &humidity);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_HUM, S_HUM);
        sensors.registerEntry("Hum: ", (void*)&gDHT, &msgHum, &readHumidity);
    }

    error = readTemperature((void*)&gDHT, &temperature);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_TEMP, S_TEMP);
        sensors.registerEntry("Tmp: ", (void*)&gDHT, &msgTemp, &readTemperature);
    }

    error = readCo2((void*)&gK_30, &co2);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_CO2, S_AIR_QUALITY);
        sensors.registerEntry("Co2: ", (void*)&gK_30, &msgCo2, &readCo2);
    }

    error = readCO((void*)&CO_PIN, &co);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_CO, S_AIR_QUALITY);
        sensors.registerEntry("CO: ", (void*)&CO_PIN, &msgCO, &readCO);
    }

    error = readLux((void*)&gTSL, &lux);
    if (error == SUCCESS) {
        gw.present(CHILD_ID_LUX, S_LIGHT_LEVEL);
        sensors.registerEntry("Lux: ", (void*)&gTSL, &msgLux, &readLux);
    }

    // Register Menu Items
    menu.registerEntry("Readings", &displayReadings);
    menu.registerEntry("Sensor ID", &displaySensorId);
    menu.registerEntry("Display Off", &displayOff);
    menu.registerEntry("Reset", &resetNode);
}
 
void loop()
{
    menu.runMenu();
}
