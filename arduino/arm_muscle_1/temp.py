
import serial
import json
import time
import pendulum

class Commands:
	START_READER = 'x1'
	STOP_READER = 'x2'
	GET_DATA_PINS = 'x3'

class Arduino:

	def __init__( self, port='COM6', baudrate=115200, timeout=0.1 ):
		self.charBuffer = []
		self.arduino = serial.Serial(
			port=port,
			baudrate=baudrate,
			timeout=timeout
		)

	def write_lines( self, value : str ) -> None:
		self.arduino.writelines([ bytes(value + "\n", 'utf-8') ])

	def read_line( self ) -> str:
		return self.arduino.read_until("\n").decode('utf-8')

	def write_read( self, value : str ) -> str:
		self.write_lines( value )
		# time.sleep(0.05)
		return self.read_line()

	def request_data_pins( self ) -> str:
		value = self.write_read( Commands.GET_DATA_PINS )
		while value.find(",") == -1:
			time.sleep(0.1)
			value = self.write_read( Commands.GET_DATA_PINS )
		items = value.replace("\n", "").replace("\r", "").split(",")[1:][:-1]
		return [ int(value) for value in items ] 

	def start_reading( self ) -> None:
		self.write_lines( Commands.START_READER )

	def stop_reading( self ) -> None:
		self.write_lines( Commands.STOP_READER )

	def read_readings( self ) -> list:
		value = self.read_line()
		while value.find(",") == -1:
			time.sleep(0.1)
			value = self.read_line()
		value = value.replace("\r", "").replace("\n", "")
		return [ int(value) for value in value.split(",") if value != "" ]

class Dataset:

	def __init__(self):
		self.data = [ ]
		self.timestamp = pendulum.now('UTC')

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
		# TODO: dunno if datetime works properly
		return json.dumps({ "dateUTC" : self.timestamp.to_iso8601_string(), "data" : self.data })

	def deserialize( self, encoded : str ) -> None:
		decoded : dict = json.loads( encoded )
		self.timestamp = self.timestamp.fromisoformat( decoded.get("dateUTC") )
		self.data = decoded.get('data')

def eeg_arduino_runtime() -> None:

	arduino = Arduino()

	result = arduino.request_data_pins()
	print( result )

	dataset = Dataset()

	arduino.start_reading( )
	while True:
		readings = arduino.read_readings()
		dataset.write( time.time_ns(), readings )
		print( len(readings), readings )
		time.sleep(0.5)

if __name__ == '__main__':
	eeg_arduino_runtime()
