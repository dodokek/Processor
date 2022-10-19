
main:
    PUSH 100
    POP rax

    ; angle
    PUSH 
    POP rbx

    ; radius
    PUSH 7
    POP rex

    ; x, y offset from the top-left corner
    PUSH 20
    POP rfx

    PUSH 10
    POP rgx
    ; end of circle params

    CALL draw

    HLT

draw:

    cycle_draw:

        ; y coord
        PUSH rbx
        SIN
        PUSH rex
        MUL
        ROUND
        PUSH rgx
        ADD
        ; sin (angle) * r
        POP rcx 
        ; DMP

        ; x coord
        PUSH rbx
        COS
        PUSH rex
        MUL
        PUSH 2
        MUL
        ROUND
        PUSH rfx
        ADD
        ; 2 * cos (angle) * r
        POP rdx 
        ; DMP

        ; drawing memory
        PUSH 60
        PUSH rcx
        MUL
        PUSH rdx
        ADD
        POP rax


        PUSH rax
        POP [rax]

        ; counter incrementing
        PUSH rbx
        PUSH 0.05
        ADD
        POP rbx

        ; end of cycle case
        PUSH rbx
        PUSH 6.18
        JA cycle_draw

RET
