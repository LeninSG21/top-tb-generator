
`timescale 1s/10ns

module register_file_tb;


    //Creación de regs y wires
	reg  reseteate;
	reg  reloj_cucu;
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



		reseteate = 0;
		reloj_cucu = 0;
		rs_addr = 0;
		rt_addr = 0;
		rd_addr = 0;
		rd_w_data = 0;
    #3
	reseteate = 1;
		for(integer i = 0; i < 10; i++) begin
			#2
			rs_addr = 9-i;
			rt_addr = 9-i;
			rd_addr = 9-i;
			rd_w_data = 9-i;
		end
    #4
	$finish;
 	 
   end

 always forever #1 reloj_cucu = ~reloj_cucu;
  
  
endmodule
    