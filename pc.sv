module pc_control (
   input  clk,
   input  rst,
   input [31:0] in_pc,
   output [31:0] out_pc
);

always @(posedge clk or posedge rst)
  begin
    	if (rst)  
   		 out_pc <= 0;
   	 else
   		 out_pc <= in_pc;
  end	 
endmodule