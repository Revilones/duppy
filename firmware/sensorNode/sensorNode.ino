#define _Digole_Serial_I2C_

#include <PinChangeInt.h>
#include <kSeries.h>
#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>
#include <DigoleSerial.h>
#include <Wire.h>

#define SUCCESS 0
#define ERROR_INTERNAL -1

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_CO2 2

#define HUMIDITY_SENSOR_DIGITAL_PIN A3
#define STATUS_LED A2
#define LCD_BL 50

#define BUTTON_UP    5
#define BUTTON_DOWN  6
#define BUTTON_BACK  7
#define BUTTON_ENTER 8
#define NO_BUTTON -1

#define READINGS      0
#define SENSOR_ID     1
#define DISPLAY_OFF   2

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
        error = ERROR_INTERNAL;
        goto error;
    } else {
	    if(metric == true)
	    {
            temperature = dht.toFahrenheit(temperature);
	    }
    }
    
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
      error = ERROR_INTERNAL;
      goto error;
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

typedef struct button_state {
    long lastDebounceTime;
    boolean lastBounceState;
    boolean lastState;
    boolean currentState;
}; 

long debounceDelay = 10;

int debounceButton(int buttonPin, struct button_state *pState)
{
    int error = SUCCESS;
    boolean reading = digitalRead(buttonPin);
  
    if (reading != pState->lastBounceState)
    {
        pState->lastDebounceTime = millis();
    } 
    
    pState->lastBounceState = reading;

    if ((millis() - pState->lastDebounceTime) > debounceDelay)
    {
        pState->currentState = reading;
    }
    else
    {
        error = ERROR_INTERNAL;
    }

    return error;
}

struct button_state upState = {0};
struct button_state downState = {0};
struct button_state backState = {0};
struct button_state enterState = {0};

int getButtonPress()
{
    int button = NO_BUTTON;

    if (debounceButton(BUTTON_UP, &upState) == SUCCESS &&
        upState.currentState != upState.lastState)
    {
        if (upState.currentState == LOW)
        {
            button = BUTTON_UP;
        }
        upState.lastState = upState.currentState;
    } 
    else if(debounceButton(BUTTON_DOWN, &downState) == SUCCESS &&
        downState.currentState != downState.lastState)
    {
        if (downState.currentState == LOW)
        {
            button = BUTTON_DOWN;
        }
        downState.lastState = downState.currentState;
    } 
    else if (debounceButton(BUTTON_BACK, &backState) == SUCCESS &&
        backState.currentState != backState.lastState)
    {
        if (backState.currentState == LOW)
        {
            button = BUTTON_BACK;
        }
        backState.lastState = backState.currentState;
    } 
    else if (debounceButton(BUTTON_ENTER, &enterState) == SUCCESS &&
        enterState.currentState != enterState.lastState)
    {
        if (enterState.currentState == LOW)
        {
            button = BUTTON_ENTER;
        }
        enterState.lastState = enterState.currentState;
    } 
    else
    {
        button = NO_BUTTON;
    }

    return button;
}

void changeCursor(int prevCursor, int cursor)
{
    mydisp.setMode('~');
    mydisp.drawBox(0, 40*prevCursor, 160, 30);
    mydisp.drawBox(0, 40*cursor, 160, 30);
}

void displayMenu()
{
    mydisp.setPrintPos(0, 1, _TEXT_);
    mydisp.setFont(51);
    mydisp.print("Readings");
    mydisp.nextTextLine();
    mydisp.setFont(18);
    mydisp.nextTextLine();
    mydisp.setFont(51);
    mydisp.print("Sensor ID");
    mydisp.nextTextLine();
    mydisp.setFont(18);
    mydisp.nextTextLine();
    mydisp.setFont(51);
    mydisp.print("Display Off");

    mydisp.setMode('~');
    mydisp.drawBox(0, 0, 160, 30);
}

void displayReadings()
{
    int error = 0;
    long now = 0;
    int button = NO_BUTTON;
    float temperature = 0;
    float humidity = 0;
    double co2 = 0;

    while(1)
    {
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
        
        digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    
        mydisp.setFont(10);
        mydisp.setPrintPos(0, 1, _TEXT_);
        mydisp.nextTextLine();
        mydisp.setFont(51);
        mydisp.print("Hum: ");
        mydisp.print(humidity);
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
        while((now + 6000) > millis())
        {
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

void menu()
{
    int button = NO_BUTTON;
    int prevCursor = 0;
    int cursor = 0;
    long now = 0;

    mydisp.clearScreen();
    displayMenu();

    now = millis();

    while(1)
    {
        if (millis() > (now + 10000))
        {
            mydisp.clearScreen();
            displayReadings();
            goto resetMenu;
        }	

        button = getButtonPress();
        if (button != NO_BUTTON)
        {
            switch(button)
            {
                case BUTTON_UP:
                    if (cursor > 0)
                    {
                        prevCursor = cursor;
                        cursor--;
                        changeCursor(prevCursor, cursor);
                    }
                    break;
                case BUTTON_DOWN:
                    if (cursor < 2)
                    {
                        prevCursor = cursor;
                        cursor++;
                        changeCursor(prevCursor, cursor);
                    }
                    break;
                case BUTTON_BACK:
                    break;
                case BUTTON_ENTER:
                    mydisp.clearScreen();
                    switch(cursor)
                    {
                        case READINGS:
                            displayReadings();
                            break;
                        case SENSOR_ID:
                            displaySensorId();
                            break;
                        case DISPLAY_OFF:
                            mydisp.setBackLight(0);
                            displayReadings();
                            break;
                        default:
                            break;
                    }
resetMenu:
                    mydisp.clearScreen();
                    displayMenu();
                    cursor = 0;
                    prevCursor = 0;
                    button = NO_BUTTON;
                    now = millis();
                    break;
            }
        }
    }
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

    dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 

    //Initialize Gateway 
    gw.begin(NULL, AUTO, false, AUTO, RF24_PA_LEVEL, RF24_CHANNEL, RF24_DATARATE);
    
    // Send the Sketch Version Information to the Gateway
    gw.sendSketchInfo("Bespin", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    gw.present(CHILD_ID_HUM, S_HUM);
    gw.present(CHILD_ID_TEMP, S_TEMP);
    gw.present(CHILD_ID_CO2, S_AIR_QUALITY);

    metric = gw.getConfig().isMetric;
    
    //Initialize Display
    mydisp.clearScreen();
    mydisp.setFont(51);
    
    delay(1000);
}
 
void loop()
{
    menu();
}
