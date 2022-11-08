import odrive
from odrive.enums import *
import time

oDrive1 = odrive.find_any()

def configure_CAN():
    oDrive1.axis0.config.can.node_id = 1
    oDrive1.axis1.config.can.node_id = 3
    oDrive1.can.config.baud_rate = 500000
    oDrive1.save_configuration()

configure_CAN()