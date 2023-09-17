
import serial
import time

arduino = serial.Serial(port='COM9', baudrate=115200, timeout=2)

def write_read( x : str ) -> str:
	arduino.write(bytes(x, 'utf-8'))
	time.sleep(0.05)
	data = arduino.readline()
	return data.decode('utf-8')

while True:
	num = input("Enter a value: ")
	value = write_read(num)
	print(value)
