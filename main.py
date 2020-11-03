import re
from funcs import *

f = open("pc.sv", 'r')
text = f.read()
moduleName = re.findall(r'module\s+([_a-zA-Z]\w*)', text)[0]
f.close()
tb = open("pc_tb.sv", 'w')
tb.write(getTBString(moduleName))
tb.close()
print(moduleName)
