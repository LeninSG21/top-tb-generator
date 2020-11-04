# var_struct --> (name, size, type, funcType)


def generateInputTb(input_dicc, inout_dicc):
    s = ""
    for i in input_dicc.values():
        s += "\treg %s %s;\n" % (i[1], i[0])
    for i in inout_dicc.values():
        s += "\treg %s %s;\n" % (i[1], i[0])
    return s


def generateOutputTb(output_dicc):
    s = ""
    for o in output_dicc.values():
        s += "\twire %s %s;\n" % (o[1], o[0])
    return s


def generateTbStr(input_dicc):
    s = ""

    for varTuple in input_dicc.values():
        # [4:0]
        busSize = int(varTuple[1].split(":")[0][1:]) + \
            1 if varTuple[1].strip() != "" else 1
        print(busSize)
        '''
        if varTuple[3] == 'random':
            generateRandom()
            random.randint()
           
        elif varTuple[3] == 'up':
            generateAscending()
        elif varTuple[3] == 'down': 
            generateDescendign()
            '''

# This function creates the Verilog testbench template with module and inputs/outputs names.


def getTBString(moduleName, regStr, wireStr, hasClk, hasRst):

    rstInit = """rst = 1;
    # 3
    rst = 0
    """

    return f"""
`timescale 1ns/1ps

module {moduleName}_tb;

    //Creación de regs y wires
    {regStr}
    {wireStr}
    //Instanciar el top
    {moduleName} UUT(.*);

initial
  begin
    $dumpfile("{moduleName}_tb.vcd");
    $dumpvars (0, {moduleName}_tb);

    {"clk = 0;" if hasClk else ""}
    {rstInit if hasRst else ""}
    
    //Qué va a pasar aquí

    #3
	$finish;
 	 
   end

 {"always forever #1 clk = ~clk;" if hasClk else ""}
  
  
endmodule
    """
