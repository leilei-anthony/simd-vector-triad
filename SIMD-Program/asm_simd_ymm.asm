; vector_triad_asm_simd_ymm_nasm.asm
; A[i] = B[i] + C[i] * D[i]
; AVX (YMM) version, Windows x64 calling convention
; Strict NASM syntax

default rel

section .text
    global vector_triad_asm_simd_ymm

vector_triad_asm_simd_ymm:
    ; Arguments (Windows x64):
    ; RCX = n (int)
    ; RDX = A (float*)
    ; R8  = B (const float*)
    ; R9  = C (const float*)
    ; [RSP + 0x28] = D (const float*)

    test    rcx, rcx
    jle     .DONE

    mov     r10, qword [rsp + 0x28]    ; Load pointer to D
    xor     r11, r11                   ; i = 0

    ; rax = n / 8
    mov     rax, rcx
    shr     rax, 3
    jz      .TAIL_LOOP

    align 16
.MAIN_LOOP:
    vmovups ymm0, [r8  + r11*4]        ; B[i..i+7]
    vmovups ymm1, [r9  + r11*4]        ; C[i..i+7]
    vmovups ymm2, [r10 + r11*4]        ; D[i..i+7]

    vmulps  ymm1, ymm1, ymm2           ; C * D
    vaddps  ymm0, ymm0, ymm1           ; B + (C * D)
    vmovups [rdx + r11*4], ymm0        ; A[i..i+7] = result

    add     r11, 8
    dec     rax
    jnz     .MAIN_LOOP

.TAIL_LOOP:
    mov     rax, rcx
    and     rax, 7
    test    rax, rax
    jz      .END_SIMD

    align 16
.TAIL_SCALAR:
    movss   xmm0, dword [r8  + r11*4]
    movss   xmm1, dword [r9  + r11*4]
    movss   xmm2, dword [r10 + r11*4]
    mulss   xmm1, xmm2
    addss   xmm0, xmm1
    movss   dword [rdx + r11*4], xmm0

    inc     r11
    dec     rax
    jnz     .TAIL_SCALAR

.END_SIMD:
    vzeroupper

.DONE:
    ret
