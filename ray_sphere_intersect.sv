// Ultra-simple ray-sphere intersection
module ray_sphere_intersect (
    input  logic clk,
    input  logic rst_n,
    input  logic start,
    output logic done,
    
    input  logic signed [31:0] ray_ox, ray_oy, ray_oz,
    input  logic signed [31:0] ray_dx, ray_dy, ray_dz,
    input  logic signed [31:0] sphere_cx, sphere_cy, sphere_cz,
    input  logic signed [31:0] sphere_r2,
    
    output logic hit,
    output logic signed [31:0] t_out
);

    logic [3:0] cycle;
    
    // Use doubles for now - just get it working
    real rox, roy, roz, rdx, rdy, rdz;
    real scx, scy, scz, sr2;
    real ocx, ocy, ocz;
    real a, b, c, disc, t;
    
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            done <= 0;
            hit <= 0;
            cycle <= 0;
        end else if (start) begin
            cycle <= 1;
            done <= 0;
            
            // Convert to real
            rox = $itor(ray_ox) / 65536.0;
            roy = $itor(ray_oy) / 65536.0;
            roz = $itor(ray_oz) / 65536.0;
            rdx = $itor(ray_dx) / 65536.0;
            rdy = $itor(ray_dy) / 65536.0;
            rdz = $itor(ray_dz) / 65536.0;
            scx = $itor(sphere_cx) / 65536.0;
            scy = $itor(sphere_cy) / 65536.0;
            scz = $itor(sphere_cz) / 65536.0;
            sr2 = $itor(sphere_r2) / 65536.0;
            
        end else if (cycle == 1) begin
            // Compute
            ocx = rox - scx;
            ocy = roy - scy;
            ocz = roz - scz;
            
            a = rdx*rdx + rdy*rdy + rdz*rdz;
            b = 2.0 * (ocx*rdx + ocy*rdy + ocz*rdz);
            c = ocx*ocx + ocy*ocy + ocz*ocz - sr2;
            
            disc = b*b - 4.0*a*c;
            
            if (disc >= 0.0 && a != 0.0) begin
                t = (-b - $sqrt(disc)) / (2.0 * a);
                if (t > 0.001) begin
                    hit <= 1;
                    t_out <= $rtoi(t * 65536.0);
                end else begin
                    hit <= 0;
                end
            end else begin
                hit <= 0;
            end
            
            done <= 1;
            cycle <= 0;
        end
    end

endmodule
