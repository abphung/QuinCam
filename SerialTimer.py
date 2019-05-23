from serial import Serial
from time import time
from SerialTimerData import SerialTimerData

class SerialTimer(Serial):

	def timed_write(self, data):
		return (time(), self.write(data))

	def timed_read(self, size=1):
		return SerialTimerData(time(), self.read(size))

	def serial_read_int(self, bits):
		read_data = self.timed_read(int(bits/8))
		return (read_data.time, int.from_bytes(read_data.read_bytes, byteorder='little'))