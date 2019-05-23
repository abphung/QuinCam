from enum import Enum, auto

class ResponseStatus(Enum):
	NotStarted = auto()
	Succeeded = auto()
	Failed = auto()