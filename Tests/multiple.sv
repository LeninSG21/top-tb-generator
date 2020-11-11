/*

gfh

fdsgfh*/

module Multiple_Gates
  (input [1:0] entrada, 
   output NOT_A, OR, AND, NOR, /*NAND,*/ XOR, XNOR);
/**/
  //input A, B, C;

  

  //output reg ASDFG:

/* 
  module Multiple_Gates
  (input [1:0] entrada, 
   output NOT_A, OR, AND, NOR, NAND, XOR, XNOR);
*/

/*

output X;
input YYASDF;

*/

/* input  [32:0] zsadd;   input  [32:0] zsadd ;
input  [32:0] zsadd   output X;     output  [32:0] zsadd ;  


   */
    wire A, B;
    assign A = entrada[1];
    assign B = entrada[0];

  assign NOT_A = ~A;
  assign OR = A | B;
  assign AND = A & B;
  assign NOR = ~(A | B);
  assign NAND = ~(A & B);
  assign XOR = A ^ B;
  assign XNOR = A ~^ B;

  // output [D:0] asdfg,dg,dfg;
  /**/
  
endmodule

//******/
//*/