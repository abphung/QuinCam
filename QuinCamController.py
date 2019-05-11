from SerialTimer import SerialTimer
from serial.tools import list_ports
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
	ports = list(list_ports.grep("QuinCam", True))
	with ExitStack() as stack:
		#use call-in line(tty) instead of call-out line(cu)
		devices = [port.device.replace("cu", "tty") for port in ports]
		serials = [stack.enter_context(SerialTimer(device, baudrate=115200, timeout=1)) for device in devices]
		cameras = map(Camera, serials)
		call_time = time.time()
		for camera in cameras:
			threading.Timer(call_time - time.time(), camera.take_picture, ["Images/" + str(time.time()) + ".jpg"]).start()
		input()