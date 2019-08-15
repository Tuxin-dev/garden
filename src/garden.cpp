#include <Arduino.h>
#include "watering.h"
#include "lighting.h"

void setup() {
    initWatering();
    initLighting();
}

void loop() {
	processWatering();
	processLighting();
}
