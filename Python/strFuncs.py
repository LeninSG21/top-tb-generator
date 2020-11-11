from random import randint
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
def generateMainSequence(input_dicc, forIt, clk, rst):
    exists = False
    s = "\tfor(integer i = 0; i < %d; i++) begin\n\t\t#2" % forIt
    for varTuple in input_dicc.values():
        if varTuple[0] != clk and varTuple[0] != rst[0]:
            exists = True
            if varTuple[3] == 'random':
                s += f"\n\t\t{varTuple[0]} = $urandom({randint(1,100000)});"
            elif varTuple[3] == 'up':
                s += f"\n\t\t{varTuple[0]} = i;"
            elif varTuple[3] == "down":
                s += f"\n\t\t{varTuple[0]} = {forIt-1}-i;"
    s += "\n\tend"
    return s if exists else ""


# Function to initialize input variables within the testbench
def variableInit(input_dicc, clk, rst):
    s = ""
    for varTuple in input_dicc.values():
        s += f"\n\t{varTuple[0]}"
        if(varTuple[0] == rst[0]):
            s += " = 1;" if rst[1] else " = 0;"
        else:
            s += " = 0;"
    return s


# Function to create the Verilog testbench template with module, parameters (if so), inputs/outputs variables, initialization and stimulus
def getTBString(date_time, moduleName, paramsStr, regStr, wireStr, hasClk, hasRst, varInit, mainSequence, scale, clk, rst):

    off = "0" if rst[1] else "1"
    rstOff = f"#3\n\t{rst[0]} = {off};"

    return f"""// Create Date:  {date_time}
// Project Name: {moduleName}

`timescale {scale}

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
    {rstOff if hasRst else ""}
{mainSequence}
    #4
	$finish;
   end
 {f"always forever #0.5 {clk} = ~{clk};" if hasClk else ""}
endmodule
    """
