
import sys
import re
from strFuncs import *
from displayMenu import *
from datetime import datetime

# Get the current time
current_time = datetime.now()
date_time = current_time.strftime("%m/%d/%Y, %H:%M:%S")

# Global regex and variables
re_com = r'\/\/[^\n]*|\/\*((?!\*\/).)*\*\/'

re_module_name = r'module\s+([_a-zA-Z]\w*)'

'''
1st group --> input|ouput|inout
3rd group --> logic | reg
4th group --> bus size
5th group --> variables separated by coma
'''
# re_inout = r'(input|output|inout)(\s+(reg|logic))?(\s*\[\d+:\d+\]\s*|\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\w*(,\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\w*)*)'
re_inout = r'(input|output|inout)(\s+(reg|logic))?(\s*\[[^\]]+\]\s*|\s+)((?!input|output|inout|reg|logic)[_a-zA-Z]\w*(,\s*(?!input|output|inout|reg|logic)[_a-zA-Z]\w*)*)'

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

    # Override definition
    fOverride = False
    scaleOverride = False
    clkOverride = False
    rstOverride = False
    forOverride = False

    # Read args
    if len(sys.argv) <= 1:
        print("Missing arguments!")
        sys.exit(0)
    elif len(sys.argv) == 2:
        if sys.argv[1] == "--help":
            printHelp()
            sys.exit(0)
        inputFile = sys.argv[1]
    else:
        for i in range(1, len(sys.argv)):
            if sys.argv[i][0] == '-':
                if sys.argv[i] == "--help":
                    printHelp()
                    sys.exit(0)
                for j in range(1, len(sys.argv[i])):
                    if sys.argv[i][j] == 'r':
                        funcOverride = 'random'
                        fOverride = True
                    elif sys.argv[i][j] == 'a':
                        funcOverride = 'up'
                        fOverride = True
                    elif sys.argv[i][j] == 'd':
                        funcOverride = 'down'
                        fOverride = True
                    elif sys.argv[i][j] == 't':
                        scaleOverride = True
                    elif sys.argv[i][j] == 'c':
                        clkOverride = True
                    elif sys.argv[i][j] == 's':
                        rstOverride = True
                    elif sys.argv[i][j] == 'f':
                        forOverride = True
            else:
                inputFile = sys.argv[i]

    # Open the design file
    f = open(inputFile, 'r')
    textC = f.read()  # Read it as a string
    f.close()  # Close the file

    print("Welcome to the testbench generator!")

    # Clk name
    clk = "clk"
    if clkOverride:
        clk = getClk()

    # (rst name, active HIGH?)
    rst = ("rst", True)
    if rstOverride:
        rst = getRst()

    # Remove comments from the file
    text = re.sub(re_com, "", textC, flags=re.DOTALL)

    # Get the module name
    moduleName = re.findall(re_module_name, text)[0]
    # Get all parameters
    params_list = re.findall(re_parameters, text)
    # Get all the inputs and outputs in the text
    inout_list = re.findall(re_inout, text)

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
                hasClk = varTuple[0] == clk
            if not hasRst:
                hasRst = varTuple[0] == rst[0]

            # Save the tuple in the appropriate dictionary
            if(m[0] == "input"):
                if varTuple[0] != clk and varTuple[0] != rst[0]:
                    if fOverride:
                        varTuple[3] = funcOverride
                    else:
                        varTuple[3] = displayMenu(varTuple)
                input_dicc[varTuple[0]] = varTuple
            elif(m[0] == "output"):
                output_dicc[varTuple[0]] = varTuple
            else:  # inout
                inout_dicc[varTuple[0]] = varTuple

    # User type in iterator for simulation values
    forIt = 10
    if forOverride:
        forIt = selectForIterations()

    # User type in the timescale for the testbench
    scale = "1ns/1ps"
    if scaleOverride:
        scale = timescale()

    # Generate variable delcarations in SystemVerilog format
    regStr = generateInputTb(input_dicc, inout_dicc)
    wireStr = generateOutputTb(output_dicc)
    varInit = variableInit(input_dicc, clk, rst)
    mainSequence = generateMainSequence(
        input_dicc, forIt, clk, rst)

    # Open the test bench file in utf8 encoding
    tbName = inputFile[0:len(inputFile)-3]+"_tb.sv"
    tb = open(tbName, 'w', encoding='utf8')

    # Write the file with the string formatted appropriately
    tb.write(getTBString(date_time, moduleName, paramsStr, regStr,
                         wireStr, hasClk, hasRst, varInit, mainSequence, scale, clk, rst))

    # Close the file
    tb.close()
    print("Done")
