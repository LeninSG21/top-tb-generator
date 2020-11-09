module register_file (
        input logic         rst,
        input logic         clk,
        input logic   [4:0] rs_addr,
        input logic   [4:0] rt_addr,
        input logic   [4:0] rd_addr,
        input logic  [31:0] rd_w_data,
        input logic         reg_write,
        output logic [31:0] rs_data,
        output logic [31:0] rt_data
    );
    
  logic [31:0] reg_mem [0:31];
  
    integer i;

    always_ff @(posedge clk or posedge rst)
        begin
            if(rst)
                for(i = 0; i <= 31; i = i+1)
                    reg_mem[i] <= 0;
            else if(reg_write & rd_addr != 0)
                reg_mem[rd_addr] <= rd_w_data;
          	
        end
  
  /*
    always_ff @(posedge clk)
      begin
        rs_data <= reg_mem[rs_addr];
        rt_data <= reg_mem[rt_addr];
      end
  */
  assign rs_data = reg_mem[rs_addr];
  assign rt_data = reg_mem[rt_addr];
        
endmodule