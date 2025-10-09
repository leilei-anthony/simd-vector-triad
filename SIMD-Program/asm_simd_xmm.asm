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

default rel
section .text
global vector_triad_asm_simd_xmm

vector_triad_asm_simd_xmm:
    test    rcx, rcx
    jle     .done

    mov     r10, qword [rsp + 0x28]        ; Load pointer to D
    xor     r11, r11                 ; Set i = 0

    mov     rax, rcx
    shr     rax, 2                   ; rax = rcx / 4
    jz      .tail_loop               ; If n < 4, skip SIMD

align 16
.main_loop:
    vmovups xmm0, [r8  + r11*4]      ; B[i..i+3]
    vmovups xmm1, [r9  + r11*4]      ; C[i..i+3]
    vmovups xmm2, [r10 + r11*4]      ; D[i..i+3]

    vmulps  xmm1, xmm1, xmm2         ; C * D
    vaddps  xmm0, xmm0, xmm1         ; B + (C*D)

    vmovups [rdx + r11*4], xmm0      ; A[i..i+3] = result

    add     r11, 4
    dec     rax
    jnz     .main_loop

.tail_loop:
    mov     rax, rcx
    and     rax, 3
    test    rax, rax
    jz      .end_simd                ; no remainder

align 16
.tail_scalar:
    movss   xmm0, dword [r8  + r11*4]      ; B[i]
    movss   xmm1, dword [r9  + r11*4]      ; C[i]
    movss   xmm2, dword [r10 + r11*4]      ; D[i]

    mulss   xmm1, xmm2               ; C * D
    addss   xmm0, xmm1               ; B + (C*D)

    movss   dword [rdx + r11*4], xmm0      ; A[i] = result

    inc     r11
    dec     rax
    jnz     .tail_scalar

.end_simd:
    vzeroupper

.done:
    ret
