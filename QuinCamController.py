from serial.tools import list_ports
from PortManager import PortManager

if __name__ == "__main__":
	ports = list(list_ports.grep("QuinCam", True))
	PortManager(ports)

