

main:
    IN
    IN
    IN
    POP rcx
    POP rbx
    POP rax

; a == 0 case
    PUSH rax
    PUSH 0
    JNE skip_linear
    CALL solve_linear

skip_linear:

; b == 0 case
    PUSH rbx
    PUSH 0
    JNE skip_b_zero
    CALL solve_zero_b

skip_b_zero:

; Find diskriminant
    CALL solvediskr

; Diskr < 0 case
    PUSH rdx   
    PUSH 0
    JA no_soll

; Diskr = 0 case
    PUSH rdx
    PUSH 0
    JE nulldiskr

; Diskr > 0 case
    CALL calc_roots

    HLT

solvediskr:
    PUSH rax
    PUSH rcx

    MUL
    PUSH 4
    MUL
    PUSH -1
    MUL
    POP [1]

    PUSH rbx
    PUSH rbx
    MUL
    POP [2]

    PUSH [2]
    PUSH [1]

    ADD
    POP rdx

    RET

no_soll:

    PUSH rcx
    PUSH 0
    JE inf_sol
    ZEROSOL

    HLT

nulldiskr:

    PUSH 2
    PUSH rax
    MUL

    PUSH -1
    PUSH rbx
    MUL

    DIV

    OUT
    HLT

solve_linear:
    ; Check b == 0
    PUSH rbx
    PUSH 0
    JE no_soll
    ;------------
    ;Check c == 0
    PUSH rcx
    PUSH 0
    JE inf_sol

    PUSH rbx

    PUSH -1
    PUSH rcx
    MUL

    DIV

    OUT
    HLT

solve_zero_b:
    ;Check  if c < 0
    PUSH rcx
    PUSH 0
    JE nulldiskr
    ;---------------
    PUSH rax

    PUSH rcx
    PUSH -1
    MUL

    DIV
    SQR
    POP [3]

    PUSH [3]
    PUSH -1
    MUL
    POP [4]

    PUSH [3]
    PUSH [4]

    OUT
    OUT
    HLT

calc_roots:
    MEOW
    ; 2a
    PUSH rax
    PUSH 2
    MUL
    POP [1]
    ; -b
    PUSH rbx
    PUSH -1
    MUL
    POP [2]
    ; sqr(diskr)
    PUSH rdx
    SQR
    POP [3]
    ; Calc first root
    PUSH [1]

    PUSH [2]
    PUSH [3]
    ADD
    DIV
    ; First root
    POP [4]

    ; Calc second root
    PUSH [1]

    PUSH [3]
    PUSH [2]
    SUB
    DIV
    ; Second root
    POP [5]
    ; OUtput
    PUSH [4]
    PUSH [5]
    OUT
    OUT

    HLT

inf_sol:
    INF_SOL
    HLT
