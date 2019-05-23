from SerialTimer import SerialTimer
from PIL import Image
import asyncio
import sys
from Response import Response
from ResponseStatus import ResponseStatus
import io
from Camera import Camera
from contextlib import ExitStack
from QuinCamera import QuinCamera
from FrameSize import FrameSize

class PortManager:

	def __init__(self, ports):
		with ExitStack() as stack:
			#use call-in line(tty) instead of call-out line(cu)
			devices = [port.device.replace("cu", "tty") for port in ports]
			serials = []
			for device in devices:
				try:
					serials.append(stack.enter_context(SerialTimer(device, baudrate=115200, timeout=1)))
				except:
					#print(sys.exc_info())
					pass
			print(f"Taking picture with {len(serials)} cameras")
			# quin_camera = QuinCamera(serials)
			# delay = 1
			# quin_camera.take_delayed_picture(delay, "Images/")
			Camera(serials[0]).func_which_takes_int("set_framesize", FrameSize.SVGA)
			Camera(serials[0]).take_picture("Images/")