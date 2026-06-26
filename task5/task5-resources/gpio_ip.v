module gpio_ip(
    input clk,
    input resetn,
    input sel,
    input write_en,
    input [1:0] offset,
    input [31:0] write_data,
    input [31:0] gpio_in,
    output reg [31:0] read_data,
    output [31:0] gpio_out
);

reg [31:0] gpio_data_reg;
reg [31:0] gpio_dir_reg;

always @(posedge clk) begin
    if(!resetn) begin
        gpio_data_reg <= 32'b0;
        gpio_dir_reg  <= 32'b0;
    end
    else if(sel && write_en) begin
        case(offset)
            2'b00: gpio_data_reg <= write_data;
            2'b01: gpio_dir_reg <= write_data;
        endcase
    end
end

always @(*) begin
    if(!sel)
        read_data = 32'b0;
    else begin
        case(offset)
            2'b00: read_data = gpio_data_reg;
            2'b01: read_data = gpio_dir_reg;
            2'b10: read_data = (gpio_data_reg & gpio_dir_reg) | (gpio_in & ~gpio_dir_reg);
            default: read_data = 32'b0;
        endcase
    end
end

assign gpio_out = gpio_data_reg & gpio_dir_reg;
endmodule