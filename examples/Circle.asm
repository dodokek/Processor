
main:
    PUSH 100
    POP rax

    ; radius
    PUSH 7
    POP rex

    ; x, y offset from the top-left corner
    PUSH 20
    POP rfx

    PUSH 10
    POP rgx
    ; -------------------------------------

    CALL draw_filled_circle

    HLT

draw_circle:

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
        PUSH 6.28
        JA cycle_draw

RET


draw_filled_circle:

    draw_smaller_rad:
    ; angle
    PUSH 0
    POP rbx

    CALL draw_circle
    PUSH -0.5
    PUSH rex
    ADD
    POP rex

    PUSH 0
    PUSH rex
    JNE draw_smaller_rad
RET
