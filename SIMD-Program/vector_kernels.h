#pragma once

// Function prototypes for all kernel versions
void vector_triad_c(int n, float* A, const float* B, const float* C, const float* D);
void vector_triad_asm_x64(int n, float* A, const float* B, const float* C, const float* D);
void vector_triad_asm_simd_xmm(int n, float* A, const float* B, const float* C, const float* D);
void vector_triad_asm_simd_ymm(int n, float* A, const float* B, const float* C, const float* D);