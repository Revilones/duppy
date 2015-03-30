//Digole Digital Solutions: www.digole.com
#include "DigoleSerial.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*
// Communication set up command
 * "SB":Baud (ascII bytes end with 0x00/0x0A/0x0D) -- set UART Baud Rate
 * "SI2CA":Address(1 byte <127) -- Set I2C address, default address is:0x27
 * "DC":1/0(1byte) -- set config display on/off, if set to 1, displayer will display current commucation setting when power on
// Text Function command
 * "CL": -- Clear screen--OK
 * "CS":1/0 (1 byte)-- Cursor on/off
 * "TP":x(1 byte) y(1 byte) -- set text position
 * "TT":string(bytes) end with 0x00/0x0A/0x0D -- display string under regular mode
// Graphic function command
 * "GP":x(1byte) y(1byte) -- set current graphic position
 * "DM":"C/!/~/&/|/^"(ASCII 1byte) -- set drawing mode--C="Copy",! and ~ = "Not", & = "And", | = "Or", ^ = "Xor"
 * "SC":1/0 (1byte) -- set draw color--only 1 and 0
 * "LN":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw line from x0,y0 to x1,y1,set new pot to x1,y1
 * "LT":x(1byte) y(1byte) -- draw line from current pos to x,y
 * "CC":x(1byte) y(1byte) ratio(byte) -- draw circle at x,y with ratio
 * "DP":x(1byte) y(1byte) Color(1byte) -- draw a pixel--OK
 * "DR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw rectangle, top-left:x0,y0; right-bottom:x1,y1
 * "FR":x0(1byte) y0(1byte) x1(1byte) y2(1byte)--draw filled rectangle, top-left:x0,y0; right-bottom:x1,y1
 */

// that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

//UART function

const uint8_t STCR_END[] = {0x80,0xC0,0x94,0xD4};

size_t DigoleSerialDisp::sendData(const char *buffer)
{
  size_t n = 0;
  size_t size = strlen(buffer);
  while (size--) {
    n += digole_write((uint8_t)*buffer++);
  }
  return n;
}

size_t DigoleSerialDisp::sendData(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    n += digole_write((uint8_t)*buffer++);
  }
  return n;
}

void DigoleSerialDisp::setLCDColRow(uint8_t col, uint8_t row) {
    sendData("STCR");
    digole_write(col);
    digole_write(row);
    sendData(STCR_END, 4);
}
/*----------Functions for Graphic LCD/OLED adapters only---------*/
void DigoleSerialDisp::drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {
    uint8_t i = 0;
    if ((w & 7) != 0)
        i = 1;
    sendData("DIM");
    digole_write(x); //x;
    digole_write(y);
    digole_write(w);
    digole_write(h);
    for (int j = 0; j < h * ((w >> 3) + i); j++) {
        digole_write(pgm_read_byte_near(bitmap + j));
//        delay(5);
    }
}

void DigoleSerialDisp::setRot90(void) {
    sendData("SD1");
}

void DigoleSerialDisp::setRot180(void) {
    sendData("SD2");
}

void DigoleSerialDisp::setRot270(void) {
    sendData("SD3");
}

void DigoleSerialDisp::undoRotation(void) {
    sendData("SD0");
}

void DigoleSerialDisp::setRotation(uint8_t d) {
    sendData("SD");
    digole_write(d);
}

void DigoleSerialDisp::setContrast(uint8_t c) {
    sendData("CT");
    digole_write(c);
}

void DigoleSerialDisp::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    sendData("FR");
    digole_write(x);
    digole_write(y);
    digole_write(x + w);
    digole_write(y + h);
}

void DigoleSerialDisp::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    setColor(color);
    sendData("FR");
    digole_write(x);
    digole_write(y);
    digole_write(x + w);
    digole_write(y + h);
}

void DigoleSerialDisp::drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t f) {
    sendData("CC");
    digole_write(x);
    digole_write(y);
    digole_write(r);
    digole_write(f);
}

void DigoleSerialDisp::drawDisc(uint8_t x, uint8_t y, uint8_t r) {
    drawCircle(x, y, r, 1);
}

