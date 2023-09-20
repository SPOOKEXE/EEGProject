
import serial
import time
import json

class Commands:
	START_READER = 'x1'
	STOP_READER = 'x2'
	GET_DATA_PINS = 'x3'

class Arduino:

	def __init__( self, port='COM6', baudrate=115200, timeout=0.1 ):
		self.arduino = serial.Serial(
			port=port,
			baudrate=baudrate,
			timeout=timeout
		)

	def write_lines( self, value : str ) -> None:
		self.arduino.writelines([ bytes(value, 'utf-8') ])

	def read_line( self ) -> str:
		return self.arduino.read_until("\n").decode('utf-8')

	def write_read( self, value : str ) -> str:
		self.write_lines( value )
		# time.sleep(0.05)
		return self.read_line()

	def request_data_pins( self ) -> str:
		return self.write_read( self, Commands.GET_DATA_PINS )

	def start_reading( self ) -> None:
		self.write_lines( Commands.START_READER )

	def stop_reading( self ) -> None:
		self.write_lines( Commands.STOP_READER )

class Dataset:

	def __init__(self):
		self.data = [ ]

	def write( self, timestamp : float, readings : list ) -> None:
		self.data.append( (timestamp, readings) )

	def read( self, start=-1, finish=-1 ) -> list:
		values : list[tuple] = self.data
		if start != -1:
			values = values[start:]
		if finish != -1:
			values = values[:finish]
		return values.copy() # disallow editing of source data

	def serialize( self ) -> str:
		return json.dumps(self.data)
	
	def deserialize( self, encoded : str ) -> None:
		self.data = json.loads( encoded )

def eeg_arduino_runtime() -> None:

	arduino = Arduino()

	# read the data pin layout
	result = arduino.request_data_pins( )
	print(result)

	# enable readings mode
	arduino.start_reading( )

	# output readings
	while True:
		out = arduino.read_line()
		print(out)

if __name__ == '__main__':
	eeg_arduino_runtime()
