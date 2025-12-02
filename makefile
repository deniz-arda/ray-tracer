# Simple Makefile for Ray Tracer (Alternative to CMake)
# Usage: make          - builds the ray tracer
#        make run      - builds and runs
#        make clean    - removes build artifacts
#        make test     - builds and runs with timing

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3 -march=native -fopenmp
LDFLAGS = -lSDL2 -lm -fopenmp

# Source files
SRC = raytracer.cpp
TARGET = raytracer

# Build target
$(TARGET): $(SRC)
	@echo "=========================================="
	@echo "Building Optimized Ray Tracer"
	@echo "=========================================="
	@echo "Compiler: $(CXX)"
	@echo "Flags:    $(CXXFLAGS)"
	@echo "Libraries: $(LDFLAGS)"
	@echo ""
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
	@echo ""
	@echo "✓ Build successful!"
	@echo "Run with: ./$(TARGET)"
	@echo ""

# Build and run
run: $(TARGET)
	@echo "=========================================="
	@echo "Running Ray Tracer"
	@echo "=========================================="
	@./$(TARGET)

# Build and run with timing
test: $(TARGET)
	@echo "=========================================="
	@echo "Performance Test"
	@echo "=========================================="
	@echo "Threads: $${OMP_NUM_THREADS:-$(shell nproc)}"
	@time ./$(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "✓ Clean complete"

# Show compiler info
info:
	@echo "=========================================="
	@echo "Compiler Information"
	@echo "=========================================="
	@$(CXX) --version
	@echo ""
	@echo "SDL2 installation:"
	@pkg-config --modversion sdl2 2>/dev/null || echo "  Not found via pkg-config"
	@echo ""
	@echo "OpenMP support:"
	@echo "#include <omp.h>" | $(CXX) -fopenmp -x c++ - -E > /dev/null 2>&1 && echo "  ✓ Available" || echo "  ✗ Not available"
	@echo ""
	@echo "CPU cores: $(shell nproc)"

# Help
help:
	@echo "=========================================="
	@echo "Ray Tracer Makefile"
	@echo "=========================================="
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the ray tracer"
	@echo "  make run      - Build and run"
	@echo "  make test     - Build and run with timing"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make info     - Show compiler and system info"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                        # Just build"
	@echo "  make run                    # Build and run"
	@echo "  OMP_NUM_THREADS=4 make run  # Run with 4 threads"
	@echo "  make clean && make          # Clean build"
	@echo ""

.PHONY: run test clean info help