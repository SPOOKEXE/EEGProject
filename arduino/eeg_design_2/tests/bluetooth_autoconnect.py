import bluetooth

name = "ESP32test"
addr = 'c8:f0:9e:a2:5c:da'
port = 1
passphrase = "password"

# print( bluetooth.lookup_name(addr) )

try:
	bt_sock = bluetooth.BluetoothSocket() 
	bt_sock.connect((addr, port)) 
except bluetooth.BluetoothError as err:
	print(err)
	exit()

bt_sock.send(b"Hello there!")
bt_sock.recv(1024)
