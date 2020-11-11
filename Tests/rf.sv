module register_file #(
  parameter ADDR = 5,
  parameter BUS_W = 32
)(
        input         rst,
        input         clk,
        input         r_write,
        input   [ADDR-1:0] rs_addr,
        input   [ADDR-1:0] rt_addr,
        input   [ADDR-1:0] rd_addr,
        input  [BUS_W - 1:0] rd_w_data,
        
        output [BUS_W - 1:0] rs_data,
        output [BUS_W - 1:0] rt_data
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