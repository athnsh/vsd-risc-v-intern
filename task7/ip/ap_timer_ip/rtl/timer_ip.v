module timer_ip (
    input             clk,
    input             resetn,    
    input             sel,         
    input             write_en,
    input      [1:0]  offset,      // 00=CTRL 01=LOAD 10=VALUE 11=STATUS
    input      [31:0] write_data,
    output reg [31:0] read_data,
    output 	      timeout
);

    reg [31:0] ctrl_reg;     // CTRL
    reg [31:0] load_reg;     // LOAD
    reg [31:0] value_reg;    // VALUE
    reg        timeout_flag; // STATUS bit 0

    wire       en        = ctrl_reg[0];
    wire       mode      = ctrl_reg[1];      // 0=one-shot, 1=periodic
    wire       presc_en  = ctrl_reg[2];
    wire [7:0] presc_div = ctrl_reg[15:8];

    reg  en_d;
    wire en_start = en & ~en_d;

    always @(posedge clk) begin
        if (!resetn)
            en_d <= 1'b0;
        else
            en_d <= en;
    end

    reg [7:0] presc_cnt;
    wire      tick = (!presc_en) || (presc_cnt == presc_div);

    always @(posedge clk) begin
        if (!resetn)
            presc_cnt <= 8'd0;
        else if (!en)
            presc_cnt <= 8'd0;
        else if (!presc_en)
            presc_cnt <= 8'd0;
        else if (presc_cnt == presc_div)
            presc_cnt <= 8'd0;
        else
            presc_cnt <= presc_cnt + 8'd1;
    end

    always @(posedge clk) begin
        if (!resetn) begin
            ctrl_reg <= 32'd0;
            load_reg <= 32'd0;
        end
        else if (sel && write_en) begin
            case (offset)
                2'b00: ctrl_reg <= write_data;          // CTRL
                2'b01: load_reg <= write_data;          // LOAD
                default: ;
            endcase
        end
    end

    always @(posedge clk) begin
        if (!resetn) begin
            value_reg    <= 32'd0;
            timeout_flag <= 1'b0;
        end
        else begin
            if (sel && write_en && (offset == 2'b11) && write_data[0])
                timeout_flag <= 1'b0;

                if (en_start) begin
                    value_reg <= load_reg;
                end
            else if (en && tick) begin
    		    if (value_reg > 32'd1) begin
       		        value_reg <= value_reg - 32'd1;
    		    end
    		else if (value_reg == 32'd1) begin
        		value_reg    <= 32'd0;
        		timeout_flag <= 1'b1;
        		if (mode)
           			value_reg <= load_reg;
    		end
    		else begin
        		if (mode)
            		value_reg <= load_reg;
    		end
	    end
        end
    end

    wire [31:0] ctrl_read   = {16'b0, presc_div, 5'b0, presc_en, mode, en};
    wire [31:0] status_read = {31'b0, timeout_flag};

    always @(*) begin
        if (!sel)
            read_data = 32'd0;
        else begin
            case (offset)
                2'b00: read_data = ctrl_read;
                2'b01: read_data = load_reg;
                2'b10: read_data = value_reg;
                2'b11: read_data = status_read;
                default: read_data = 32'd0;
            endcase
        end
    end
    assign timeout = timeout_flag;
endmodule