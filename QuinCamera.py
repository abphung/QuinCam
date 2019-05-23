from ICamera import ICamera
from Camera import Camera
import time
import threading


class QuinCamera(ICamera):

	#input: open serial ports
	def __init__(self, serials):
		self.cameras = map(Camera, serials)

	def take_picture(self, savepath = None):
		for camera in self.cameras:
			camera.take_picture(savepath)

	#should take picture open the port and maintain it?
	def take_delayed_picture(self, delay, savepath = None):
		call_time = time.time() + delay
		self.take_picture_at_time(call_time, savepath)

	def take_picture_at_time(self, call_time, savepath = None):
		for camera in self.cameras:
			threading.Timer(call_time - time.time(), camera.take_picture, [savepath]).start()
		time.sleep(call_time - time.time() + 1)