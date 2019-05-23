from Tkinter import *  
from QuinCamController import QuinCamController
from PixelFormat import PixelFormat
from FrameSize import FrameSize

class CameraSettings(Frame):

	def __init__(self, root=None):
		self.controller = QuinCamController()

		self.pixel_format_label = Label(text="Pixel Format")
		pixel_formats = zip(*PixelFormat.__members__.items())[0]
		self.pixel_format_var = StringVar()
		self.pixel_format_var.set(pixel_formats[0])
		self.pixel_format_option = OptionMenu(root, self.pixel_format_var, *pixel_formats, command = self.pixel_format_option_callback)

		self.frame_size_label = Label(text="Frame Size")
		frame_sizes = zip(*FrameSize.__members__.items())[0]
		self.frame_size_var = StringVar()
		self.frame_size_var.set(frame_sizes[0])
		self.frame_size_option = OptionMenu(root, self.frame_size_var, *frame_sizes, command = self.frame_size_option_callback)

		self.labels = [self.pixel_format_label, self.frame_size_label]
		self.options = [self.pixel_format_option, self.frame_size_option]

		for i, label in enumerate(self.labels):
			label.grid(row=i, column=0)
		for i, option in enumerate(self.options):
			option.grid(row=i, column=1)

	def pixel_format_option_callback(self, event):
		self.controller.set_pixel_format(self.pixel_format_var.get())
		#print(f"new selection is {self.pixel_format_var.get()}")

	def frame_size_option_callback(self, event):
		self.frame_size_var.get()
		#print(f"new selection is {self.frame_size_var.get()}")

	
if __name__ == "__main__":
	root = Tk()
	root.protocol("WM_DELETE_WINDOW",  exit)
	CameraSettings(root)
	mainloop()