def package(content):
	return str.encode(chr(len(content.split(" "))) + " " + content + chr(0))