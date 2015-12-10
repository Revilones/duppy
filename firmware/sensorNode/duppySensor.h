
#include <duppyTypes.h>
#include "duppyButton.h"

#include <MySensor.h>
#include <DigoleSerial.h>
#include <kSeries.h>
#include <SI7021.h>
#include <Adafruit_TSL2561_U.h>

int readHumidity(void *handle, double *value);
int readTemperature(void *handle, double *value);
int readCo2(void *handle, double *value);
int readCO(void *handle, double *value);
int readLux(void *handle, double *value);

#define MAX_ENTRIES 10

typedef struct SENSOR_ENTRY {
    char *label;
    void *handle;
    double value; 
    MyMessage *msg;
    int (*callBack)(void *handle, double *value);
} sensor_entry;

class DuppySensor
{
    public:
        DuppySensor(DigoleSerialDisp *display, MySensor *gw);
        void registerEntry(char *label, void *handle, MyMessage *msg, 
            int (*callBack)(void *handle, double *value));
        void sensorView();
    
    private:
        sensor_entry _sensors[MAX_ENTRIES];
        DigoleSerialDisp *_myDisp;
        MySensor *_gw;
        int _sensorCount;
        int _cursor;
        void changeCursor(int prevCursor, int cursor);
        void printSensor(char *item, double value);
        void displaySensors(int start, int end);
        void getSensorValues(bool send);
};
