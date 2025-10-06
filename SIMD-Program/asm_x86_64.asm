; vector_triad_asm_x64.asm

PUBLIC vector_triad_asm_x64
.code

vector_triad_asm_x64 PROC
    ; rcx = n
    ; rdx = A
    ; r8  = B
    ; r9  = C
    ; [rsp+28h] = D

    test    rcx, rcx
    jz      done

    mov     r10, qword ptr [rsp+28h]   ; D pointer
    xor     r11, r11                   ; i = 0

loop:
    cmp     r11, rcx
    jge     done

    movss   xmm0, dword ptr [r8  + r11*4]   ; B[i]
    movss   xmm1, dword ptr [r9  + r11*4]   ; C[i]
    movss   xmm2, dword ptr [r10 + r11*4]   ; D[i]
    mulss   xmm1, xmm2
    addss   xmm0, xmm1
    movss   dword ptr [rdx + r11*4], xmm0

    inc     r11
    jmp     loop

done:
    ret
vector_triad_asm_x64 ENDP
END
