; ============================================================
; File: vector_triad_asm_simd_xmm.asm
; ------------------------------------------------------------
; RCX = n (int)                 ; length of vectors
; RDX = A (float*)              ; destination
; R8  = B (float*)              ; vector
; R9  = C (float*)              ; vector
; [RSP + 0x28] = D (float*)     ; vector (Windows x64 calling convention)
; ------------------------------------------------------------
; Performs A[i] = B[i] + C[i] * D[i]
; ============================================================


section .text
global vector_triad_asm_x64

vector_triad_asm_x64:
    test    rcx, rcx
    jz      .done

    mov     r10, [rsp+0x28]       ; D pointer
    xor     r11, r11              ; i = 0

.loop:
    cmp     r11, rcx
    jge     .done

    movss   xmm0, [r8  + r11*4]   ; B[i]
    movss   xmm1, [r9  + r11*4]   ; C[i]
    movss   xmm2, [r10 + r11*4]   ; D[i]
    mulss   xmm1, xmm2
    addss   xmm0, xmm1
    movss   [rdx + r11*4], xmm0   ; A[i] = B[i] + C[i] * D[i]

    inc     r11
    jmp     .loop

.done:
    ret
