# Vector Triad Performance Analysis

**Author:** Aira Jin Garganera, Nigel Roi Nograles, Lester Anthony Sityar Jr.

**Course:** CSC612M Advanced Computer Architecture - G03

**Date:** October 10th, 2025

---

## Overview

This project implements and analyzes a **Vector Triad** operation:

`[A[i] = B[i] + C[i] * D[i]]`

where `A`, `B`, `C`, and `D` are single-precision floating-point vectors.
The goal is to compare four implementations:

1. C version (reference)
2. x86-64 Assembly (non-SIMD)
3. x86-64 Assembly using XMM registers (SSE)
4. x86-64 Assembly using YMM registers (AVX)

Each kernel is verified for correctness and benchmarked to determine whether the operation is memory-bound or compute-bound.

---

## Files

| File                       | Description                                                               |
| -------------------------- | ------------------------------------------------------------------------- |
| `main.c`                   | Main program: initializes vectors, calls kernels, measures execution time |
| `kernels.h`                | Kernel function declarations                                              |
| `kernels_c.c`              | C version of vector triad                                                 |
| `vector_triad_asm_x64.asm` | Non-SIMD assembly version                                                 |
| `vector_triad_xmm_asm.asm` | SSE (XMM) assembly version                                                |
| `vector_triad_ymm_asm.asm` | AVX (YMM) assembly version                                                |
| `README.md`                | Project documentation and analysis                                        |

---

## Screenshots of Program Output

### Execution Times

![Execution Times](Screenshots/Release/PR.png)

---

### Correctness Verification

**Non-SIMD x86-64:**

![ASM Verification](Screenshots/Release/CV-X86.png)

**SIMD XMM Version:**

![XMM Verification](Screenshots/Release/CV-XMM.png)

**SIMD YMM Version:**

![YMM Verification](Screenshots/Release/CV-YMM.png)

---

### Boundary Tests

![Boundary Tests](Screenshots/Release/BC-1.png)
![Boundary Tests](Screenshots/Release/BC-2.png)

---

## Comparative Table of Execution Times - Release

| **Size**    | **Kernel**   | **Avg (ms)** | **Min (ms)** | **Max (ms)** | **GFLOPS** | **GB/s** |
| ----------- | ------------ | ------------ | ------------ | ------------ | ---------- | -------- |
| **1024**    | C            | 0.000        | 0.000        | 0.000        | 24.58      | 196.61   |
|             | ASM x86-64   | 0.000        | 0.000        | 0.001        | 4.69       | 37.52    |
|             | ASM SIMD XMM | 0.000        | 0.000        | 0.000        | 18.62      | 148.95   |
|             | ASM SIMD YMM | 0.000        | 0.000        | 0.001        | 20.48      | 163.84   |
| **4096**    | C            | 0.001        | 0.000        | 0.001        | 14.21      | 113.65   |
|             | ASM x86-64   | 0.002        | 0.002        | 0.002        | 4.31       | 34.49    |
|             | ASM SIMD XMM | 0.001        | 0.001        | 0.001        | 12.54      | 100.31   |
|             | ASM SIMD YMM | 0.001        | 0.000        | 0.001        | 15.96      | 127.67   |
| **16384**   | C            | 0.004        | 0.004        | 0.005        | 7.33       | 58.60    |
|             | ASM x86-64   | 0.015        | 0.009        | 0.073        | 2.15       | 17.22    |
|             | ASM SIMD XMM | 0.004        | 0.004        | 0.004        | 7.64       | 61.15    |
|             | ASM SIMD YMM | 0.004        | 0.004        | 0.004        | 7.95       | 63.63    |
| **65536**   | C            | 0.011        | 0.009        | 0.027        | 12.30      | 98.37    |
|             | ASM x86-64   | 0.026        | 0.024        | 0.037        | 5.08       | 40.67    |
|             | ASM SIMD XMM | 0.011        | 0.010        | 0.014        | 12.00      | 95.96    |
|             | ASM SIMD YMM | 0.011        | 0.009        | 0.048        | 11.64      | 93.12    |
| **262144**  | C            | 0.106        | 0.074        | 0.635        | 4.95       | 39.60    |
|             | ASM x86-64   | 0.098        | 0.093        | 0.116        | 5.37       | 42.94    |
|             | ASM SIMD XMM | 0.079        | 0.075        | 0.126        | 6.67       | 53.37    |
|             | ASM SIMD YMM | 0.078        | 0.075        | 0.089        | 6.75       | 53.98    |
| **1048576** | C            | 0.555        | 0.352        | 1.411        | 3.78       | 30.22    |
|             | ASM x86-64   | 0.723        | 0.519        | 1.388        | 2.90       | 23.31    |
|             | ASM SIMD XMM | 0.549        | 0.380        | 1.822        | 3.82       | 30.55    |
|             | ASM SIMD YMM | 0.436        | 0.353        | 0.816        | 4.81       | 38.45    |
| **4194304** | C            | 2.547        | 2.071        | 3.501        | 3.29       | 26.34    |
|             | ASM x86-64   | 2.784        | 2.318        | 3.920        | 3.01       | 24.10    |
|             | ASM SIMD XMM | 2.765        | 2.256        | 4.337        | 3.03       | 24.27    |
|             | ASM SIMD YMM | 2.827        | 2.146        | 3.850        | 2.97       | 23.74    |

