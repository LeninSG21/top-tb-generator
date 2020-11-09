
`timescale 1ns/1ps

module register_file_tb;


    //Creación de regs y wires
	reg  rst;
	reg  clk;
	reg [4:0] rs_addr;
	reg [4:0] rt_addr;
	reg [4:0] rd_addr;
	reg [31:0] rd_w_data;

	wire [31:0] rs_data;
	wire [31:0] rt_data;

    //Instanciar el top
    register_file UUT(.*);

initial
  begin
    $dumpfile("register_file_tb.vcd");
    $dumpvars (1, register_file_tb);

		rs_addr = 0;
		rt_addr = 0;
		rd_addr = 0;
		rd_w_data = 0;
    clk = 0;
    rst = 1;

    # 3
    rst = 0;

		for(integer i = 0; i < 10; i++) begin
			#2
			rs_addr = $urandom();
			rt_addr = $urandom();
			rd_addr = $urandom();
			rd_w_data = $urandom();
		end
    #4
	$finish;
 	 
   end

 always forever #1 clk = ~clk;
  
  
endmodule
    