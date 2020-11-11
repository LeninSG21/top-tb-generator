// Create Date:  11/11/2020, 11:10:26
// Project Name: register_file

`timescale 100ns/1ps

module register_file_tb;

	parameter ADDR = 5;
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
    #3
	reset = 1;
	for(integer i = 0; i < 32; i++) begin
		#2
		rs_addr = $urandom(14911);
		rt_addr = $urandom(30397);
		rd_addr = $urandom(81269);
		rd_w_data = 31-i;
	end
    #4
	$finish;
   end
 always forever #0.5 reloj = ~reloj;
endmodule
    