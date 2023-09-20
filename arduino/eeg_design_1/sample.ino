
// convert string 0/1 to the integer form
int binCharToInt( char item ) {
	return ((int) item) - 48;
}

// select using binary string (001000) HIGH/LOW
void binary_mux_select( String bin ) {
	set_mux_select(
		binCharToInt( bin.charAt(5) ),
		binCharToInt( bin.charAt(4) ),
		binCharToInt( bin.charAt(3) ),
		binCharToInt( bin.charAt(2) ),
		binCharToInt( bin.charAt(1) ),
		binCharToInt( bin.charAt(0) )
	);
}
