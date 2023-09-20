

#define BIT(n, i) (n >> i & 1);

const String COMMAND_START_READER = "x1";
const String COMMAND_STOP_READER = "x2";
const String COMMAND_GET_DATA_PINS = "x3";

/*
	PIN-STATE TABLE
	---------------
	0 = UNUSED,
	1 = NEGATIVE,
	2 = POSITIVE,
	3 = READ
*/
const int TOTAL_PINS = 64;
const int PINS_TYPES[TOTAL_PINS] = { 2, 3, 1, 3, 2, 3, 1, 0, 3, 0, 3, 0, 3, 0, 3, 0, 1, 3, 2, 3, 1, 3, 2, 0, 3, 0, 3, 0, 3, 0, 3, 0, 2, 3, 1, 3, 2, 3, 1, 0, 3, 0, 3, 0, 3, 0, 3, 0, 1, 3, 2, 3, 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*
Select Pins
*/
const int TOTAL_SELECT_PINS = 6;
const int ARDUINO_SEL_PINS[TOTAL_SELECT_PINS] = { 2, 3, 4, 5, 6, 7 };
const int ARDUINO_DATA_PIN = A4;

// delay in milliseconds to allow mux to update
const int MUX_SELECT_DELAY_MS = 10;

// is the reading cycle enabled (spam python w/ readings)
bool readingCycleEnabled = false;
// total data pins
int TOTAL_DATA_PINS = 0;

// select using bits
void set_mux_select(int s1, int s2, int s3, int s4, int s5, int s6) {
	digitalWrite( ARDUINO_SEL_PINS[0], s1 );
	digitalWrite( ARDUINO_SEL_PINS[1], s2 );
	digitalWrite( ARDUINO_SEL_PINS[2], s3 );
	digitalWrite( ARDUINO_SEL_PINS[3], s4 );
	digitalWrite( ARDUINO_SEL_PINS[4], s5 );
	digitalWrite( ARDUINO_SEL_PINS[5], s6 );
	// delay for electricity, milliseconds
	delay( MUX_SELECT_DELAY_MS );
}

// convert number index (0-63) to 0/1 mux input
void mux_select_data_pin(int pinIndex) {
	// flip with !BIT so it goes in reverse order (16 -> 0) instead of
	int b1 = !BIT(pinIndex % 16, 0);
	int b2 = !BIT(pinIndex % 16, 1);
	int b3 = !BIT(pinIndex % 16, 2);
	int b4 = !BIT(pinIndex % 16, 3);

	// select mux chip based on pin #
	if (pinIndex >= 0 and pinIndex <= 15) {
		set_mux_select(0, 0, b4, b3, b2, b1);
	} else if (pinIndex >= 16 and pinIndex <= 31) {
		set_mux_select(0, 1, b4, b3, b2, b1);
	} else if (pinIndex >= 32 and pinIndex <= 47) {
		set_mux_select(1, 0, b4, b3, b2, b1);
	} else if (pinIndex >= 48 and pinIndex <= 63) {
		set_mux_select(1, 1, b4, b3, b2, b1);
	}
}

// reads 0 -> 1024
int read_data_pin() {
	return analogRead(ARDUINO_DATA_PIN);
}

// iterate over mux for readings
void update_readings(int readings[], int length) {
	// iterate over data pins
	int index = 0;
	for (int pin_index = 0; pin_index < TOTAL_PINS; pin_index++) {
		// if its a read pin
		if (PINS_TYPES[pin_index] == 3) {
			// read the pin's value
			mux_select_data_pin(pin_index);
			readings[index] = read_data_pin();
			index += 1;
		}
	}
}

// handle incoming python
void handle_incoming_command( String command ) {
	if (command == COMMAND_START_READER) {
		readingCycleEnabled = true;
	} else if (command == COMMAND_STOP_READER) {
		readingCycleEnabled = false;
	} else if (command == COMMAND_GET_DATA_PINS) {
		for (int pin_index = 0; pin_index < TOTAL_PINS; pin_index++) {
			Serial.print(PINS_TYPES[pin_index]);
			Serial.print(",");
		}
		Serial.println("");
	}
}

// handle sending readings
void send_readings_to_python() {
	// readings array of n items
	int readings[TOTAL_DATA_PINS];
	for (int i = 0; i < TOTAL_DATA_PINS - 1; i++) readings[i] = 0;

	// update the readings
	update_readings(readings, TOTAL_DATA_PINS);

	// output the readings
	for (int index = 0; index < TOTAL_DATA_PINS; index++) {
		Serial.print(readings[index]);
		Serial.print(",");
	}
	Serial.print("\n");
}

void setup() {
	Serial.begin(115200);

	// set analog 0 as input
	pinMode(A0, INPUT);

	// set all select digital pins as output
	for (int i = 0; i < TOTAL_SELECT_PINS; i++) {
		pinMode(ARDUINO_SEL_PINS[i], OUTPUT);
	}

	// count
	TOTAL_DATA_PINS = 0;
	for (int pin_index = 0; pin_index < TOTAL_PINS; pin_index++) {
		if (PINS_TYPES[pin_index] == 3) {
			TOTAL_DATA_PINS += 1;
		}
	}
}

void loop() {
	// read incoming data
	if (Serial.available() > 2) {
		String value = Serial.readStringUntil('\n');
		// Serial.println(value);
		handle_incoming_command(value);
	}

	// send outgoing if enabled
	if (readingCycleEnabled) {
		send_readings_to_python();
	}

	delay(20);
}
