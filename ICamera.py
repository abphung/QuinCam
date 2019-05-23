from abc import ABC

class ICamera:

	def take_picture(self, savepath = None):
		raise NotImplementedError()

	def take_delayed_picture(self, delay, savepath = None):
		raise NotImplementedError()

	def take_picture_at_time(self, time, savepath = None):
		raise NotImplementedError()
