# Makefile for Vector Triad Project (Windows)

# Compiler settings
CC = gcc
CFLAGS_DEBUG = -Wall -Wextra -g -O0 -DDEBUG
CFLAGS_RELEASE = -Wall -Wextra -O3 -DNDEBUG -march=native

# Assembler settings (for when you add assembly files)
ASM = nasm
ASMFLAGS = -f win64

# Linker settings
LDFLAGS = -lm  # Math library for sin, cos, tan functions

# Source files
C_SOURCES = main.c vector_kernels.c
# ASM_SOURCES = asm_x86_64.asm asm_simd_xmm.asm asm_simd_ymm.asm  # Add when ready

# Object files
C_OBJECTS_DEBUG = $(C_SOURCES:.c=_debug.o)
C_OBJECTS_RELEASE = $(C_SOURCES:.c=_release.o)
# ASM_OBJECTS = $(ASM_SOURCES:.asm=.o)  # Add when ready

# Executables
TARGET_DEBUG = vector_triad_debug.exe
TARGET_RELEASE = vector_triad_release.exe

# Default target
all: debug release

# Debug build
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(C_OBJECTS_DEBUG)
	$(CC) $(C_OBJECTS_DEBUG) -o $@ $(LDFLAGS)
	@echo "Debug build completed: $(TARGET_DEBUG)"

%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

# Release build
release: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(C_OBJECTS_RELEASE)
	$(CC) $(C_OBJECTS_RELEASE) -o $@ $(LDFLAGS)
	@echo "Release build completed: $(TARGET_RELEASE)"

%_release.o: %.c
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

# Assembly object files (uncomment when you add assembly files)
# %.o: %.asm
#	$(ASM) $(ASMFLAGS) $< -o $@

# Clean build files
clean:
	del /Q *.o $(TARGET_DEBUG) $(TARGET_RELEASE) 2>nul || echo "No files to clean"

# Run debug version
run-debug: $(TARGET_DEBUG)
	$(TARGET_DEBUG)

# Run release version
run-release: $(TARGET_RELEASE)
	$(TARGET_RELEASE)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build both debug and release versions"
	@echo "  debug      - Build debug version (with debugging info)"
	@echo "  release    - Build optimized release version"
	@echo "  clean      - Remove all build files"
	@echo "  run-debug  - Build and run debug version"
	@echo "  run-release- Build and run release version"
	@echo "  help       - Show this help message"

.PHONY: all debug release clean run-debug run-release help

# Notes for assembly integration:
# 1. Uncomment ASM_SOURCES and ASM_OBJECTS when you create .asm files
# 2. Add $(ASM_OBJECTS) to both target dependencies
# 3. Make sure nasm is installed and in your PATH
# 4. Use win64 calling convention in your assembly functions