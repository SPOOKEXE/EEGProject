
#define BIT(n, i) (n >> i & 1);

const int TOTAL_SELECT_PINS = 6;
const int ARDUINO_SEL_PINS[TOTAL_SELECT_PINS] = { 2, 3, 4, 5, 6, 7 };
const int ARDUINO_DATA_PIN = A4;

const int MUX_SELECT_DELAY_MS = 10;

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

void setup() {
	Serial.begin(115200);

	// set analog 0 as input
	pinMode(A0, INPUT);

	// set all select digital pins as output
	for (int i = 0; i < TOTAL_SELECT_PINS - 1; i++) {
		pinMode(ARDUINO_SEL_PINS[i], OUTPUT);
	}
}

void loop() {
	// reading
	mux_select_data_pin(0);
	int reading = read_data_pin();

	// output the readings
	Serial.print("READINGS: ");
	Serial.println(reading);
	delay(20);
}
