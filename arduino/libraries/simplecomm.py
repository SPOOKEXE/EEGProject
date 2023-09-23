
import serial

from time import sleep
from threading import Thread

MAX_CHAR_BUFFER = 400
MAX_STR_BUFFER = 30

START_CHAR = b"="
END_CHAR = b"+"

def array_find( array, value ) -> int:
	try:
		return array.index( value )
	except:
		return -1

def array_slice( original : list, start : int, finish : int ) -> tuple:
	return original[start+1:finish], original[finish + 1:]

class simplecomm:

	def __init__(self):
		self.serial = None
		self._thread = None

		self.rawBuffer = []
		self.messages = []

	def _update_loop( self ) -> None:
		'''
		Core update loop - ran in another thread.
		'''
		while self.serial and self.serial.is_open:
			# read buffer
			buffer = self.serial.read_all()
			if buffer != None:
				self.rawBuffer.extend( [ b.decode('utf-8') for b in buffer] )
			# max buffer check
			diff = len(self.rawBuffer) - MAX_CHAR_BUFFER
			if diff > 0:
				self.rawBuffer = self.rawBuffer[diff:]
			# check for messages message
			start = array_find( self.rawBuffer, START_CHAR )
			while start != -1:
				finish = array_find( self.rawBuffer, END_CHAR )
				if finish != -1:
					segment, new_list = array_slice( self.rawBuffer, start, finish )
					self.rawBuffer = new_list
					message = "".join( [ v.decode("utf-8") for v in segment ] )
					self.messages.append( message )
			# message buffer
			diff = len(self.messages) - MAX_STR_BUFFER
			if diff > 0:
				self.messages = self.messages[diff:]
			# delay
			sleep( 0.01667 )

	def open( self, COM : str, baudrate : int ) -> None:
		'''
		Open the serial.
		Raises AssertionError when the serial is already open.
		'''
		assert self.serial == None, "Serial is already open."
		self.serial = serial.Serial( COM, baudrate )
		self._thread = Thread(target=self._update_loop, args=())
		self._thread.start()

	def close( self ) -> None:
		'''
		Close the seriel.
		Raises AssertionError when the serial is not open.
		'''
		assert self.serial != None, "Serial is not currently open."
		self.serial.close()
		self.serial = None

	def flushBuffer( self ) -> list:
		'''
		Flush the raw buffer and return the buffer items.
		'''
		buffer = self.rawBuffer
		self.rawBuffer = []
		return buffer

	def readAll( self ) -> list[str]:
		'''
		Read all available messages in the Arduino message buffer.
		Clears the message buffer.
		'''
		messages = self.messages
		self.messages = []
		return messages

	def popRead( self ) -> str | None:
		'''
		Read any available message in the Arduino message buffer.
		Pops the first element in the buffer, otherwise, returns None.
		'''
		try:
			return self.messages.pop(0)
		except:
			return None

	def yieldRead( self ) -> str:
		'''
		Yield until a complete message comes from the arduino and return it.
		'''
		while len(self.messages) == 0:
			sleep(0.1)
		return self.messages.pop(0)

	def write( self, message : str | bytes ) -> None:
		'''
		Write a message to the Arduino.
		'''
		assert self.serial != None, "Serial is not currently open."
		if isinstance( message, str ):
			message = message.encode('utf-8')
		self.serial.write( START_CHAR + message + END_CHAR )

	def writeRead( self, message : str | bytes ) -> str:
		'''
		Write the message to the arduino and read the next response.
		'''
		assert self.serial != None, "Serial is not currently open."
		self.write( message )
		return self.yieldRead()

if __name__ == '__main__':

	comm = simplecomm()
	comm.open("COM6", 115200)

	response = comm.writeRead("Hello Arduino!")
	print( response )

	comm.write("Hello Arduino!")
	response = comm.yieldRead()
	print( response )
