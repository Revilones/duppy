//Digole Digital Solutions: www.digole.com
#ifndef DigoleSerialDisp_h
#define DigoleSerialDisp_h

#include <inttypes.h>
#include "Print.h"
#include "../Wire/Wire.h"
#include "Arduino.h"
#include <Adafruit_GFX.h>

// Communication set up command
// Text function command
// Graph function command
#define Serial_UART 0
#define Serial_I2C 1
#define Serial_SPI 2
#define _TEXT_ 0
#define _GRAPH_ 1

#define DIGOLE_WIDTH 160
#define DIGOLE_HEIGHT 128

class DigoleSerialDisp : public Adafruit_GFX {
public:
#if defined(_Digole_Serial_UART_)

DigoleSerialDisp(HardwareSerial *s, unsigned long baud) : Adafruit_GFX(160,128)//UART set up
    {
        _mySerial = s;
        _Baud = baud;
        _Comdelay=2;
    }

    size_t digole_write(uint8_t value) {
        _mySerial->write(value);
        return 1; // assume sucess
    }

    void begin(void) {
        _mySerial->begin(9600);
        _mySerial->print("SB");
        _mySerial->println(_Baud);
        delay(100);
        _mySerial->begin(_Baud);
    }
#endif
#if defined(_Digole_Serial_I2C_)

void begin(void) {
        _myWire->begin();
    }

    DigoleSerialDisp(TwoWire *s, uint8_t add) : Adafruit_GFX(160,128) //U2C set up
    {
        _myWire = s;
        _I2Caddress = add;
        _Comdelay=6;
    }
#endif
#if defined(_Digole_Serial_SPI_)

void begin(void) {
    }

    DigoleSerialDisp(uint8_t pin_data, uint8_t pin_clock, uint8_t SS) : Adafruit_GFX(160,128) //spi set up
    {
        _Clockpin = pin_clock;
        _Datapin = pin_data;
        _SSpin = SS;
        pinMode(_Clockpin, OUTPUT);
        pinMode(_Datapin, OUTPUT);
        pinMode(_SSpin, OUTPUT);
        digitalWrite(_SSpin, HIGH);
        digitalWrite(_Clockpin, LOW);
        digitalWrite(_Datapin, LOW);
        _Comdelay=6;
    }

    size_t digole_write(uint8_t value) {
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        digitalWrite(_SSpin, LOW);
        shiftOut(_Datapin, _Clockpin, MSBFIRST, value);
        digitalWrite(_SSpin, HIGH);
        return 1; // assume sucess
    }
#endif
    //    virtual size_t write(uint8_t);
    //    void begin(void);
    size_t digole_write(uint8_t value) {
        _myWire->beginTransmission(_I2Caddress);
        _myWire->write(value);
        _myWire->endTransmission();
        return 1; // assume sucess
    }
    
    size_t sendData(const char *buffer);
    size_t sendData(const uint8_t *buffer, size_t size);

        /*---------fucntions for Text and Graphic LCD adapters---------*/
    void disableCursor(void) {
        sendData("CS0");
    }

    void enableCursor(void) {
        sendData("CS1");
    }

    void drawStr(uint8_t x, uint8_t y, const char *s) {
        sendData("TP");
        digole_write(x);
        digole_write(y);
        sendData("TT");
        Print::println(s);
    }

    void setPrintPos(uint8_t x, uint8_t y, uint8_t graph = _TEXT_) {
        if (graph == _TEXT_) {
            sendData("TP");
            digole_write(x);
            digole_write(y);
        } else {
            sendData("GP");
            digole_write(x);
            digole_write(y);
        }
    }

    void clearScreen(void) {
        sendData("CL");
    }

    void setI2CAddress(uint8_t add) {
        sendData("SI2CA");
        digole_write(add);
        _I2Caddress = add;
    }

    void displayConfig(uint8_t v) {
        sendData("DC");
        digole_write(v);
    }

    /*----------Functions for Graphic LCD/OLED adapters only---------*/
    //the functions in this section compatible with u8glib
    void setLCDColRow(uint8_t col, uint8_t row);
    void drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void drawBitmap256(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void drawBitmap262K(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap);
    void setTrueColor(uint8_t r, uint8_t g, uint8_t b);
    void setRot90(void);
    void setRot180(void);
    void setRot270(void);
    void undoRotation(void);
    void setRotation(uint8_t);
    void setContrast(uint8_t);
    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
    void drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t = 0);
    void drawDisc(uint8_t x, uint8_t y, uint8_t r);
    void drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
    void drawLineTo(uint8_t x, uint8_t y);
    void drawHLine(uint8_t x, uint8_t y, uint8_t w);
    void drawVLine(uint8_t x, uint8_t y, uint8_t h);
    //-------------------------------
    //special functions for our adapters
    void setFont(uint8_t font); //set font, availale: 6,10,18,51,120,123, user font: 200-203
    void nextTextLine(void); //got to next text line, depending on the font size
    void setColor(uint8_t); //set color for graphic function
    void backLightOn(void); //Turn on back light
    void backLightOff(void); //Turn off back light
    void directCommand(uint8_t d); //send command to LCD drectly
    void directData(uint8_t d); //send data to LCD drectly
    void moveArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char xoffset, char yoffset); //move a area of screen to another place

    void displayStartScreen(uint8_t m) {
        sendData("DSS");
        digole_write(m);
    } //display start screen

    void setMode(uint8_t m) {
        sendData("DM");
        digole_write(m);
    } //set display mode

    void setTextPosBack(void) {
        sendData("ETB");
    } //set text position back to previous, only one back allowed

    void setTextPosOffset(char xoffset, char yoffset) {
        sendData("ETO");
        digole_write(xoffset);
        digole_write(yoffset);
    }

    void setTextPosAbs(uint8_t x, uint8_t y) {
        sendData("ETP");
        digole_write(x);
        digole_write(y);
    }
    void setLinePattern(uint8_t pattern) {
        sendData("SLP");
        digole_write(pattern);
     }
    void setLCDChip(uint8_t chip) {      //only for universal LCD adapter
        sendData("SLCD");
        digole_write(chip);
     }
    void setBackLight(uint8_t bl){
        sendData("BL");
        digole_write(bl);
    }
    void uploadStartScreen(int lon, const unsigned char *data); //upload start screen
    void uploadUserFont(int lon, const unsigned char *data, uint8_t sect); //upload user font
    void digitalOutput(uint8_t x) {sendData("DOUT");digole_write(x);}
private:
    unsigned long _Baud;
    HardwareSerial *_mySerial;
    uint8_t _I2Caddress;
    TwoWire *_myWire;
    uint8_t _Clockpin;
    uint8_t _Datapin;
    uint8_t _SSpin;
    uint8_t _Comdelay;
};

#endif
