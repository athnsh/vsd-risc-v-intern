module gpio_ip(
    input clk,
    input resetn,
    input write_en,
    input [31:0] write_data,
    output [31:0] read_data,
    output [31:0] gpio_out
);

reg [31:0] gpio_reg;

always @(posedge clk) begin
    if(!resetn)
        gpio_reg <= 32'b0;
    else if(write_en)
        gpio_reg <= write_data;
end

assign read_data = gpio_reg;
assign gpio_out = gpio_reg;

endmodule