; ============================================================
; File: asm_simd_xmm.asm
; ------------------------------------------------------------
; Argument registers
; ------------------------------------------------------------
; RCX = n (int)                 ; length of vectors
; RDX = A (float*)              ; destination
; R8  = B (float*)              ; vector
; R9  = C (float*)              ; vector
; [RSP + 28h] = D (float*)      ; vector
; ============================================================

PUBLIC vector_triad_asm_simd_xmm
.code

vector_triad_asm_simd_xmm PROC
    test    rcx, rcx
    jle     DONE

    mov     r10, qword ptr [rsp + 28h]      ; Load pointer to D
    xor     r11, r11                        ; Set i = 0

    ; rax = number of 4-float chunks
    mov     rax, rcx
    shr     rax, 2                          ; rax = rcx / 4
    jz      TAIL_LOOP                       ; If n < 4

ALIGN 16
MAIN_LOOP:
    vmovups xmm0, xmmword ptr [r8  + r11*4]     ; B[i..i+3]
    vmovups xmm1, xmmword ptr [r9  + r11*4]     ; C[i..i+3]
    vmovups xmm2, xmmword ptr [r10 + r11*4]     ; D[i..i+3]

    vmulps  xmm1, xmm1, xmm2                    ; C * D
    vaddps  xmm0, xmm0, xmm1                    ; B + (C*D)

    vmovups xmmword ptr [rdx + r11*4], xmm0     ; A[i..i+3] = result

    add     r11, 4
    dec     rax
    jnz     MAIN_LOOP

TAIL_LOOP:
    ; remainder = n % 4
    mov     rax, rcx
    and     rax, 3
    test    rax, rax
    jz      END_SIMD        ; If remainder == 0

ALIGN 16
TAIL_SCALAR:
    ; Scalar loop for remaining vectors
    movss   xmm0, dword ptr [r8  + r11*4]     ; B[i]
    movss   xmm1, dword ptr [r9  + r11*4]     ; C[i]
    movss   xmm2, dword ptr [r10 + r11*4]     ; D[i]

    mulss   xmm1, xmm2                        ; C*D
    addss   xmm0, xmm1                        ; B + (C*D)

    movss   dword ptr [rdx + r11*4], xmm0     ; A[i] = result

    inc     r11
    dec     rax
    jnz     TAIL_SCALAR

END_SIMD:
    vzeroupper

DONE:
    ret

vector_triad_asm_simd_xmm ENDP
END
