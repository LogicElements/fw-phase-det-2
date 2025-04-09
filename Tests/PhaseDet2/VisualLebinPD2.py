from PhaseDet2 import *

dev = PhaseDet2()
ports = dev.search_com()
dev.open(comport=ports[0])
dev.visual(None, height=700)