---

## Comparative Table of Execution Times - Debug

| **Size**    | **Kernel**   | **Avg (ms)** | **Min (ms)** | **Max (ms)** | **GFLOPS** | **GB/s** |
| ----------- | ------------ | ------------ | ------------ | ------------ | ---------- | -------- |
| **1024**    | C            | 0.001        | 0.001        | 0.002        | 1.53       | 12.23    |
|             | ASM x86-64   | 0.000        | 0.000        | 0.000        | 5.12       | 40.96    |
|             | ASM SIMD XMM | 0.000        | 0.000        | 0.000        | 17.07      | 136.53   |
|             | ASM SIMD YMM | 0.000        | 0.000        | 0.000        | 21.19      | 169.49   |
| **4096**    | C            | 0.006        | 0.005        | 0.013        | 1.43       | 11.46    |
|             | ASM x86-64   | 0.002        | 0.001        | 0.009        | 4.64       | 37.10    |
|             | ASM SIMD XMM | 0.001        | 0.000        | 0.001        | 15.55      | 124.44   |
|             | ASM SIMD YMM | 0.000        | 0.000        | 0.000        | 21.94      | 175.54   |
| **16384**   | C            | 0.021        | 0.020        | 0.025        | 1.53       | 12.28    |
|             | ASM x86-64   | 0.007        | 0.007        | 0.008        | 4.70       | 37.63    |
|             | ASM SIMD XMM | 0.003        | 0.003        | 0.004        | 10.28      | 82.26    |
|             | ASM SIMD YMM | 0.003        | 0.003        | 0.004        | 11.73      | 93.85    |
| **65536**   | C            | 0.111        | 0.076        | 0.194        | 1.18       | 9.47     |
|             | ASM x86-64   | 0.030        | 0.024        | 0.151        | 4.30       | 34.43    |
|             | ASM SIMD XMM | 0.009        | 0.009        | 0.011        | 13.92      | 111.39   |
|             | ASM SIMD YMM | 0.008        | 0.007        | 0.016        | 17.25      | 138.03   |
| **262144**  | C            | 0.365        | 0.298        | 0.624        | 1.44       | 11.49    |
|             | ASM x86-64   | 0.139        | 0.115        | 0.296        | 3.77       | 30.14    |
|             | ASM SIMD XMM | 0.081        | 0.073        | 0.255        | 6.45       | 51.59    |
|             | ASM SIMD YMM | 0.104        | 0.075        | 0.318        | 5.03       | 40.27    |
| **1048576** | C            | 1.742        | 1.234        | 3.134        | 1.20       | 9.63     |
|             | ASM x86-64   | 0.544        | 0.449        | 0.838        | 3.85       | 30.83    |
|             | ASM SIMD XMM | 0.460        | 0.372        | 0.772        | 4.56       | 36.49    |
|             | ASM SIMD YMM | 0.612        | 0.356        | 1.262        | 3.43       | 27.42    |
| **4194304** | C            | 6.523        | 5.014        | 9.896        | 1.29       | 10.29    |
|             | ASM x86-64   | 3.836        | 2.780        | 4.936        | 2.19       | 17.50    |
|             | ASM SIMD XMM | 3.082        | 2.393        | 4.840        | 2.72       | 21.77    |
|             | ASM SIMD YMM | 3.454        | 2.123        | 8.042        | 2.43       | 19.43    |

### Metrics

- Average Time Relative to C (×) - This shows how much faster each kernel runs compared to the baseline C code.
- GFLOPS Relative to C (×) - This measures how much higher the floating-point computation rate (billion FLOPs per second) is compared to C.
- GB/s Relative to C (×) - This measures how much more memory bandwidth (gigabytes per second) each kernel achieves relative to the C baseline.

Each performance metric rate compares the assembly implementations (x86-64, SIMD XMM, and SIMD YMM) against the baseline C implementation to show how much faster or more efficient each kernel performs.

## Performance Analysis - Release

| **Kernel**       | **Avg (ms) Relative to C (×)** | **GFLOPS Relative to C (x)** | **GB/s Relative to C (x)** |
| ---------------- | ------------------------------ | ---------------------------- | -------------------------- |
| **C**            | 1.00x baseline                 | 1.00x baseline               | 1.00x baseline             |
| **ASM x86-64**   | 1.13x slower                   | 0.39x lower                  | 0.39x lower                |
| **ASM SIMD XMM** | 1.06x slower                   | 0.91x lower                  | 0.91x lower                |
| **ASM SIMD YMM** | 1.04x slower                   | 1.00x lower                  | 1.00x lower                |

## Performance Analysis - Debug

