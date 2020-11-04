module pc_control (
   input  clk, rst,
   input [31:0] in_pc,
   output reg [31:0] out_pc
);

output zero;

always @(posedge clk or posedge rst)
  begin
    	if (rst)  
   		 out_pc <= 0;
   	 else
   		 out_pc <= in_pc;
  end	 
endmodule