void DigoleSerialDisp::drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    sendData("DR");
    digole_write(x);
    digole_write(y);
    digole_write(x + w);
    digole_write(y + h);
}

void DigoleSerialDisp::drawPixel(int16_t x, int16_t y, uint16_t color) {
    setColor(color);
    sendData("DP");
    digole_write((uint8_t)x);
    digole_write((uint8_t)y);
}

void DigoleSerialDisp::drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1) {
    sendData("LN");
    digole_write(x);
    digole_write(y);
    digole_write(x1);
    digole_write(y1);
}

void DigoleSerialDisp::drawLineTo(uint8_t x, uint8_t y) {
    sendData("LT");
    digole_write(x);
    digole_write(y);
}

void DigoleSerialDisp::drawHLine(uint8_t x, uint8_t y, uint8_t w) {
    drawLine(x, y, x + w, y);
}

void DigoleSerialDisp::drawVLine(uint8_t x, uint8_t y, uint8_t h) {
    drawLine(x, y, x, y + h);
}

void DigoleSerialDisp::nextTextLine(void) {
    digole_write((uint8_t) 0);
    sendData("TRT");
}

void DigoleSerialDisp::setFont(uint8_t font) {
    sendData("SF");
    digole_write(font);
}

void DigoleSerialDisp::setColor(uint8_t color) {
    sendData("SC");
    digole_write(color);
}

void DigoleSerialDisp::backLightOn(void) {
    sendData("BL");
    digole_write((uint8_t) 1);
}

void DigoleSerialDisp::backLightOff(void) {
    sendData("BL");
    digole_write((uint8_t) 0);
}

void DigoleSerialDisp::directCommand(uint8_t d) {
    sendData("MCD");
    digole_write(d);
}

void DigoleSerialDisp::directData(uint8_t d) {
    sendData("MDT");
    digole_write(d);
}

void DigoleSerialDisp::moveArea(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char xoffset, char yoffset) {
    sendData("MA");
    digole_write(x0);
    digole_write(y0);
    digole_write(w);
    digole_write(h);
    digole_write(xoffset);
    digole_write(yoffset);
}

void DigoleSerialDisp::uploadStartScreen(int lon, const unsigned char *data) {
     int j;
     uint8_t c;
    sendData("SSS");
    digole_write((uint8_t) (lon % 256));
    digole_write((uint8_t) (lon / 256));
    delay(300);
    for (j = 0; j < lon;j++) {
        if((j%32)==0)
         delay(50);
         delay(_Comdelay);
        c=pgm_read_byte_near(data+j);
        digole_write(c);
    }
}

void DigoleSerialDisp::uploadUserFont(int lon, const unsigned char *data, uint8_t sect) {
     uint8_t c;
    sendData("SUF");
    digole_write(sect);
    digole_write((uint8_t) (lon % 256));
    digole_write((uint8_t) (lon / 256));
    for (int j = 0; j < lon; j++) {
        if((j%32)==0)
         delay(50);
         delay(_Comdelay);
        c=pgm_read_byte_near(data+j);
        digole_write(c);
    }
}
void DigoleSerialDisp::drawBitmap256(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {	//display 256 color image
    uint8_t i = 0;
    sendData("EDIM1");
    digole_write(x); //x;
    digole_write(y);
    digole_write(w);
    digole_write(h);
    for (int j = 0; j < h * w; j++) {
        digole_write(pgm_read_byte_near(bitmap + j));
//        delay(5);
    }
}
void DigoleSerialDisp::drawBitmap262K(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bitmap) {	//display 256 color image
    uint8_t i = 0;
    sendData("EDIM3");
    digole_write(x); //x;
    digole_write(y);
    digole_write(w);
    digole_write(h);
    for (int j = 0; (j < h * w *3); j++) {
        digole_write(pgm_read_byte_near(bitmap + j));
//        delay(5);
    }
}
void DigoleSerialDisp::setTrueColor(uint8_t r, uint8_t g, uint8_t b) {	//Set true color
    uint8_t i = 0;
    sendData("ESC");
    digole_write(r); 
    digole_write(g);
    digole_write(b);
}