| **Kernel**       | **Avg (ms) Relative to C (×)** | **GFLOPS Relative to C (x)** | **GB/s Relative to C (x)** |
| ---------------- | ------------------------------ | ---------------------------- | -------------------------- |
| **C**            | 1.00x baseline                 | 1.00x baseline               | 1.00x baseline             |
| **ASM x86-64**   | 1.92x faster                   | 2.98x higher                 | 2.97x higher               |
| **ASM SIMD XMM** | 2.41x faster                   | 7.35x higher                 | 7.35x higher               |
| **ASM SIMD YMM** | 2.10x faster                   | 8.65x higher                 | 8.64x higher               |

---

### Discussion of Performance

- **C Implementation** – In Release mode, the compiler applies heavy optimizations and automatic SIMD vectorization, making the C version the fastest. In Debug mode, these optimizations are disabled, causing significant slowdowns due to unoptimized, scalar execution and added debugging overhead.
- **ASM (Non-SIMD)** – The non-SIMD assembly performs better than unoptimized C in Debug but slower than optimized C in Release. Without SIMD, it becomes limited by instruction throughput and memory access speed.
- **XMM (SSE)** – The XMM version achieves notable gains in Debug, processing four floats per instruction. However, its advantage fades in Release since the compiler likely emits similar SSE code for C.
- **YMM (AVX)** – The YMM version performs best in Debug, doubling throughput over XMM. In Release, its performance nearly matches optimized C, which may already use AVX.
- **Memory vs Compute Bound** – At small sizes, performance is compute-bound, benefiting from SIMD. At larger sizes, all kernels become memory-bound, reducing the performance gap between implementations.

### Analysis of Performance

The comparative results show how compiler optimization and vector width influence execution time. In Debug mode, the unoptimized C implementation performs the slowest, while all assembly kernels execute significantly faster due to direct instruction control and reduced overhead. The XMM and YMM kernels were identical except for register width, yet XMM achieves slightly shorter execution times. This counterintuitive result suggests that the additional width of YMM registers does not yield proportional speedups because the operation is largely memory-bound, not compute-bound. Wider YMM loads increase memory traffic, and minor alignment or scheduling penalties can offset their theoretical advantage.

In Release mode, full compiler optimizations narrow performance gaps. The optimized C kernel executes the fastest overall, with the assembly versions only 1.04 to 1.13x slower on average. This indicates that the compiler’s auto-vectorization already generates efficient code. As the input size increases, execution times scale predictably: small arrays fit within cache and benefit from high instruction throughput, while larger arrays exceed cache capacity, causing all kernels to slow down similarly due to memory bandwidth limits.

Overall, execution time trends confirm that SIMD width alone does not guarantee faster performance. XMM remains slightly faster because it balances throughput and memory access without the added latency and power overhead of 256-bit YMM operations.

---

## Discussion

### Problems Encountered and Solutions

- **Build Configuration Issues** - Encountered difficulties building and linking the C and assembly files in Visual Studio 2022 due to mismatched project settings.
  > **Solution** - Adjusted the project configuration to ensure correct calling conventions and linking between `.c` and `.asm` files.
- **Memory Alignment Isseus** - The program crashed with exit code 0x80000003, indicating a breakpoint or memory alignment issue.
  > **Solution** - Fixed alignment problems by ensuring proper use of `_aligned_malloc` and verifying that SIMD operations accessed valid, aligned memory addresses.
- **Boundary Handling** - Initially attempted to use masking to handle remainder elements that didn’t fit evenly into SIMD vector widths.
  > **Solution** - Since masking added unnecessary complexity and gave no performance gain for small remainders, we instead processed the remaining elements using a scalar vector triad for simpler and more stable results.
- **Performance Discrepancy Between Debug and Release Versions** – Noticed that the C implementation outperformed the assembly versions in the Release build, but not in Debug mode.
  > **Solution** – Investigated compiler optimizations and confirmed that the compiler’s aggressive auto-vectorization and inlining in Release mode were the main cause. This highlighted how compiler optimizations can outperform manual assembly under certain conditions.

---

### Insights / AHA Moments

- “Wider” isn’t always faster: Even though both XMM and YMM use AVX2, XMM runs faster because it avoids AVX downclocking, reduces memory traffic, and sustains higher instruction throughput.
- SIMD speedups taper beyond cache size: After ~262K elements, data exceeds the cache, making the workload memory-bound where DRAM bandwidth limits performance.
- Using SIMD registers directly in assembly clarified instruction-level parallelism.
- Averaging multiple runs smoothed out timing noise for accurate measurements.
- Explicitly comparing all four versions highlighted the performance impact of SIMD instructions.
- Observing performance differences between Release and Debug builds revealed how compiler optimizations and vectorization significantly affect execution speed.

---

## Conclusion

- Compiler optimizations in the Release build allow the C version to perform nearly as efficiently as the assembly implementations.
- In the Debug build, assembly, especially SIMD versions shows significantly higher performance due to the lack of compiler optimizations.
- Manual low-level optimization can outperform unoptimized C code but offers diminishing returns when compiler optimizations are enabled.

---
