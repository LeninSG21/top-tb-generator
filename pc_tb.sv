
`timescale 1ns/1ps

module pc_control_tb;

    //Creación de regs y wires
    	reg  clk;
	reg  rst;
	reg [31:0] in_pc;
	reg [4:0] Selector;

    	wire [31:0] out_pc;

    //Instanciar el top
    pc_control UUT(.*);

initial
  begin
    $dumpfile("pc_control_tb.vcd");
    $dumpvars (0, pc_control_tb);

    clk = 0;
    rst = 1;
    # 3
    rst = 0
    
    
    //Qué va a pasar aquí

    #3
	$finish;
 	 
   end

 always forever #1 clk = ~clk;
  
  
endmodule
    