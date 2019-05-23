from enum import Enum

class PixelFormat(Enum):
	PIXFORMAT_RGB565 = 0    # 2BPP/RGB565
	PIXFORMAT_YUV422 = 1    # 2BPP/YUV422
	PIXFORMAT_GRAYSCALE = 2 # 1BPP/GRAYSCALE
	PIXFORMAT_JPEG = 3      # JPEG/COMPRESSED
	PIXFORMAT_RGB888 = 4    # 3BPP/RGB888
	PIXFORMAT_RAW = 5       # RAW
	PIXFORMAT_RGB444 = 6    # 3BP2P/RGB444
	PIXFORMAT_RGB555 = 7    # 3BP2P/RGB555