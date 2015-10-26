/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#ifndef MyHwPIC32MX_h
#define MyHwPIC32MX_h

#ifdef __PIC32MX__ 

#include "MyHw.h"
#include "MyConfig.h"
#include "MyMessage.h"
#include <EEPROM.h>
//#include <avr/pgmspace.h>
//#include <avr/sleep.h>
//#include <avr/power.h>
//#include <avr/interrupt.h>
//#include <avr/wdt.h>

#ifdef __cplusplus
#include <Arduino.h>
#include <SPI.h>
#endif

enum period_t
{
	SLEEP_15Ms,
	SLEEP_30MS,
	SLEEP_60MS,
	SLEEP_120MS,
	SLEEP_250MS,
	SLEEP_500MS,
	SLEEP_1S,
	SLEEP_2S,
	SLEEP_4S,
	SLEEP_8S,
	SLEEP_FOREVER
};

void hw_init();
void hw_watchdogReset();
void hw_reboot();
unsigned long hw_millis();
void hw_readConfigBlock(void* buf, void* adr, size_t length);
void hw_writeConfigBlock(void* buf, void* adr, size_t length);
void hw_writeConfig(int adr, uint8_t value);
uint8_t hw_readConfig(int adr);

class MyHwPIC32MX : public MyHw
{ 
public:
	MyHwPIC32MX();

	void sleep(unsigned long ms);
	bool sleep(uint8_t interrupt, uint8_t mode, unsigned long ms);
	uint8_t sleep(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms);
#ifdef DEBUG
	void debugPrint(bool isGW, const char *fmt, ... );
#endif
private:
	void internalSleep(unsigned long ms);
};
#endif
#endif // #ifdef __PIC32MX__
