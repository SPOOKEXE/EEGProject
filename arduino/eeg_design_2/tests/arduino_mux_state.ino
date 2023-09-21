
#define BIT(n, i) (n >> i & 1);

const int TOTAL_PINS = 64;
const int PINS_TYPES[TOTAL_PINS] = { 2, 3, 1, 3, 2, 3, 1, 0, 3, 0, 3, 0, 3, 0, 3, 0, 1, 3, 2, 3, 1, 3, 2, 0, 3, 0, 3, 0, 3, 0, 3, 0, 2, 3, 1, 3, 2, 3, 1, 0, 3, 0, 3, 0, 3, 0, 3, 0, 1, 3, 2, 3, 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const int TOTAL_SELECT_PINS = 6;
const int ARDUINO_SEL_PINS[TOTAL_SELECT_PINS] = { 2, 3, 4, 5, 6, 7 };
const int ARDUINO_DATA_PIN = A4;

const int MUX_SELECT_DELAY_MS = 10;

int TOTAL_DATA_PINS = 0;

// output int array of given capacity
void print_int_array( int values[], int capacity ) {
	for (int index = 0; index < capacity; index++) {
		Serial.print(values[index]);
		if (index != capacity - 1) {
			Serial.print(",");
		}
	}
	Serial.println("");
}

// select using bits
void set_mux_select(int s1, int s2, int s3, int s4, int s5, int s6) {
	digitalWrite( ARDUINO_SEL_PINS[0], s1 );
	digitalWrite( ARDUINO_SEL_PINS[1], s2 );
	digitalWrite( ARDUINO_SEL_PINS[2], s3 );
	digitalWrite( ARDUINO_SEL_PINS[3], s4 );
	digitalWrite( ARDUINO_SEL_PINS[4], s5 );
	digitalWrite( ARDUINO_SEL_PINS[5], s6 );

	// output mux select
	Serial.print("MUX-SELECT: ");
	Serial.print(s1);
	Serial.print(s2);
	Serial.print(s3);
	Serial.print(s4);
	Serial.print(s5);
	Serial.println(s6);

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
	for (int pin_index = 0; pin_index < TOTAL_PINS - 1; pin_index++) {
		// if its a read pin
		if (PINS_TYPES[pin_index] == 3) {
			// read the pin's value
			mux_select_data_pin(pin_index);
			readings[index] = read_data_pin();
			index += 1;
		}
	}
}

void setup() {
	Serial.begin(115200);

	// set analog 0 as input
	pinMode(A0, INPUT);

	// set all select digital pins as output
	for (int i = 0; i < TOTAL_SELECT_PINS - 1; i++) {
		pinMode(ARDUINO_SEL_PINS[i], OUTPUT);
	}

	// count
	TOTAL_DATA_PINS = 0;
	for (int pin_index = 0; pin_index < TOTAL_PINS - 1; pin_index++) {
		if (PINS_TYPES[pin_index] == 3) {
			TOTAL_DATA_PINS += 1;
		}
	}
}

int COUNTER = 0;

void loop() {
	COUNTER += 1;
	Serial.println(COUNTER);

	// readings array of n items
	int readings[TOTAL_DATA_PINS];
	for (int i = 0; i < TOTAL_DATA_PINS - 1; i++) readings[i] = 0;

	// update the readings
	update_readings(readings, TOTAL_DATA_PINS);

	// output the readings
	Serial.print("READINGS: ");
	print_int_array( readings, TOTAL_DATA_PINS );
	delay(20);
}
