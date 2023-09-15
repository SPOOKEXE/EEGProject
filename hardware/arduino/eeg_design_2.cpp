
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
#include <chrono>
#include <fstream>

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

const int TOTAL_UNUSED_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 0 );
const int TOTAL_NEGATIVE_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 1 );
const int TOTAL_POSITIVE_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 2 );
const int TOTAL_DATA_PINS = std::count( PINS_TYPES, PINS_TYPES + TOTAL_PINS, 3 );

int read_pin_value( int pinIndex ) {
	return -1;
}

// https://www.techiedelight.com/get-current-timestamp-in-milliseconds-since-epoch-in-cpp/
uint64_t get_time_since_epoch( ) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

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

void send_over_serial( uint64_t timestamp, int readings[] ) {

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

	// output to file with timestamp
	uint64_t timestamp = get_time_since_epoch();
	// send over serial
	send_over_serial( timestamp, readings );
}

void setup() {
	std::cout << "UNUSED PINS: " << TOTAL_UNUSED_PINS << std::endl;
	std::cout << "NEGATIVE PINS: " << TOTAL_NEGATIVE_PINS << std::endl;
	std::cout << "POSITIVE PINS: " << TOTAL_POSITIVE_PINS << std::endl;
	std::cout << "DATA PINS: " << TOTAL_DATA_PINS << std::endl;
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
