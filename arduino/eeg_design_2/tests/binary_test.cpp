
// PURE C++ IMPLEMENTATION

#include <bits/stdc++.h>
#include <iostream>

void set_arduino_mux( const int s6, const int s5, const int s4, const int s3, const int s2, const int s1 ) {
	std::cout << "MUX-SEL: " << s6 << s5 << s4 << s3 << s2 << s1 << std::endl;
}

void read_data_pin( int pinIndex ) {
	std::bitset dat_bits = std::bitset<4>(pinIndex % 16);
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
}

int main() {

	for (int pinIndex = 0; pinIndex < 63; pinIndex++) {
		std::cout << "PIN INDEX: " << pinIndex << std::endl;
		read_data_pin( pinIndex );
	}

	return 0;
}
