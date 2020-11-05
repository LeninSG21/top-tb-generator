module gray_ctr
  #(parameter WIDTH=4)
  (input logic clk,                
	input logic reset,
    output logic [WIDTH-1:0] q);    
   
  logic [WIDTH-1:0] counter;
  
  always_ff @ (posedge clk) begin
    if (reset) begin
      counter <= 0;
    end  
    else begin
      counter <= counter + 1;    
    end  
  end
   
  always_comb begin
  for (int i=1; i < WIDTH; i = i + 1) begin
    q[i-1] = counter[i]^counter[i-1]; 
  end
  q[WIDTH-1] = counter[WIDTH-1];
  end  
  
  
endmodule