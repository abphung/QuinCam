import serial
from PIL import Image
import cStringIO as StringIO
import os
import time
import threading

def foo():
    print(time.ctime())
    threading.Timer(1000, foo).start()

def package(content):
	return chr(len(content.split(" "))) + " " + content + chr(0)

#bits must be multiples of 8
def serial_read_int(bits):
	return sum([x*2**(8*i) for i, x in enumerate(map(ord, ser.read(bits/8)))])

def take_picture():
	p = package('TakePicture -n')
	print p
	ser.write(p)
	length = serial_read_int(32)
	print length
	try:
		os.remove("test.jpg") 
	except:
		pass
	f = open("test.jpg", "a")
	for i in range(length):
		f.write(ser.read())
	f.close()

def get_time():
	p = package('GetTime -n')
	print p
	ser.write(p)
	time = serial_read_int(64)
	print time
	

if __name__ == "__main__":
	ser = serial.Serial('/dev/tty.ESP32test-ESP32SPP', timeout=1)
	#clear out serial port. for some reason flush does not do this correctly
	while ser.read():
		pass
	#do something
	take_picture()
	ser.close()	