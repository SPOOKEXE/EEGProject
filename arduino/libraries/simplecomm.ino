
#include <Arduino.h>
#include "simplecomm.h"

simplecomm bridge(Serial);

void setup() {
	Serial.begin(115200);
	Serial.setTimeout(1);

	Serial.print("Ready to go!");
}

void loop() {

	bridge.update();

	// if (bridge.messageAvailable == true) {
	// 	Serial.println(bridge.read());
	// 	bridge.write("Hello Python!");
	// }

}
