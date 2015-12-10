
#include "duppySensor.h"

#define MAX_DISPLAYED 3

#define SEND_INTERVAL 60000 //Delay between sensor readings
#define DISPLAY_INTERVAL 5000 //Delay between sensor readings

int readHumidity(void *handle, double* value)
{
    int error = SUCCESS;
    SI7021 *dht = (SI7021*)handle;
    float humidity = 0;
    
    if (dht->sensorExists())
    {
        humidity = dht->getHumidityPercent();
    }
    else
    {
      Serial.println("Humidity Error");
      error = ERROR_INTERNAL;
      goto error;
    }
    
    *value = humidity;
    
cleanup:
    
    return error;
    
error:

    goto cleanup;
}

int readTemperature(void *handle, double *value)
{
    int error = SUCCESS;
    SI7021 *dht = (SI7021*)handle;
    float temperature = 0;
    
    if (dht->sensorExists())
    {
        temperature = dht->getCelsiusHundredths();
        temperature = temperature / 100;
    }
    else
    {
        Serial.println("Temperature Error");
        error = ERROR_INTERNAL;
        goto error;
    }
    
    *value = temperature;

cleanup:
    
    return error;
    
error:

    goto cleanup;
}

int readCo2(void *handle, double *value)
{
    int error = SUCCESS;
    kSeries *k_30 = (kSeries*)handle;
    double co2 = 0;
    
    co2 = k_30->getCO2('p');
    
    if (co2 < 0)
    {
        Serial.println("Co2 Error");
        error = ERROR_INTERNAL;
        goto error;
    }
    
    *value = co2;

cleanup:

    return error;
    
error:

    goto cleanup;
}

int readCO(void *handle, double *value)
{
    /* Equations
    * Transimpedance Amplifier Vout = Ip*Rf
    */
    int error = SUCCESS;
    const double VperU = .0049; //4.9 mv/unit
    const double nAperPPM = .0000000018;
    const double resistance = 988000;
    int COpin = *(int*)handle;
    double Vout = 0;
    double Ip = 0;
    double co = 0;

    Vout = analogRead(COpin);
    
    Vout = VperU * Vout;
    Ip = Vout / resistance;
    co = Ip / nAperPPM; //Gives CO in PPM

    *value = co;

cleanup:

    return error;

error:

    goto cleanup;
}

int readLux(void *handle, double *value)
{
    int error = SUCCESS;
    Adafruit_TSL2561_Unified *tsl = (Adafruit_TSL2561_Unified*)handle;
    uint16_t ir = 0;
    uint16_t broadband = 0;
    double lux = 0;
    
    if (tsl->initialized())
    {
        /* Calculate the actual lux value */
        tsl->getLuminosity(&broadband, &ir);
        lux = tsl->calculateLux(broadband, ir);
    }
    else
    {
        Serial.println("Lux Error");
        error = ERROR_INTERNAL;
        goto error;
    }
    
    *value = lux;

cleanup:

    return error;
    
error:

    goto cleanup;
}

DuppySensor::DuppySensor(DigoleSerialDisp *display, MySensor *gw)
{
    _myDisp = display;
    _gw = gw;
    _sensorCount = 0; 
}

void DuppySensor::registerEntry(char *label, void *handle, MyMessage *msg, 
    int (*callBack)(void *handle, double *value))
{
    _sensors[_sensorCount].label = label;
    _sensors[_sensorCount].handle = handle;
    _sensors[_sensorCount].callBack = callBack;
    _sensors[_sensorCount].msg = msg;
    _sensors[_sensorCount].value = 0;
    _sensorCount++;
}

void DuppySensor::sensorView()
{
    int button = NO_BUTTON;
    int prevCursor = 0;
    int cursor = 0;
    long timeDisplay = 0;
    long timeSend = 0;

    getSensorValues(false);
    displaySensors(0, MAX_DISPLAYED);

    timeDisplay = millis();
    timeSend = millis();

    while(1)
    {
        if (millis() > (timeDisplay + DISPLAY_INTERVAL))
        {
            getSensorValues(false);
            displaySensors(cursor, cursor+MAX_DISPLAYED);
            timeDisplay = millis();
        }

        if (millis() > (timeSend + SEND_INTERVAL))
        {
            getSensorValues(true);
            timeSend = millis();
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
                    if (cursor < (_sensorCount-1) / MAX_DISPLAYED)
                    {
                        prevCursor = cursor;
                        cursor++;
                        changeCursor(prevCursor, cursor);
                    }
                    break;
                case BUTTON_ENTER:
                case BUTTON_BACK:
                    return;
            }
        }
    }
}

void DuppySensor::changeCursor(int prevCursor, int cursor)
{
    displaySensors(cursor, cursor+MAX_DISPLAYED);
}

void DuppySensor::printSensor(char *item, double value)
{
    _myDisp->setFont(51);
    _myDisp->print(item);
    _myDisp->print(value);
    _myDisp->nextTextLine();
    _myDisp->setFont(10);
    _myDisp->nextTextLine();
}

void DuppySensor::displaySensors(int start, int end)
{
    int i = 0;
    _myDisp->clearScreen();
    _myDisp->setPrintPos(0, 1, _TEXT_);
    for (i=start; i<=end && i<_sensorCount; i++)
    {
        printSensor(_sensors[i].label, _sensors[i].value);
    }
}

void DuppySensor::getSensorValues(bool send)
{
    int i = 0;
    double value = 0;
    for (i=0; i<=_sensorCount - 1; i++)
    {
        _sensors[i].callBack(_sensors[i].handle, &value);
        _sensors[i].value = value;
        if (send == true)
        {
            _gw->send(_sensors[i].msg->set(value, 1));
        }
    }
}

