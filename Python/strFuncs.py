# var_struct --> (name, size, type, funcType)

# Function to generate all (regs) inputs and/or inouts within the testbench
def generateInputTb(input_dicc, inout_dicc):
    s = ""
    for i in input_dicc.values():
        s += "\treg %s %s;\n" % (i[1], i[0])
    for i in inout_dicc.values():
        s += "\treg %s %s;\n" % (i[1], i[0])
    return s

# Function to generate all (wires) outputs within the testbench


def generateOutputTb(output_dicc):
    s = ""
    for o in output_dicc.values():
        s += "\twire %s %s;\n" % (o[1], o[0])
    return s

# Generates stimulus values for inputs to simulate


def generateMainSequence(input_dicc, forIt):
    exists = False
    s = "\t\tfor(integer i = 0; i < %d; i++) begin\n\t\t\t#2" % forIt
    for varTuple in input_dicc.values():
        if varTuple[0] != 'clk' and varTuple[0] != 'rst':
            exists = True
            # busSize = int(varTuple[1].split(":")[0][1:]) + \
            #     1 if varTuple[1].strip() != "" else 1
            if varTuple[3] == 'random':
                s += f"\n\t\t\t{varTuple[0]} = $urandom();"
            elif varTuple[3] == 'up':
                s += f"\n\t\t\t{varTuple[0]} = i;"
            elif varTuple[3] == "down":
                s += f"\n\t\t\t{varTuple[0]} = {forIt-1}-i;"
    s += "\n\t\tend"
    return s if exists else ""

# Function to initialize input variables within the testbench


def variableInit(input_dicc):
    s = ""
    for varTuple in input_dicc.values():
        if varTuple[0] != 'clk' and varTuple[0] != 'rst':
            s += f"\n\t\t{varTuple[0]} = 0;"
        # if varTuple[0] != 'clk' and varTuple[0] != 'rst':
        #     busSize = int(varTuple[1].split(":")[0][1:]) + \
        #         1 if varTuple[1].strip() != "" else 1
        #     if varTuple[3] == 'random':
        #         s += generateRandom(busSize, varTuple[0])
            # elif varTuple[3] == 'up':
            #     s+=generateAscending()
            # elif varTuple[3] == 'down':
            #     s+=generateDescendign()
    return s

# Function to create the Verilog testbench template with module, parameters (if so), inputs/outputs variables, initialization and stimulus


def getTBString(moduleName, paramsStr, regStr, wireStr, hasClk, hasRst, varInit, mainSequence):

    rstInit = """rst = 1;

    # 3
    rst = 0;"""

    return f"""
`timescale 1ns/1ps

module {moduleName}_tb;
{paramsStr}

    //Creación de regs y wires
{regStr}
{wireStr}
    //Instanciar el top
    {moduleName} UUT(.*);

initial
  begin
    $dumpfile("{moduleName}_tb.vcd");
    $dumpvars (1, {moduleName}_tb);
{varInit}
    {"clk = 0;" if hasClk else ""}
    {rstInit if hasRst else ""}

{mainSequence}
    #4
	$finish;
 	 
   end

 {"always forever #1 clk = ~clk;" if hasClk else ""}
  
  
endmodule
    """
