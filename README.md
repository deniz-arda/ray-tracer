# Ray-Sphere Intersection Hardware Accelerator

A hardware accelerator for ray-sphere intersection testing, implemented in SystemVerilog with a C++ software raytracer and Verilator testbench.

## Project Status

**Note:** The hardware acceleration in this project is not complete and is a very simple implementation

## Overview

This project implements ray-sphere intersection calculation in hardware to potentially accelerate raytracing operations. The ray-sphere intersection is one of the fundamental operations in raytracing, and hardware acceleration could significantly speed up rendering.

### Components

- **`ray_sphere_intersect.sv`** - SystemVerilog hardware module
  - Uses Q16.16 fixed-point representation for inputs/outputs
  - Currently uses `real` types internally (simulation only)
  - Simple 2-cycle operation: conversion + computation
  
- **`raytracer.cpp`** - Software raytracer implementation
  - Pure C++ reference implementation
  - Renders scenes with spheres
  
- **`ray_sphere_intersect_tb.cpp`** - Verilator testbench
  - C++ wrapper for the hardware accelerator
  - Comparison tests between hardware and software
  - Test cases: basic intersection, miss, tangent, inside sphere

- **`interesting_scenes.cpp`** - Pre-configured scene library
  - 7 ready-to-use scene configurations with different aesthetics
  - Includes: Mirror Gallery, Neon Dreams, Planetary System, Glass Orbs, Golden Hour, Candy Land, Deep Ocean
  - Copy scene function contents into `raytracer.cpp` to change the rendered scene

- **`makefile`** - Build automation for the software raytracer
  - Optimized build with OpenMP parallelization
  - Convenient targets for building, running, and testing

## Building

### Software Raytracer

Requires:
- C++11 compatible compiler
- SDL2 library
- OpenMP support (optional, for parallelization)

```bash
# Build the raytracer
make

# Build and run
make run

# Build and run with performance timing
make test

# Clean build artifacts
make clean

# Show compiler and system info
make info
```

### Hardware Testbench

Requires:
- Verilator (for hardware simulation)
- C++17 compatible compiler

```bash
# Compile the testbench
verilator --cc ray_sphere_intersect.sv --exe ray_sphere_intersect_tb.cpp --build

# Run tests
./obj_dir/Vray_sphere_intersect
```

## Architecture

The hardware module expects:
- Ray origin (ox, oy, oz) and direction (dx, dy, dz)
- Sphere center (cx, cy, cz) and radius² (r²)
- All values in Q16.16 fixed-point format

Outputs:
- `hit` signal (1 if intersection found)
- `t_out` value (distance along ray to intersection)
- `done` signal when computation completes

## Known Issues

- Hardware module currently not producing correct results
- Needs debugging of fixed-point conversion or computation logic
- Timing/control flow may need adjustment

## Future Work

- Debug and fix hardware implementation
- Replace `real` types with true fixed-point arithmetic
- Pipeline the computation for higher throughput
- Integrate with actual rendering pipeline
- Add support for multiple spheres