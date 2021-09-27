`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// UP1024 direct pins access
//////////////////////////////////////////////////////////////////////////////////

//uncomment this to build for FPGA1
//`define FPGA1 1

module main(D, nRD, nWR, RDY, nD_A, IFCLK, CRCLK, PINS);
    inout [7:0] D;
    input nRD;
    input nWR;
    inout RDY;
    input nD_A;
    input IFCLK;
    input CRCLK;
//	 input nCS;
//	 input nWRITE;
    inout [127:0] PINS;

wire nrd_int;
wire nwr_int;
wire ifclk_int;
wire crclk_int;

///////////////////////////////// RDY //////////////////////////////////////////////////////////////	
assign RDY = 1'bz; //always ready
/////////////////////////////////// address //////////////////////////////////////////////////
reg [3:0] addr;
reg oea;
reg sel;
reg awr;

always @(negedge nwr_int) awr <= nD_A;

always @(posedge nwr_int)
	if (awr)
	begin
		oea <= D[7];
		addr[3:0] <= D[3:0];
		sel <= D[4];
	end
/////////////////////////////////// out reg /////////////////////////////////////////////////
reg [7:0] outreg [15:0];

`ifdef FPGA1
always @(posedge nwr_int) if (!awr & sel & !oea) outreg[addr[3:0]] <= D; //fpga1
`else
always @(posedge nwr_int) if (!awr & !sel & !oea) outreg[addr[3:0]] <= D; //fpga0
`endif
/////////////////////////////////// oe reg /////////////////////////////////////////////////
reg [7:0] oereg [15:0];

`ifdef FPGA1
always @(posedge nwr_int) if (!awr & sel & oea) oereg[addr[3:0]] <= D; //fpga1
`else
always @(posedge nwr_int) if (!awr & !sel & oea) oereg[addr[3:0]] <= D; //fpga0
`endif
/////////////////////////////////// out tri ////////////////////////////////////////////////
genvar gi;
generate
		for(gi=0; gi<16; gi=gi+1)
		begin: OUTTRI
			assign PINS[(gi*8)]   = oereg[gi][0] ? outreg[gi][0] : 1'bz;
			assign PINS[(gi*8)+1] = oereg[gi][1] ? outreg[gi][1] : 1'bz;
			assign PINS[(gi*8)+2] = oereg[gi][2] ? outreg[gi][2] : 1'bz;
			assign PINS[(gi*8)+3] = oereg[gi][3] ? outreg[gi][3] : 1'bz;
			assign PINS[(gi*8)+4] = oereg[gi][4] ? outreg[gi][4] : 1'bz;
			assign PINS[(gi*8)+5] = oereg[gi][5] ? outreg[gi][5] : 1'bz;
			assign PINS[(gi*8)+6] = oereg[gi][6] ? outreg[gi][6] : 1'bz;
			assign PINS[(gi*8)+7] = oereg[gi][7] ? outreg[gi][7] : 1'bz;
		end
endgenerate
////////////////////////////////////// in addressing //////////////////////////////////////////////////////
reg [7:0] rdpins;

always @(addr[3:0] or PINS)
begin
	 case (addr[3:0])
      4'b0000: begin
                  rdpins <= PINS[7:0];
               end
      4'b0001: begin
                  rdpins <= PINS[15:8];
               end
      4'b0010: begin
                  rdpins <= PINS[23:16];
               end
      4'b0011: begin
                  rdpins <= PINS[31:24];
               end
      4'b0100: begin
                  rdpins <= PINS[39:32];
               end
      4'b0101: begin
                  rdpins <= PINS[47:40];
               end
      4'b0110: begin
                  rdpins <= PINS[55:48];
               end
      4'b0111: begin
                  rdpins <= PINS[63:56];
               end
      4'b1000: begin
                  rdpins <= PINS[71:64];
               end
      4'b1001: begin
                  rdpins <= PINS[79:72];
               end
      4'b1010: begin
                  rdpins <= PINS[87:80];
               end
      4'b1011: begin
                  rdpins <= PINS[95:88];
               end
      4'b1100: begin
                  rdpins <= PINS[103:96];
               end
      4'b1101: begin
                  rdpins <= PINS[111:104];
               end
      4'b1110: begin
                  rdpins <= PINS[119:112];
               end
      4'b1111: begin
                  rdpins <= PINS[127:120];
               end
	endcase
end
////////////////////////////////// in tri ////////////////////////////////////////	
`ifdef FPGA1
assign D = (nrd_int | !sel) ? 8'hzz : rdpins; //fpga1
`else
assign D = (nrd_int | sel) ? 8'hzz : rdpins; //fpga0
`endif
////////////////////////////////// global signals ////////////////////////////////
IBUFG #(
     .IOSTANDARD("DEFAULT")
   ) IBUFG_nrd (
      .O(nrd_int), // Clock buffer output
      .I(nRD)  // Clock buffer input (connect directly to top-level port)
   );
IBUFG #(
     .IOSTANDARD("DEFAULT")
   ) IBUFG_nwr (
      .O(nwr_int), // Clock buffer output
      .I(nWR)  // Clock buffer input (connect directly to top-level port)
   );
IBUFG #(
     .IOSTANDARD("DEFAULT")
   ) IBUFG_ifclk (
      .O(ifclk_int), // Clock buffer output
      .I(IFCLK)  // Clock buffer input (connect directly to top-level port)
   );
IBUFG #(
     .IOSTANDARD("DEFAULT")
   ) IBUFG_crclk (
      .O(crclk_int), // Clock buffer output
      .I(CRCLK)  // Clock buffer input (connect directly to top-level port)
   );
//////////////////////////////////////////////////////////////////////////////////

endmodule
