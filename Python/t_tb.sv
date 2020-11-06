
`timescale 1ns/1ps

module ALU_tb;

	parameter W = 9;
	parameter H = 2'h12;

    //Creación de regs y wires
	reg  x;
	reg [3:0] zz;

	wire  y;
	wire [W-1:0] yy;

    //Instanciar el top
    ALU UUT(.*);

initial
  begin
    $dumpfile("ALU_tb.vcd");
    $dumpvars (1, ALU_tb);

		x = 0;
		zz = 0;
    
    

		for(integer i = 0; i < 8; i++) begin
			#2
			x = 7-i;
			zz = 7-i;
		end
    #4
	$finish;
 	 
   end

 
  
  
endmodule
    