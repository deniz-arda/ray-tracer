//==============================================================================
// Verilator C++ Testbench for Ray-Sphere Intersection Accelerator
//==============================================================================
// This wrapper provides a simple C++ interface to the hardware accelerator
//==============================================================================

#include "Vray_sphere_intersect.h"
#include "verilated.h"
#include <iostream>
#include <cmath>
#include <cstdint>

// Q16.16 fixed-point conversion utilities
const int FRAC_BITS = 16;
const double SCALE = (1 << FRAC_BITS);

// Convert double to Q16.16 fixed-point
int32_t double_to_fixed(double val) {
    return static_cast<int32_t>(val * SCALE);
}

// Convert Q16.16 fixed-point to double
double fixed_to_double(int32_t val) {
    return static_cast<double>(val) / SCALE;
}

// Hardware accelerator class
class RaySphereAccelerator {
private:
    Vray_sphere_intersect* dut;
    uint64_t time_counter;
    
    void tick() {
        dut->clk = 0;
        dut->eval();
        time_counter++;
        
        dut->clk = 1;
        dut->eval();
        time_counter++;
    }
    
    void reset() {
        dut->rst_n = 0;
        tick();
        tick();
        dut->rst_n = 1;
    }

public:
    RaySphereAccelerator() : time_counter(0) {
        dut = new Vray_sphere_intersect;
        reset();
    }
    
    ~RaySphereAccelerator() {
        dut->final();
        delete dut;
    }
    
    // Main intersection test function
    // Returns true if hit, and sets t to the intersection distance
    bool intersect(double ray_ox, double ray_oy, double ray_oz,
                   double ray_dx, double ray_dy, double ray_dz,
                   double sphere_cx, double sphere_cy, double sphere_cz,
                   double sphere_radius,
                   double& t) {
        
        // Convert inputs to fixed-point
        dut->ray_ox = double_to_fixed(ray_ox);
        dut->ray_oy = double_to_fixed(ray_oy);
        dut->ray_oz = double_to_fixed(ray_oz);
        
        dut->ray_dx = double_to_fixed(ray_dx);
        dut->ray_dy = double_to_fixed(ray_dy);
        dut->ray_dz = double_to_fixed(ray_dz);
        
        dut->sphere_cx = double_to_fixed(sphere_cx);
        dut->sphere_cy = double_to_fixed(sphere_cy);
        dut->sphere_cz = double_to_fixed(sphere_cz);
        dut->sphere_r2 = double_to_fixed(sphere_radius * sphere_radius);
        
        // Assert start signal
        dut->start = 1;
        tick();
        dut->start = 0;
        
        // Wait for done signal (with timeout)
        int timeout = 100;
        while (!dut->done && timeout > 0) {
            tick();
            timeout--;
        }
        
        if (timeout == 0) {
            std::cerr << "Hardware timeout!" << std::endl;
            return false;
        }
        
        // Read results
        bool hit = dut->hit;
        if (hit) {
            t = fixed_to_double(dut->t_out);
        }
        
        return hit;
    }
};

//==============================================================================
// Test functions
//==============================================================================

void test_basic_intersection() {
    std::cout << "\n=== Test 1: Basic Intersection ===" << std::endl;
    RaySphereAccelerator accel;
    
    // Ray from (0,0,5) towards origin
    double t;
    bool hit = accel.intersect(
        0.0, 0.0, 5.0,    // ray origin
        0.0, 0.0, -1.0,   // ray direction
        0.0, 0.0, 0.0,    // sphere center
        1.0,              // sphere radius
        t
    );
    
    std::cout << "Hit: " << (hit ? "YES" : "NO") << std::endl;
    if (hit) {
        std::cout << "t = " << t << " (expected ~4.0)" << std::endl;
    }
}

void test_miss() {
    std::cout << "\n=== Test 2: Ray Miss ===" << std::endl;
    RaySphereAccelerator accel;
    
    // Ray that misses sphere
    double t;
    bool hit = accel.intersect(
        5.0, 0.0, 0.0,    // ray origin (far to the right)
        0.0, 0.0, -1.0,   // ray direction (pointing away)
        0.0, 0.0, 0.0,    // sphere center
        1.0,              // sphere radius
        t
    );
    
    std::cout << "Hit: " << (hit ? "YES" : "NO") << " (expected NO)" << std::endl;
}

void test_tangent() {
    std::cout << "\n=== Test 3: Tangent Ray ===" << std::endl;
    RaySphereAccelerator accel;
    
    // Ray that grazes the sphere
    double t;
    bool hit = accel.intersect(
        0.0, 1.0, 5.0,    // ray origin (at sphere's edge height)
        0.0, 0.0, -1.0,   // ray direction
        0.0, 0.0, 0.0,    // sphere center
        1.0,              // sphere radius
        t
    );
    
    std::cout << "Hit: " << (hit ? "YES" : "NO") << std::endl;
    if (hit) {
        std::cout << "t = " << t << " (expected ~5.0)" << std::endl;
    }
}

