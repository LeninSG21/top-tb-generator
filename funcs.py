def getTBString(moduleName):

    return f"""
`timescale 1ns/1ps

module {moduleName}_tb;

    //Creación de regs y wires

    //Instanciar el top
    {moduleName} UUT(.*);

initial
  begin
    $dumpfile("{moduleName}_tb.vcd");
    $dumpvars (0, {moduleName}_tb);

	clk = 0;
    rst = 1;
    
	#3
	rst = 0;

    //Qué va a pasar aquí


    #3
	$finish;
 	 
   end
  
  always forever #1 clk = ~clk;
  
endmodule
    """
