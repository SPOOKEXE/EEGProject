
from time import sleep
from pySerialTransfer import pySerialTransfer as txfer

if __name__ == '__main__':
	try:
		link = txfer.SerialTransfer('COM13')

		link.open()
		sleep(2) # allow some time for the Arduino to completely reset

		link.txBuff[0] = 'h'
		link.txBuff[1] = 'i'
		link.txBuff[2] = '\n'

		link.send(3)

		while not link.available():
			if link.status < 0:
				print('ERROR: {}'.format(link.status))

		print('Response received:')

		response = ''
		for index in range(link.bytesRead):
			response += chr(link.rxBuff[index])
		
		print(response)
		link.close()
		
	except KeyboardInterrupt:
		link.close()
