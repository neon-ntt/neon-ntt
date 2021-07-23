
.align 2
.global __asm_state_interleave
.type __asm_state_interleave, %function
__asm_state_interleave:

    mov x3, #25

    _state_interleave_loop:

    ldr x4, [x1], #8
    ldr x5, [x2], #8

    stp x4, x5, [x0], #16

    sub x3, x3, #1
    cbnz x3, _state_interleave_loop

    br lr


.align 2
.global __asm_state_deinterleave
.type __asm_state_deinterleave, %function
__asm_state_deinterleave:

    mov x3, #25

    _state_deinterleave_loop:

    ldp x4, x5, [x2], #16

    str x4, [x0], #8
    str x5, [x1], #8

    sub x3, x3, #25
    cbnz x3, _state_deinterleave_loop

    br lr



 




