void test_inside_sphere() {
    std::cout << "\n=== Test 4: Ray Origin Inside Sphere ===" << std::endl;
    RaySphereAccelerator accel;
    
    // Ray starting inside sphere
    double t;
    bool hit = accel.intersect(
        0.0, 0.0, 0.0,    // ray origin (at sphere center)
        1.0, 0.0, 0.0,    // ray direction
        0.0, 0.0, 0.0,    // sphere center
        2.0,              // sphere radius
        t
    );
    
    std::cout << "Hit: " << (hit ? "YES" : "NO") << std::endl;
    if (hit) {
        std::cout << "t = " << t << " (expected ~2.0)" << std::endl;
    }
}

void compare_with_software() {
    std::cout << "\n=== Test 5: Accuracy Comparison ===" << std::endl;
    RaySphereAccelerator accel;
    
    // Software implementation (from the raytracer)
    auto software_intersect = [](double ox, double oy, double oz,
                                  double dx, double dy, double dz,
                                  double cx, double cy, double cz,
                                  double radius) -> std::pair<bool, double> {
        double oc_x = ox - cx;
        double oc_y = oy - cy;
        double oc_z = oz - cz;
        
        double a = dx*dx + dy*dy + dz*dz;
        double b = 2.0 * (oc_x*dx + oc_y*dy + oc_z*dz);
        double c = oc_x*oc_x + oc_y*oc_y + oc_z*oc_z - radius*radius;
        
        double discriminant = b*b - 4*a*c;
        if (discriminant < 0) return {false, 0.0};
        
        double t1 = (-b - sqrt(discriminant)) / (2.0*a);
        if (t1 > 0.001) return {true, t1};
        
        double t2 = (-b + sqrt(discriminant)) / (2.0*a);
        if (t2 > 0.001) return {true, t2};
        
        return {false, 0.0};
    };
    
    // Test several cases
    struct TestCase {
        double ray_ox, ray_oy, ray_oz;
        double ray_dx, ray_dy, ray_dz;
        double sphere_cx, sphere_cy, sphere_cz;
        double radius;
    } test_cases[] = {
        {0, 0, 5,  0, 0, -1,  0, 0, 0,  1.0},
        {2, 0, 3,  0, 0, -1,  0, 0, 0,  1.0},
        {0, 2, 5,  0, -1, 0,  0, 0, 0,  1.5},
        {-3, -3, 5,  1, 1, -1,  0, 0, 0,  2.0},
    };
    
    int passed = 0;
    int total = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < total; i++) {
        auto& tc = test_cases[i];
        
        // Hardware
        double t_hw;
        bool hit_hw = accel.intersect(
            tc.ray_ox, tc.ray_oy, tc.ray_oz,
            tc.ray_dx, tc.ray_dy, tc.ray_dz,
            tc.sphere_cx, tc.sphere_cy, tc.sphere_cz,
            tc.radius, t_hw
        );
        
        // Software
        auto [hit_sw, t_sw] = software_intersect(
            tc.ray_ox, tc.ray_oy, tc.ray_oz,
            tc.ray_dx, tc.ray_dy, tc.ray_dz,
            tc.sphere_cx, tc.sphere_cy, tc.sphere_cz,
            tc.radius
        );
        
        bool match = (hit_hw == hit_sw);
        if (hit_hw && hit_sw) {
            double error = fabs(t_hw - t_sw);
            match = match && (error < 0.01); // 1% tolerance for fixed-point
            
            std::cout << "Case " << i+1 << ": HW t=" << t_hw 
                      << ", SW t=" << t_sw 
                      << ", error=" << error << " ";
        } else {
            std::cout << "Case " << i+1 << ": HW hit=" << hit_hw 
                      << ", SW hit=" << hit_sw << " ";
        }
        
        if (match) {
            std::cout << "[PASS]" << std::endl;
            passed++;
        } else {
            std::cout << "[FAIL]" << std::endl;
        }
    }
    
    std::cout << "\nPassed: " << passed << "/" << total << std::endl;
}

//==============================================================================
// Main
//==============================================================================
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    
    std::cout << "======================================" << std::endl;
    std::cout << "Ray-Sphere Intersection Accelerator" << std::endl;
    std::cout << "Hardware Testbench (Verilator)" << std::endl;
    std::cout << "======================================" << std::endl;
    
    test_basic_intersection();
    test_miss();
    test_tangent();
    test_inside_sphere();
    compare_with_software();
    
    std::cout << "\n=== All tests complete ===" << std::endl;
    
    return 0;
}