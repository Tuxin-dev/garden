/*
 * watering.cpp
 *
 *  Created on: 15 août 2019
 *      Author: tuxin
 */
#include <Arduino.h>

// Flags
// -----
const byte WATERING_IN_PROGRESS = (1 << 0);

// Pins
// ----
const int WATERING_CMD_PIN = 4;

// Variables
// ---------
static unsigned long timer_watering;
static unsigned char flags_watering;

// Functions Declaration
// ---------------------
void startWatering(void);
void stopWatering(void);

// Public Functions
// ----------------
void initWatering(void)
{
	timer_watering = 0;
	flags_watering = 0;
	pinMode(WATERING_CMD_PIN, OUTPUT);
}

void setWateringDuration(unsigned int seconds)
{
	timer_watering = seconds;
}

/* Called each second */
void processWatering(void)
{
    if (timer_watering) {
    	if (!(flags_watering & WATERING_IN_PROGRESS)) {
    		startWatering();
    	}
    	if (timer_watering != 65535) {
    	    --timer_watering;
    	}
    }
    else {
    	if (flags_watering & WATERING_IN_PROGRESS) {
    	    stopWatering();
    	}
    }
}

void startWatering(void)
{
    digitalWrite(WATERING_CMD_PIN, HIGH);
    flags_watering |= WATERING_IN_PROGRESS;
}

void stopWatering(void)
{
	digitalWrite(WATERING_CMD_PIN, LOW);
	flags_watering &= ~WATERING_IN_PROGRESS;
}


