// Read input sensors
// Current: transmit over data ports
// TODO: Transmit over ethernet/bluetooth to receiver device

// https://www.arduino.cc/reference/en/libraries/ethernet/ethernet.begin/
// https://www.electronics-lab.com/get-sensor-data-arduino-smartphone-via-bluetooth/

/*
	const int MAP_FROM_0_TO = 5;
	void setup() {
		Serial.begin(9600); // read at 9600 bits of data per second
	}
	void loop() {
		int sensorValue = analogRead(A0); // read the resistance value (between 0 and 1023)
		double voltage = sensorValue * (ARDUINO_VOLTAGE / 1023.0); // map from 0 and 1023, to 0.0 and "MAP_FROM_0_TO"
		Serial.println(voltage); // output voltage
	}
*/

const int INPUT_READING_DELAY_MS = 10; // try keep this low

const int ReadBit = A0;
const int ControlBits[4] = { A1, A2, A3, A4 };
const bool INPUT_1_BOOL_ARRAY[4] = { true, false, false, false };	// TFFF
const bool INPUT_2_BOOL_ARRAY[4] = { true, true, false, false };	// TTFF
const bool INPUT_3_BOOL_ARRAY[4] = { true, true, true, false };		// TTTF
const bool INPUT_4_BOOL_ARRAY[4] = { true, true, true, true };		// TTTT
const bool INPUT_5_BOOL_ARRAY[4] = { false, true, false, false };	// FTFF
const bool INPUT_6_BOOL_ARRAY[4] = { false, true, true, false };	// FTTF
const bool INPUT_7_BOOL_ARRAY[4] = { false, true, true, true };		// FTTT
const bool INPUT_8_BOOL_ARRAY[4] = { false, false, true, true };	// FFTT
const bool INPUT_9_BOOL_ARRAY[4] = { false, false, false, true };	// FFFT
const bool INPUT_10_BOOL_ARRAY[4] = { false, false, false, false };	// FFFF

// Utilities //
double round_places(double num, int dec_places) {
	int decimals = pow(10, dec_places);
	return (double) (floor(num * decimals) / decimals);
}

double get_reading(const bool ControlArray[]) { // Each Multiplex Option
	for (int index = 0; index < 4; index++) {
	// Serial.print(ControlBits[index]-14, ">", (ControlArray[index] ? 1 : 0), "|");
		digitalWrite(ControlBits[index], (ControlArray[index] ? HIGH : LOW));
	}
	delay(INPUT_READING_DELAY_MS); // slight delay for current to change (also quality)
	return analogRead(ReadBit);
}

void update_reading_array(double inputs[]) { // Multiplexer Control
	// Multiplexer.xlsx (10 input bits, 4 control bits, 1 output bit)
	inputs[0] = get_reading(INPUT_1_BOOL_ARRAY); // TFFF
	inputs[1] = get_reading(INPUT_2_BOOL_ARRAY); // TTFF
	inputs[2] = get_reading(INPUT_3_BOOL_ARRAY); // TTTF
	inputs[3] = get_reading(INPUT_4_BOOL_ARRAY); // TTTT
	inputs[4] = get_reading(INPUT_5_BOOL_ARRAY); // FTFF
	inputs[5] = get_reading(INPUT_6_BOOL_ARRAY); // FTTF
	inputs[6] = get_reading(INPUT_7_BOOL_ARRAY); // FTTT
	inputs[7] = get_reading(INPUT_8_BOOL_ARRAY); // FFTT
	inputs[8] = get_reading(INPUT_9_BOOL_ARRAY); // FFFT
	inputs[9] = get_reading(INPUT_10_BOOL_ARRAY); // FFFF
}

// MAIN
void setup() {
	Serial.begin(9600);  // read at 9600 bits of data per second

	// Setup multiplexer reader inputs/outputs
	pinMode(ReadBit, OUTPUT);  // read from this
	for (uint8_t bit : ControlBits) {
		pinMode(bit, INPUT);  // write to this
	}
}

auto last = millis();
void loop() {
	// get new readings
	double readings[10] {};
	update_reading_array(readings);

	// output update interval
	auto deltaTime = millis() - last;
	auto rounded_deltaTime = round_places((double) (deltaTime / 1e3), 3);
	Serial.print("INTERVAL: ");
	Serial.println(rounded_deltaTime);
	last = millis();

	// output readings
	Serial.print("READING");
	for (double reading : readings) {
		Serial.print("-");
	Serial.print(reading);
	}
	Serial.println();
}
