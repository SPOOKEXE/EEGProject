
#include "simplecomm.h"
#include "Arduino.h"

simplecomm::simplecomm(Stream &serial) {
	ser = serial;
}

String simplecomm::read( ) {
	String message = "";
	for (int index = 0; index < MAX_BUFFER; index++) {
		// end message
		if ( messageBuffer[index] == 0 ) {
			break;
		}
		// append character
		message += messageBuffer[index];
	}
	return message;
}

void simplecomm::write( String message ) {
	ser.print( message );
}

void simplecomm::update() {
	// check if there are serial characters available
	if (ser.available() < 1) {
		return;
	}

	// append characters to buffer
	while ( ser.available() > 0 ) {

		// read character
		char value = ser.read();

		// if its the end marker
		if (value == END_CHAR) {
			Serial.print("END CHAR");
			readInProgress = false;
			messageAvailable = true;
			// make sure current spot has a value
			// non-char item here so we can ignore it
			rawBuffer[bytesRecieved] = 0; 
			// copy from raw buffer to the message buffer
			strcpy(messageBuffer, rawBuffer);
		}

		// if reading
		if (readInProgress) {
			rawBuffer[bytesRecieved] = value;
			bytesRecieved++;
			Serial.print( bytesRecieved );
			// only overwrite the last byte, cannot extend pass the buffer.
			if (bytesRecieved == MAX_BUFFER) {
				bytesRecieved = MAX_BUFFER - 1;
			}
		}

		// if its the start marker
		if (value == START_CHAR) {
			Serial.print("START CHAR");
			bytesRecieved = 0;
			readInProgress = true;
			messageAvailable = false;
		}

	}
}
