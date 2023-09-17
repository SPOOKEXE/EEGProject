
/*
	MIXTURE OF C++ and Arduino
	-> MUST be converted to Arduino.
*/

/*
	EEG Design 2
	> 64 electrodes
	> 1 output, single stream

	Pros:
	- fast
	- efficient
	- simple

	Cons:
	- limited (software) customization
	- manual node positions via hardware
*/

#include <iostream>
#include <math.h>
#include <algorithm>
#include <bits/stdc++.h>

const int TOTAL_PINS = 64;

/*
	PIN-STATE TABLE
	---------------
	0 = UNUSED,
	1 = NEGATIVE,
	2 = POSITIVE,
	3 = READ
*/
const int PINS_TYPES[TOTAL_PINS] = {
	2, 3, 1, 3, 2, 3, 1, 0, // pins 0-8 (top left)
	3, 0, 3, 0, 3, 0, 3, 0,
	1, 3, 2, 3, 1, 3, 2, 0,
	3, 0, 3, 0, 3, 0, 3, 0, // top right
	2, 3, 1, 3, 2, 3, 1, 0, // bottom left
	3, 0, 3, 0, 3, 0, 3, 0,
	1, 3, 2, 3, 1, 3, 2, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // pins 54-64 (bottom right)
};

const int TOTAL_SELECT_PINS = std::ceil( std::log2( TOTAL_PINS ) );
const int ARDUINO_SEL_PINS[TOTAL_SELECT_PINS] = { 0, 1, 2, 3, 4, 5 };
const int ARDUINO_READ_PIN = A0;
const int ARDUINO_READ_DELAY_MS = 100;

const int TOTAL_UNUSED_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 0 );
const int TOTAL_NEGATIVE_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 1 );
const int TOTAL_POSITIVE_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 2 );
const int TOTAL_DATA_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 3 );

int counter;

void set_arduino_mux( int s0, int s1, int s2, int s3, int s4, int s5 ) {
	// output mux selection info
	std::cout << "MUX_SEL: " << s0 << s1 << s2 << s3 << s4 << s5 << std::endl;

	// set the select pin outputs
	digitalWrite( ARDUINO_SEL_PINS[0], s0 );
	digitalWrite( ARDUINO_SEL_PINS[1], s1 );
	digitalWrite( ARDUINO_SEL_PINS[2], s2 );
	digitalWrite( ARDUINO_SEL_PINS[3], s3 );
	digitalWrite( ARDUINO_SEL_PINS[4], s4 );
	digitalWrite( ARDUINO_SEL_PINS[5], s5 );
}

// pinIndex = 0 -> 63
float read_data_pin( int pinIndex ) {
	// convert to bits set
	std::bitset dat_bits = std::bitset<4>(pinIndex % 16);
	// flip it so it goes in reverse order (16 -> 0) instead of (0 -> 16)
	dat_bits.flip();

	if (pinIndex >= 0 and pinIndex <= 15) {
		set_arduino_mux( 0, 0, dat_bits[3], dat_bits[2], dat_bits[1], dat_bits[0] );
	} else if (pinIndex >= 16 and pinIndex <= 31) {
		set_arduino_mux( 0, 1, dat_bits[3], dat_bits[2], dat_bits[1], dat_bits[0] );
	} else if (pinIndex >= 32 and pinIndex <= 47) {
		set_arduino_mux( 1, 0, dat_bits[3], dat_bits[2], dat_bits[1], dat_bits[0] );
	} else if (pinIndex >= 48 and pinIndex <= 63) {
		set_arduino_mux( 1, 1, dat_bits[3], dat_bits[2], dat_bits[1], dat_bits[0] );
	}

	// delay so electricity can update :wizard:
	delay(25);
	return analogRead( ARDUINO_READ_PIN );
}

// https://www.techiedelight.com/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/

void update_readings( int readings[], int size ) {
	// array of n items that are set to 0.
	int SEL_PINS[TOTAL_SELECT_PINS];

	// set array to 0s
	for ( int i = 0; i < size-1; i++ ) readings[i] = 0;

	// iterate over pins
	int index = 0;
	for (int pin_index = 0; pin_index < TOTAL_PINS; pin_index++) {
		// if its a read pin
		if (PINS_TYPES[pin_index] == 3) {
			// read the pin's value
			readings[index] = read_pin_value( pin_index );
			index += 1;
		}
	}
}

void send_pin_data( ) {
	// TOTAL_PINS, PINS_TYPES

}

void send_readings( const int timestamp, const int readings[] ) {

}

void loop() {
	// reading array of n items
	int READINGS[TOTAL_DATA_PINS];

	// update the readings
	update_readings( READINGS, TOTAL_DATA_PINS );

	// output the readings
	std::cout << "DATA READINGS: " << std::endl;
	for (int i = 0; i < TOTAL_DATA_PINS-1; i++) {
		std::cout << " " << READINGS[i];
	}
	std::cout << std::endl;

	// send over serial
	send_over_serial( counter, readings );

	// increment counter
	counter += 1;
}

void setup() {
	// value output
	std::cout << "UNUSED PINS: " << TOTAL_UNUSED_PINS << std::endl;
	std::cout << "NEGATIVE PINS: " << TOTAL_NEGATIVE_PINS << std::endl;
	std::cout << "POSITIVE PINS: " << TOTAL_POSITIVE_PINS << std::endl;
	std::cout << "DATA PINS: " << TOTAL_DATA_PINS << std::endl;

	// setup the counter value
	counter = 0;

	// allow mux to be read from
	pinMode(ARDUINO_READ_PIN, INPUT);

	// allow select pins to be set (digital)
	for (int i = 0; i < TOTAL_SELECT_PINS; i++) {
		pinMode(ARDUINO_SEL_PINS[i], OUTPUT);
	}
}

// for .cpp runtime
int main() {
	setup();
	loop();

	// while (true) {
	// 	loop();
	// };

	return 0;
};
