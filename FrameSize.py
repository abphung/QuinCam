from enum import IntEnum

class FrameSize(IntEnum):
	QQVGA = 0    # 160x120
	QQVGA2 = 1   # 128x160
	QCIF = 2     # 176x144
	HQVGA = 3    # 240x176
	QVGA = 4     # 320x240
	CIF = 5      # 400x296
	VGA = 6      # 640x480
	SVGA = 7     # 800x600
	XGA = 8      # 1024x768
	SXGA = 9     # 1280x1024
	UXGA = 10    # 1600x1200
	QXGA = 11    # 2048*1536