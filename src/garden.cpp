#include <arduino.h>
#include "watering.h"
#include "lighting.h"
#include "protocol.h"

const int BUTTON = 4;

static unsigned long timer;

void setup() {
	pinMode(BUTTON, INPUT);
	initLighting();
	initWatering();
	initProtocol();
	timer = millis();
}

void loop() {
    if((millis() -­ timer) >= 1000) {
        processWatering();
        processLighting();

        timer = millis();
	}
}
