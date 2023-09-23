
// maximum characters before automatically stops reading until line terminator
#define MAX_BUFFER 400

#ifndef simplecomm_h
#define simplecomm_h

#include <Arduino.h>

class simplecomm {

	public:
		simplecomm(Stream &serial);

		int bytesRecieved = 0;
		char rawBuffer[MAX_BUFFER]; // raw serial line

		Stream& ser;
		bool readInProgress = false;
		bool messageAvailable = false;
		char messageBuffer[MAX_BUFFER];

		const char START_CHAR = "=";
		const char END_CHAR = "-";

		String read(); // read from serial
		void write( String message ); // write to serial
		void update();
};

#endif
