// Create Date:  11/13/2020, 16:10:01
// Project Name: register_file

`timescale 1ns/1ps

module register_file_tb;

	parameter ADDR = 3'h5;
	parameter BUS_W = 32;
    //Creación de regs y wires
	reg  reset;
	reg  reloj;
	reg [ADDR-1:0] rs_addr;
	reg [ADDR-1:0] rt_addr;
	reg [ADDR-1:0] rd_addr;
	reg [BUS_W - 1:0] rd_w_data;

	wire [BUS_W - 1:0] rs_data;
	wire [BUS_W - 1:0] rt_data;

    //Instanciar el top
    register_file UUT(.*);

initial
  begin
    $dumpfile("register_file_tb.vcd");
    $dumpvars (1, register_file_tb);

	reset = 0;
	reloj = 0;
	rs_addr = 0;
	rt_addr = 0;
	rd_addr = 0;
	rd_w_data = 0;
    
	for(integer i = 0; i < 10; i++) begin
		#2
		reset = $urandom(63097);
		reloj = $urandom(64344);
		rs_addr = $urandom(80374);
		rt_addr = $urandom(82866);
		rd_addr = $urandom(65757);
		rd_w_data = $urandom(50567);
	end
    #4
	$finish;
   end
 
endmodule
    