import logging
from lecore.LeBin.VisualLeBin import *
from lecore.LeBin.SerialCom import *

# Chnage logger level just for errors
logger = logging.getLogger()
logger.setLevel(logging.ERROR)

# Find and open com port
com = SerialCom()
port = com.get_list_ports(pid=22336, sn="BM03TESTER")
com.open_com(port[0], 1)

# Create register map object
regs = RegisterMap(com)
regs.load('Bm03Tester_registers.json')

# Run visual application
if __name__ == '__main__':
    vis = VisualLeBin(regs)
    vis.draw(height=700)
    vis.handle()
