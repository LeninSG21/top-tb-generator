
`timescale 1ns/1ps

module gray_ctr_tb;

	parameter WIDTH=4;
    //Creación de regs y wires
	reg  clk;
	reg  reset;

	wire [WIDTH-1:0] q;

    //Instanciar el top
    gray_ctr UUT(.*);

initial
  begin
    $dumpfile("gray_ctr_tb.vcd");
    $dumpvars (0, gray_ctr_tb);

		reset = 0;
    clk = 0;
    

		for(integer i = 0; i < 10; i++) begin
			#2
			reset = $urandom();
		end
    #4
	$finish;
 	 
   end

 always forever #1 clk = ~clk;
  
  
endmodule
    