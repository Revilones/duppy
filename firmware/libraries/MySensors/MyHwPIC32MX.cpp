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

#ifdef __PIC32MX__ 

#include "MyHw.h"
#include "MyHwPIC32MX.h"

int8_t pinIntTrigger = 0;
void wakeUp()	 //place to send the interrupts
{
	pinIntTrigger = 1;
}
void wakeUp2()	 //place to send the second interrupts
{
	pinIntTrigger = 2;
}

#if 0
// Watchdog Timer interrupt service routine. This routine is required
// to allow automatic WDIF and WDIE bit clearance in hardware.
ISR (WDT_vect)
{
	// WDIE & WDIF is cleared in hardware upon entering this ISR
	wdt_disable();
}
#endif

MyHwPIC32MX::MyHwPIC32MX() : MyHw()
{
}


// The following was redifined as macros to save space

void hw_init() {
	Serial.begin(BAUD_RATE);
}

void hw_watchdogReset() {
	//wdt_reset();
}

void hw_reboot() {
	//wdt_enable(WDTO_15MS); for (;;);
}

unsigned long hw_millis() {
	return millis();
}

static void hw_initConfigBlock( size_t length = 1024 /*ATMega328 has 1024 bytes*/ )
{
  static bool initDone = false;
  if (!initDone)
  {
    EEPROM.setMaxAddress(length);
    initDone = true;
  }
}

void hw_readConfigBlock(void* buf, void* adr, size_t length)
{
  hw_initConfigBlock();
  uint8_t* dst = static_cast<uint8_t*>(buf);
  int offs = reinterpret_cast<int>(adr);
  while (length-- > 0)
  {
    *dst++ = EEPROM.read(offs++); 
  }
}

void hw_writeConfigBlock(void* buf, void* adr, size_t length)
{
  hw_initConfigBlock();
  uint8_t* src = static_cast<uint8_t*>(buf);
  int offs = reinterpret_cast<int>(adr);
  while (length-- > 0)
  {
    EEPROM.write(offs++, *src++);
  }
}

uint8_t hw_readConfig(int adr)
{
  uint8_t value;
  hw_readConfigBlock(&value, reinterpret_cast<void*>(adr), 1);
  return value;
}

void hw_writeConfig(int adr, uint8_t value)
{
  uint8_t curr = hw_readConfig(adr);
  if (curr != value)
  {
    hw_writeConfigBlock(&value, reinterpret_cast<void*>(adr), 1); 
  }
}

void powerDown(period_t period) {

#if 0
	ADCSRA &= ~(1 << ADEN);

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}
	#if defined __AVR_ATmega328P__
		do {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			cli();
			sleep_enable();
			sleep_bod_disable();
			sei();
			sleep_cpu();
			sleep_disable();
			sei();
		} while (0);
	#else
		do {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			cli();
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();
			sei();
		} while (0);
	#endif

	ADCSRA |= (1 << ADEN);
#endif
}

void MyHwPIC32MX::internalSleep(unsigned long ms) {
	// Let serial prints finish (debug, log etc)
  #ifdef ENABLED_SERIAL
	  Serial.flush();
  #endif
	pinIntTrigger = 0;
	while (!pinIntTrigger && ms >= 8000) { powerDown(SLEEP_8S); ms -= 8000; }
	if (!pinIntTrigger && ms >= 4000)    { powerDown(SLEEP_4S); ms -= 4000; }
	if (!pinIntTrigger && ms >= 2000)    { powerDown(SLEEP_2S); ms -= 2000; }
	if (!pinIntTrigger && ms >= 1000)    { powerDown(SLEEP_1S); ms -= 1000; }
	if (!pinIntTrigger && ms >= 500)     { powerDown(SLEEP_500MS); ms -= 500; }
	if (!pinIntTrigger && ms >= 250)     { powerDown(SLEEP_250MS); ms -= 250; }
	if (!pinIntTrigger && ms >= 125)     { powerDown(SLEEP_120MS); ms -= 120; }
	if (!pinIntTrigger && ms >= 64)      { powerDown(SLEEP_60MS); ms -= 60; }
	if (!pinIntTrigger && ms >= 32)      { powerDown(SLEEP_30MS); ms -= 30; }
	if (!pinIntTrigger && ms >= 16)      { powerDown(SLEEP_15Ms); ms -= 15; }
}

void MyHwPIC32MX::sleep(unsigned long ms) {
	internalSleep(ms);
}

bool MyHwPIC32MX::sleep(uint8_t interrupt, uint8_t mode, unsigned long ms) {
	// Let serial prints finish (debug, log etc)
	bool pinTriggeredWakeup = true;
	attachInterrupt(interrupt, wakeUp, mode);
	if (ms>0) {
		pinIntTrigger = 0;
		internalSleep(ms);
		if (0 == pinIntTrigger) {
			pinTriggeredWakeup = false;
		}
	} else {
    #ifdef ENABLED_SERIAL
		  Serial.flush();
    #endif
		powerDown(SLEEP_FOREVER);
	}
	detachInterrupt(interrupt);
	return pinTriggeredWakeup;
}

inline uint8_t MyHwPIC32MX::sleep(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms) {
	int8_t retVal = 1;
	attachInterrupt(interrupt1, wakeUp, mode1);
	attachInterrupt(interrupt2, wakeUp2, mode2);
	if (ms>0) {
		pinIntTrigger = 0;
		internalSleep(ms);
		if (0 == pinIntTrigger) {
			retVal = -1;
		}
	} else {
    #ifdef ENABLED_SERIAL
		  Serial.flush();
    #endif
		powerDown(SLEEP_FOREVER);
	}
	detachInterrupt(interrupt1);
	detachInterrupt(interrupt2);

	if (1 == pinIntTrigger) {
		retVal = (int8_t)interrupt1;
	} else if (2 == pinIntTrigger) {
		retVal = (int8_t)interrupt2;
	}
	return retVal;
}



#ifdef DEBUG
void MyHwPIC32MX::debugPrint(bool isGW, const char *fmt, ... ) {
	char fmtBuffer[300];
	if (isGW) {
		// prepend debug message to be handled correctly by controller (C_INTERNAL, I_LOG_MESSAGE)
		snprintf(fmtBuffer, 299, PSTR("0;0;%d;0;%d;"), C_INTERNAL, I_LOG_MESSAGE);
		Serial.print(fmtBuffer);
	}
	va_list args;
	va_start (args, fmt );
	va_end (args);
	if (isGW) {
		// Truncate message if this is gateway node
		vsnprintf(fmtBuffer, 60, fmt, args);
		fmtBuffer[59] = '\n';
		fmtBuffer[60] = '\0';
	} else {
		vsnprintf(fmtBuffer, 299, fmt, args);
	}
	va_end (args);
	Serial.print(fmtBuffer);
	Serial.flush();

	//Serial.write(freeRam());
}
#endif

#endif // #ifdef ARDUINO_ARCH_AVR
