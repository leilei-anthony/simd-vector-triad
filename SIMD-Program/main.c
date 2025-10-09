#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <malloc.h>

#define ALIGNED_MALLOC(size, alignment) _aligned_malloc(size, alignment)
#define ALIGNED_FREE(ptr) _aligned_free(ptr)

#include "vector_kernels.h"

// Test configuration
#define MIN_ITERATIONS 30
#define ALIGNMENT 32  // 32-byte alignment for AVX
#define EPSILON 1e-6f // For floating point comparison

// Test sizes - you can modify these
const int TEST_SIZES[] = {
    1024,      // 2^10
    4096,      // 2^12  
    16384,     // 2^14
    65536,     // 2^16
    262144,    // 2^18
    1048576,   // 2^20
    4194304    // 2^22
};
const int NUM_TEST_SIZES = sizeof(TEST_SIZES) / sizeof(TEST_SIZES[0]);

// Timing functions
double get_time() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / frequency.QuadPart;
}

// Memory allocation with alignment
float* allocate_aligned_vector(int n) {
    size_t size = n * sizeof(float);
    float* ptr = (float*)ALIGNED_MALLOC(size, ALIGNMENT);
    if (!ptr) {
        fprintf(stderr, "Error: Failed to allocate %zu bytes\n", size);
        exit(1);
    }
    return ptr;
}
    
// Initialize vectors with your chosen pattern
void initialize_vectors(int n, float* B, float* C, float* D) {
    // Using the pattern from your example: sin, cos, tan functions
    // This creates predictable but varied test data
    for (int i = 0; i < n; i++) {
        B[i] = sinf(i * 0.001f);           // Values between -1 and 1
        C[i] = cosf(i * 0.002f);           // Values between -1 and 1  
        D[i] = tanf(i * 0.0005f + 1.0f);   // Varied values
    }
}

// Verify correctness by comparing results
int verify_results(int n, const float* reference, const float* test, const char* version_name) {
    int errors = 0;
    float max_diff = 0.0f;
    

    for (int i = 0; i < n; i++) {
        float diff = fabsf(reference[i] - test[i]);
        if (diff > EPSILON) {
            if (errors < 10) { // Only print first 10 errors
                printf("Error at index %d: reference=%.6f, %s=%.6f, diff=%.6f\n",
                    i, reference[i], version_name, test[i], diff);
            }
            errors++;
        }
        if (diff > max_diff) {
            max_diff = diff;
        }
    }

    if (errors > 0) {
        // Keep existing FAILED output
        printf("VERIFICATION FAILED for %s: %d errors found, max difference: %.6e\n",
            version_name, errors, max_diff);
        return 0;
    }
    else {
  

        // PASSED output in table format
        printf("%-15s | %-10d | %-15.6e | %-6s |\n",
            version_name, n, max_diff, "PASSED");
        return 1;
    }
}


// Performance testing function
typedef void (*kernel_func_t)(int n, float* A, const float* B, const float* C, const float* D);

typedef struct {
    const char* name;
    kernel_func_t func;
} kernel_version_t;

void benchmark_kernel(const kernel_version_t* kernel, int n,
    float* A, const float* B, const float* C, const float* D,
    double* min_time, double* avg_time, double* max_time) {

    double total_time = 0.0;
    *min_time = INFINITY;
    *max_time = 0.0;

    // Warm-up runs
    for (int i = 0; i < 5; i++) {
        kernel->func(n, A, B, C, D);
    }

    // Actual timing runs
    for (int run = 0; run < MIN_ITERATIONS; run++) {
        // Clear A vector to ensure fresh start
        memset(A, 0, n * sizeof(float));

        double start_time = get_time();
        kernel->func(n, A, B, C, D);
        double end_time = get_time();

        double elapsed = end_time - start_time;
        total_time += elapsed;

        if (elapsed < *min_time) *min_time = elapsed;
        if (elapsed > *max_time) *max_time = elapsed;
    }

    *avg_time = total_time / MIN_ITERATIONS;
}

void print_performance_results(const char* kernel_name, int n,
    double min_time, double avg_time, double max_time) {
    // Calculate performance metrics
    // Vector triad: A[i] = B[i] + C[i] * D[i] 
    // Operations per element: 1 multiply + 1 add = 2 FLOPs
    long long total_ops = (long long)n * 2;
    double gflops = (total_ops / avg_time) / 1e9;

    // Memory bandwidth calculation
    // Reads: B, C, D (3 vectors) + Writes: A (1 vector) = 4 * n * sizeof(float) bytes
    long long total_bytes = (long long)n * 4 * sizeof(float);
    double bandwidth_gb_s = (total_bytes / avg_time) / 1e9;

    printf("%-20s | %8d | %8.3f | %8.3f | %8.3f | %8.2f | %8.2f\n",
        kernel_name, n, min_time * 1000, avg_time * 1000, max_time * 1000,
        gflops, bandwidth_gb_s);
}

