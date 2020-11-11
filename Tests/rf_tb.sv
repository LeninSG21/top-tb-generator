// Project Name: register_file

`timescale 1ns/1ps

module register_file_tb;

	parameter ADDR = 5;
	parameter BUS_W = 32;
	reg           clk;
	reg           r_write;
	reg    [ADDR-1:0]  rd_addr;
	reg   [BUS_W - 1:0]  rd_w_data;
	reg    [ADDR-1:0]  rs_addr;
	reg           rst;
	reg    [ADDR-1:0]  rt_addr;

	wire  [BUS_W - 1:0]  rs_data;
	wire  [BUS_W - 1:0]  rt_data;

	register_file UUT(.*);
	initial begin
		$dumpfile("register_file_tb.vcd");
		$dumpvars (1, register_file_tb);
	
		clk = 0;
		r_write = 0;
		rd_addr = 0;
		rd_w_data = 0;
		rs_addr = 0;
		rst = 1;
		rt_addr = 0;
		#3
		rst = 0;
		for(integer i = 0; i < 10; i++) begin 
			#2
			r_write = i;
			rd_addr = i;
			rd_w_data = i;
			rs_addr = i;
			rt_addr = i;
		end
		#4
		$finish;
	end
	always forever #0.5 clk = ~clk;
endmodule
