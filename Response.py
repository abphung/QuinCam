from dataclasses import dataclass
from ResponseStatus import ResponseStatus
from time import time

@dataclass
class Response:
	response_time: time = None
	status: ResponseStatus = ResponseStatus.NotStarted
	exception: Exception = None
	content: object = None