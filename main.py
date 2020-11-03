import re
from funcs import *

# Variables y regex globales
re_module_name = r'module\s+([_a-zA-Z]\w*)'
re_inout = r'((input|output|inout)(\s+(reg|logic))*\s*(\[\d+:\d+\]\s*|\s+)(?!input|output|inout)([_a-zA-Z]\w*(,\s*(?!input|output|inout)[_a-zA-Z]\w*)*))'

# Diccionarios globales
input_dicc = {}
output_dicc = {}
inout_dicc = {}


# (input|output|inout)[\s]+([^;,\)]+)[\s]*[;,])


f = open("pc.sv", 'r')
text = f.read()
moduleName = re.findall(re_module_name, text)[0]
inout_list = re.findall(re_inout, text)

for m in inout_list:
    print(m)
f.close()
# tb = open("pc_tb.sv", 'w')
# tb.write(getTBString(moduleName))
# tb.close()
# print(moduleName)
