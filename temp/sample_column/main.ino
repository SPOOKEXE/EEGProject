
/*
	EEG Circuit Reader

	TODO:
	- multiplexor
	- de-multiplexor
	- read data line
*/

// == Imports == //
#include <iostream>
#include <cmath>

// == Constants == //
const int DATA_READ_DELAY_MS = 10;
const int DATA_PIN = A0;

const int PINS_PER = 4;
const int MUX_CTRL_PINS[PINS_PER] = { 1, 2, 3, 4 }
const int DEMUX_POS_CTRL_PINS[PINS_PER] = { 2, 3, 4, 5 }
const int DEMUX_NEG_CTRL_PINS[PINS_PER] = { 6, 7, 8, 9 }

// == Variables == //
int LAST_DATA_READ = 0;
int COUNTER = 0;

// == Functions == //
double round_decimal(double num, int dec_places) {
	int decimals = std::pow(10, dec_places);
	return (double) (std::round(num * decimals) / decimals);
}

// == Main == //
void setup() {
	// read at 9600 bits of data per second
	Serial.begin(9600);

	// for each control pin, set mode to OUTPUT
	pinMode(DATA_PIN, INPUT);
	for (int index = 0; index < 4; index++) {
		pinMode( MUX_CTRL_PINS[index], OUTPUT );
		pinMode( DEMUX_POS_CTRL_PINS[index], OUTPUT );
		pinMode( DEMUX_NEG_CTRL_PINS[index], OUTPUT );
	}
}

void loop() {

	// data read delay
	if (millis() - LAST_DATA_READ < DATA_READ_DELAY_MS) {
		return;
	}
	LAST_DATA_READ = millis();

	// TODO: read next data pin
	COUNTER += 1;


}
