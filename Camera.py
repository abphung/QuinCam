from ICamera import ICamera
from Response import Response
from QuinCamHelpers import package
from ResponseStatus import ResponseStatus
from PIL import Image
from io import BytesIO

class Camera(ICamera):

	def __init__(self, serial):
		self.ser = serial

	#should take in a camera config file to set the different camera settings
	def take_picture(self, savepath = None):
		response = Response()
		p = package('TakePicture -n')

		try:
			start_time, bytes_written = self.ser.timed_write(str.encode(p))
			end_time, length = self.ser.serial_read_int(32)
			response.time = end_time - start_time
			image_bytes = self.ser.read(length)
			if length != len(image_bytes):
				print(length, len(image_bytes))
				raise Exception("expected " + str(length) + " bytes but reveived " + str(len(image_bytes)) + " instead")
			response.content = Image.open(BytesIO(image_bytes))
			if savepath != None:
				response.content.save(savepath)
			response.status = ResponseStatus.Succeeded
		except Exception as inst:
			#Do not move this assignment into the if statement. We want to measure the time as soon as possible.
			response.status = ResponseStatus.Failed
			response.exception = inst
		finally:
			print(response.__dict__)
			return response