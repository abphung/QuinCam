from ICamera import ICamera
from Response import Response
from QuinCamHelpers import package
from ResponseStatus import ResponseStatus
from PIL import Image
from io import BytesIO
from types import FunctionType
import time
import traceback

class Camera(ICamera):

	def __init__(self, serial):
		self.ser = serial

	#should take in a camera config file to set the different camera settings
	def take_picture(self, savepath = None):
		response = Response()
		packaged = package('take_picture -n')
		start_time, bytes_written = self.ser.timed_write(packaged)
		try:
			end_time, length = self.ser.serial_read_int(32)
			response.response_time = end_time - start_time
			image_bytes = self.ser.read(length)
			print(f"length is: {length}")#capped at 9600 for some reason
			if length != len(image_bytes):
				print(length, len(image_bytes))
				raise Exception("expected " + str(length) + " bytes but reveived " + str(len(image_bytes)) + " instead")
			response.content = Image.open(BytesIO(image_bytes))
			if savepath != None:
				filename = savepath + str(time.time()) + ".jpg"
				response.content.save(filename)
			response.status = ResponseStatus.Succeeded
		except Exception as inst:
			response.response_time = time.time() - start_time
			response.status = ResponseStatus.Failed
			response.exception = inst
			traceback.print_exc()
			raise
		finally:
			print(response.__dict__)
			return response

	def func_which_takes_int(self, function_name: str, level: int) -> int:
		response = Response()
		p = package(f'{function_name} {level}')
		try:
			start_time, bytes_written = self.ser.timed_write(p)
			end_time, content = self.ser.serial_read_int(32)
			response.time = end_time - start_time
			response.content = content
			response.status = ResponseStatus.Succeeded
		except Exception as inst:
			#Do not move this assignment into the if statement. We want to measure the time as soon as possible.
			response.status = ResponseStatus.Failed
			response.exception = inst
		finally:
			print(response.__dict__)
			return response