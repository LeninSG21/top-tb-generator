
`timescale 1ns/1ps

module Multiple_Gates_tb;

    //Creación de regs y wires
	reg [1:0] entrada;

	wire  NOT_A;
	wire  OR;
	wire  AND;
	wire  NOR;
	wire  XOR;
	wire  XNOR;

    //Instanciar el top
    Multiple_Gates UUT(.*);

initial
  begin
    $dumpfile("Multiple_Gates_tb.vcd");
    $dumpvars (1, Multiple_Gates_tb);

		entrada = 0;
    
		for(integer i = 0; i < 4; i++) begin
			#2
			entrada = $urandom(32705);
		end
    #4
	$finish;
   end
 
endmodule
    