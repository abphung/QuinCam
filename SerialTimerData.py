from dataclasses import dataclass
import time

@dataclass
class SerialTimerData:
	time: time.time
	read_bytes: bytes