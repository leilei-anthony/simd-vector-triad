; ============================================
; vector_triad_asm_simd_ymm.asm
; A[i] = B[i] + C[i] * D[i]
; Uses AVX (YMM registers)
; Compatible with Windows x64 calling convention
; ============================================

PUBLIC vector_triad_asm_simd_ymm
.code

vector_triad_asm_simd_ymm PROC
    ; Argument registers:
    ; RCX = n (int)
    ; RDX = A (float*)
    ; R8  = B (const float*)
    ; R9  = C (const float*)
    ; [RSP + 28h] = D (const float*)

    test    rcx, rcx
    jle     DONE

    mov     r10, qword ptr [rsp + 28h]    ; Load pointer to D safely
    xor     r11, r11                      ; i = 0

    ; rax = number of 8-float chunks
    mov     rax, rcx
    shr     rax, 3
    jz      TAIL_LOOP                     ; If n < 8, skip SIMD

ALIGN 16
MAIN_LOOP:
    vmovups ymm0, ymmword ptr [r8  + r11*4]    ; B[i..i+7]
    vmovups ymm1, ymmword ptr [r9  + r11*4]    ; C[i..i+7]
    vmovups ymm2, ymmword ptr [r10 + r11*4]    ; D[i..i+7]

    vmulps  ymm1, ymm1, ymm2                   ; C * D
    vaddps  ymm0, ymm0, ymm1                   ; B + (C * D)
    vmovups ymmword ptr [rdx + r11*4], ymm0    ; A[i..i+7] = result

    add     r11, 8
    dec     rax
    jnz     MAIN_LOOP

TAIL_LOOP:
    ; remainder = n % 8
    mov     rax, rcx
    and     rax, 7
    test    rax, rax
    jz      END_SIMD

ALIGN 16
TAIL_SCALAR:
    movss   xmm0, dword ptr [r8  + r11*4]
    movss   xmm1, dword ptr [r9  + r11*4]
    movss   xmm2, dword ptr [r10 + r11*4]
    mulss   xmm1, xmm2
    addss   xmm0, xmm1
    movss   dword ptr [rdx + r11*4], xmm0

    inc     r11
    dec     rax
    jnz     TAIL_SCALAR

END_SIMD:
    vzeroupper          ; prevent AVX-SSE transition penalty

DONE:
    ret
vector_triad_asm_simd_ymm ENDP
END
