

const int TOTAL_SELECT_PINS = 6;
const int ARDUINO_SEL_PINS[TOTAL_SELECT_PINS] = { 2, 3, 4, 5, 6, 7 };

void set_mux_select( int s1, int s2, int s3, int s4, int s5, int s6 ) {
	digitalWrite( ARDUINO_SEL_PINS[0], s1 );
	digitalWrite( ARDUINO_SEL_PINS[1], s2 );
	digitalWrite( ARDUINO_SEL_PINS[2], s3 );
	digitalWrite( ARDUINO_SEL_PINS[3], s4 );
	digitalWrite( ARDUINO_SEL_PINS[4], s5 );
	digitalWrite( ARDUINO_SEL_PINS[5], s6 );
}

void binary_mux_select( String bin ) {
	set_mux_select(
		((int) bin.charAt(5)) - 48,
		((int) bin.charAt(4)) - 48,
		((int) bin.charAt(3)) - 48,
		((int) bin.charAt(2)) - 48,
		((int) bin.charAt(1)) - 48,
		((int) bin.charAt(0)) - 48
	);
}

int read_data_pin( ) {
	return analogRead( A4 ); // 0 -> 1024
}

void parse_instruction( String value ) {

	String command_bits = value.substring(0, 2);
	String data_bits = value.substring(2);

	// Serial.println(data_bits);

	if( command_bits == "00" && data_bits.length() == 0 ) {
		Serial.println( read_data_pin() );
	} else if ( command_bits == "01" && data_bits.length() == 6 ) {
		// Serial.println("MUX SET BINARY");
		// Serial.println(data_bits);
		binary_mux_select( data_bits );
	// } else {
		// Serial.println("Unknown Command");
	}
}

void setup() {
	Serial.begin(115200);
	Serial.setTimeout(1);

	pinMode( A0, INPUT );
	for (int i = 0; i < TOTAL_SELECT_PINS; i++) {
		pinMode( ARDUINO_SEL_PINS[i], OUTPUT );
	}
}

void loop() {
	if ( Serial.available() > 2 ) {
		String value = Serial.readStringUntil('\n');
		parse_instruction( value );;
	}
}
