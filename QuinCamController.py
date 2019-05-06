from SerialTimer import SerialTimer
from PIL import Image
import time
import threading
import asyncio
import sys
from Response import Response
from ResponseStatus import ResponseStatus
import io
from Camera import Camera
from contextlib import ExitStack

def get_time():
	response = Response()
	p = package('GetTime -n')
	start_time = time.time()

	try:
		ser.write(str.encode(p))
		response.content = serial_read_int(64)
		#this can be moved closer to the first ser.read for more accuracy
		response.time = time.time() - start_time
		print(time)
	except:
		#Do not move this assignment into the if statement. We want to measure the time as soon as possible.
		response_time = time.time() - start_time
		if response.time == None:
			response.time = response_time
		response.status = ResponseStatus.Failed
		response.exception = sys.exc_info()
	finally:	
		response.status = ResponseStatus.Succeeded
	return response
	
if __name__ == "__main__":
	with ExitStack() as stack:
		ser1 = stack.enter_context(SerialTimer('/dev/tty.QuinCam1-ESP32SPP', baudrate=115200, timeout=1))
		ser2 = stack.enter_context(SerialTimer('/dev/tty.QuinCam2-ESP32SPP', baudrate=115200, timeout=1))
		Camera(ser1).take_picture("Images/Cam1" + str(time.time()) + ".jpg")
		Camera(ser2).take_picture("Images/Cam2" + str(time.time()) + ".jpg")
	# with SerialTimer('/dev/tty.QuinCam2-ESP32SPP', baudrate=115200, timeout=1) as ser2:
	# 	with SerialTimer('/dev/tty.QuinCam1-ESP32SPP', baudrate=115200, timeout=1) as ser1:	
	# 		#perform action
	# 		Camera(ser1).take_picture("Cam1" + str(time.time()) + ".jpg")
	# 		Camera(ser2).take_picture("Cam2" + str(time.time()) + ".jpg")

# 
#     foo = stack.enter_context(open("foo.txt"))
#     bar = stack.enter_context(open("bar.txt"))
#     baz = stack.enter_context(open("baz.txt"))