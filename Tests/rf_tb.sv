// Project Name: register_file

`timescale 1000ns/1ps

module register_file_tb;

	parameter ADDR = 3'h5;
	parameter BUS_W = 32;
	reg           r_write;
	reg    [ADDR-1:0]  rd_addr;
	reg   [BUS_W - 1:0]  rd_w_data;
	reg           reloj_cucu;
	reg           reset;
	reg    [ADDR-1:0]  rs_addr;
	reg    [ADDR-1:0]  rt_addr;

	wire  [BUS_W - 1:0]  rs_data;
	wire  [BUS_W - 1:0]  rt_data;

	register_file UUT(.*);
	initial begin
		$dumpfile("register_file_tb.vcd");
		$dumpvars (1, register_file_tb);
	
		r_write = 0;
		rd_addr = 0;
		rd_w_data = 0;
		reloj_cucu = 0;
		reset = 0;
		rs_addr = 0;
		rt_addr = 0;
		#3
		reset = 1;
		for(integer i = 0; i < 256; i++) begin 
			#2
			r_write = 255 - i;
			rd_addr = 255 - i;
			rd_w_data = 255 - i;
			rs_addr = 255 - i;
			rt_addr = 255 - i;
		end
		#4
		$finish;
	end
	always forever #0.5 reloj_cucu = ~reloj_cucu;
endmodule