void test_boundary_conditions(const kernel_version_t* kernels, int num_kernels) {
    printf("\n=== Testing Boundary Conditions ===\n");

    // Test cases that might cause issues for SIMD implementations
    int boundary_sizes[] = { 1, 3, 5, 7, 15, 17, 31, 33, 1003 }; // Including your example size
    int num_boundary_sizes = sizeof(boundary_sizes) / sizeof(boundary_sizes[0]);

    for (int size_idx = 0; size_idx < num_boundary_sizes; size_idx++) {
        int n = boundary_sizes[size_idx];
        
        printf("\nTesting vector size: %d\n", n);
        printf("----------------|------------|-----------------|---------\n");
        printf("%-15s | %-10s | %-15s | %-6s\n", "Kernel", "Size", "Max Difference", "Result");

        printf("----------------|------------|-----------------|---------\n");

        // Allocate vectors
        float* A_ref = allocate_aligned_vector(n);
        float* A_test = allocate_aligned_vector(n);
        float* B = allocate_aligned_vector(n);
        float* C = allocate_aligned_vector(n);
        float* D = allocate_aligned_vector(n);

        // Initialize test data
        initialize_vectors(n, B, C, D);

        // Run C version as reference
        memset(A_ref, 0, n * sizeof(float));
        kernels[0].func(n, A_ref, B, C, D);

        // Test other kernel versions against reference
        for (int k = 2; k < num_kernels; k++) {
            memset(A_test, 0, n * sizeof(float));
            kernels[k].func(n, A_test, B, C, D);
            verify_results(n, A_ref, A_test, kernels[k].name);
        }

        printf("---------------------------------------------------------\n");
        // Clean up
        ALIGNED_FREE(A_ref);
        ALIGNED_FREE(A_test);
        ALIGNED_FREE(B);
        ALIGNED_FREE(C);
        ALIGNED_FREE(D);
    }
}
int main() {
    printf("Vector Triad Benchmark: A[i] = B[i] + C[i] * D[i]\n");
    printf("========================================================\n");

    kernel_version_t kernels[] = {
        {"C", vector_triad_c},
        {"ASM x86-64", vector_triad_asm_x64},
        {"ASM SIMD XMM", vector_triad_asm_simd_xmm},
        {"ASM SIMD YMM", vector_triad_asm_simd_ymm}
    };
    int num_kernels = sizeof(kernels) / sizeof(kernels[0]);

    printf("Compiled for Windows with high-resolution timing\n");
    printf("Test configuration: %d iterations per test\n", MIN_ITERATIONS);
    printf("Memory alignment: %d bytes\n\n", ALIGNMENT);

    // ======================
// Correctness Verification
// ======================
printf("=== Correctness Verification ===\n");

// Loop over all kernels except C (C is reference)

for (int k = 1; k < num_kernels; k++) {
    printf("----------------|------------|-----------------|---------\n");
    printf("%-15s | %-10s | %-15s | %-6s\n", "Kernel", "Size", "Max Difference", "Result");
    
    printf("----------------|------------|-----------------|---------\n");
    
    for (int size_idx = 0; size_idx < NUM_TEST_SIZES; size_idx++) {
        
        int n = TEST_SIZES[size_idx];

        // Allocate vectors
        float* A_ref = allocate_aligned_vector(n);
        float* A_test = allocate_aligned_vector(n);
        float* B = allocate_aligned_vector(n);
        float* C = allocate_aligned_vector(n);
        float* D = allocate_aligned_vector(n);

        // Initialize vectors
        initialize_vectors(n, B, C, D);

        // Run reference C version
        vector_triad_c(n, A_ref, B, C, D);

        // Run current kernel
        kernels[k].func(n, A_test, B, C, D);

        // Verify and just get PASSED/FAILED
        int result = verify_results(n, A_ref, A_test, kernels[k].name);


        // Clean up
        ALIGNED_FREE(A_ref);
        ALIGNED_FREE(A_test);
        ALIGNED_FREE(B);
        ALIGNED_FREE(C);
        ALIGNED_FREE(D);
    }
}

    // ======================
    // Performance Benchmark
    // ======================
    printf("\n=== Performance Results ===\n");

    for (int size_idx = 0; size_idx < NUM_TEST_SIZES; size_idx++) {
        int n = TEST_SIZES[size_idx];

        // Allocate vectors
        float* A_ref = allocate_aligned_vector(n);
        float* A_test = allocate_aligned_vector(n);
        float* B = allocate_aligned_vector(n);
        float* C = allocate_aligned_vector(n);
        float* D = allocate_aligned_vector(n);

        // Initialize vectors
        initialize_vectors(n, B, C, D);

        printf("\nSize: %d\n", n);
        printf("%-20s | %8s | %8s | %8s | %8s | %8s\n",
            "Kernel", "Avg(ms)", "Min(ms)", "Max(ms)", "GFLOPS", "GB/s");
        printf("---------------------------------------------------------------------------\n");

        for (int k = 0; k < num_kernels; k++) {
            double min_time, avg_time, max_time;

            benchmark_kernel(&kernels[k], n,
                (k == 0) ? A_ref : A_test,
                B, C, D, &min_time, &avg_time, &max_time);

            // Reorder min/max/avg columns
            printf("%-20s | %8.3f | %8.3f | %8.3f | %8.2f | %8.2f\n",
                kernels[k].name,
                avg_time * 1000,
                min_time * 1000,
                max_time * 1000,
                ((long long)n * 2 / avg_time) / 1e9,
                ((long long)n * 4 * sizeof(float) / avg_time) / 1e9);
        }

        ALIGNED_FREE(A_ref);
        ALIGNED_FREE(A_test);
        ALIGNED_FREE(B);
        ALIGNED_FREE(C);
        ALIGNED_FREE(D);
    }

    // Boundary tests remain unchanged
    test_boundary_conditions(kernels, num_kernels);

    printf("\nBenchmark completed successfully!\n");
    return 0;
}

