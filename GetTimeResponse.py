from ResponseStatus import ResponseStatus

class GetTimeResponse:

	def __init__(self):
		self.response_time = 0
		self.status = ResponseStatus.NotStarted
		self.response_content = None