void vector_triad_c(int n, float* A, const float* B, const float* C, const float* D) {
    for (int i = 0; i < n; i++) {
        A[i] = B[i] + C[i] * D[i];
    }
}