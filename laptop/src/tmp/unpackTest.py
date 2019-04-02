
import numpy as np
import scipy.misc
import struct





my_byte = "\x01"

pixels = (struct.unpack('>H', b'\x00' + my_byte))
print pixels

print ord(my_byte)