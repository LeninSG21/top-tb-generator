`timescale 1ns/1ps

module nombre_module_tb;

    //Creación de regs y wires

    //Instanciar el top
    nombre_modulo UUT(.*);

initial
  begin
    $dumpfile("nombre_module_tb.vcd");
    $dumpvars (0, nombre_module_tb);

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