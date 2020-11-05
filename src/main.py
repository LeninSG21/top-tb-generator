
import sys
import re
from strFuncs import *
from displayMenu import *

# Global regex and variables
re_module_name = r'module\s+([_a-zA-Z]\w*)'

'''
1st group --> input|ouput|inout
3rd group --> logic | reg
4th group --> bus size
5th group --> variables separated by coma
'''
# re_inout = r'(input|output|inout)(\s+(reg|logic))?(\s*\[\d+:\d+\]\s*|\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\w*(,\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\w*)*)'
re_inout = r'(input|output|inout)(\s+(reg|logic))?(\s*\[.+\]\s*|\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\w*(,\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\w*)*)'

'''
1st group --> param_name
2nd group --> param_size
'''
re_parameters = r'((parameter)\s+(\w*)\s*\=\s*((\d+\'(b|h|d))?\w+))'  # r'(parameter)\s+(\w*)\s*\=\s*((\d+\'(b|h|d))?\d+)'

'''
key --> variable name
var_struct --> (name, size, type, funcType)
{
    key: var_struct
}

'''
# Global dictionaries
input_dicc = {}
output_dicc = {}
inout_dicc = {}


if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Missing input arguments!")
        sys.exit(0)
    # Argument provided by the console with the file to open
    filename = sys.argv[1]

    # Open the design file
    f = open(filename, 'r')
    text = f.read()  # Read it as a string
    f.close()  # Close the file

    print("Welcome to the testbench generator!")

    # Get the module name
    moduleName = re.findall(re_module_name, text)[0]
    # Get all parameters
    params_list = re.findall(re_parameters, text)
    # Get all the inputs and outputs in the text
    inout_list = re.findall(re_inout, text)

    # print(params_list)
    # Give propper format to parameters within the testbench
    # Iterate over parameters
    paramsStr = ""
    for par in params_list:
        paramsStr += "\n\t" + par[0] + ";"

    # Flags for clk and rst (verify if secuential or combinational verilog design)
    hasClk = False
    hasRst = False

    # Iterate over the inputs and outputs
    for m in inout_list:
        '''
        Groups in the regex match
        1st group --> input|ouput|inout
        3rd group --> logic | reg
        4th group --> bus size
        5th group --> variables separated by coma
        var_struct --> (name, size, type, funcType)
        '''
        varList = m[4].split(",")  # Get variables list
        for var in varList:  # Iterate over the variables list

            # Create the tuple to save the variable info
            varTuple = [var.strip(), m[3].strip(), m[2], '']
            if not hasClk:
                hasClk = varTuple[0] == 'clk'
            if not hasRst:
                hasRst = varTuple[0] == 'rst'

            # Save the tuple in the appropriate dictionary
            if(m[0] == "input"):
                if varTuple[0] != 'clk' and varTuple[0] != 'rst':
                    varTuple[3] = displayMenu(varTuple)
                input_dicc[varTuple[0]] = varTuple
            elif(m[0] == "output"):
                output_dicc[varTuple[0]] = varTuple
            else:  # inout
                inout_dicc[varTuple[0]] = varTuple

    # User type in iterator for simulation values
    forIt = selectForIterations()

    # Generate variable delcarations in SystemVerilog format
    regStr = generateInputTb(input_dicc, inout_dicc)
    wireStr = generateOutputTb(output_dicc)
    varInit = variableInit(input_dicc)
    mainSequence = generateMainSequence(input_dicc, forIt)

    # Open the test bench file in utf8 encoding
    tbName = filename[0:len(filename)-3]+"_tb.sv"
    tb = open(tbName, 'w', encoding='utf8')
    # Write the file with the string formatted appropriately
    tb.write(getTBString(moduleName, paramsStr, regStr,
                         wireStr, hasClk, hasRst, varInit, mainSequence))
    # Close the file
    tb.close()
    print("Done")
