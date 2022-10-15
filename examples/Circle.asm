
main:
    PUSH 10
    POP rax

    // angle
    PUSH 1
    POP rbx

    CALL draw
    HLT

draw:

    cycle_draw:

    // y coord
    PUSH rbx
    SIN
    PUSH 10
    MUL
    ROUND
    POP rcx
    POP [rcx]

    PUSH 628
    PUSH rbx

    JAE cycle_draw

RET
