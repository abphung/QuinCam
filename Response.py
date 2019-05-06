from ResponseStatus import ResponseStatus

class Response:

	def __init__(self):
		self.time = None
		self.status = ResponseStatus.NotStarted
		self.exception = None
		self.content = None