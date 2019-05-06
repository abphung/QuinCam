def package(content):
	return chr(len(content.split(" "))) + " " + content + chr(0)