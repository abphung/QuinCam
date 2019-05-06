from abc import ABC

class ICamera:

	def take_picture(self):
		raise NotImplementedError()