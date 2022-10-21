

main:
    IN
    IN
    IN
    POP rcx
    POP rbx
    POP rax

<<<<<<< HEAD
// a == 0 case
=======
; a == 0 case
>>>>>>> fixing-bug
    PUSH rax
    PUSH 0
    JNE skip_linear
    CALL solve_linear
<<<<<<< HEAD
    skip_linear:

// b == 0 case
=======

skip_linear:

; b == 0 case
>>>>>>> fixing-bug
    PUSH rbx
    PUSH 0
    JNE skip_b_zero
    CALL solve_zero_b
<<<<<<< HEAD
    skip_b_zero:

// Find diskriminant
    CALL solvediskr

// Diskr < 0 case
=======

skip_b_zero:

; Find diskriminant
    CALL solvediskr

; Diskr < 0 case
>>>>>>> fixing-bug
    PUSH rdx   
    PUSH 0
    JA no_soll

<<<<<<< HEAD
// Diskr = 0 case
=======
; Diskr = 0 case
>>>>>>> fixing-bug
    PUSH rdx
    PUSH 0
    JE nulldiskr

<<<<<<< HEAD
// Diskr > 0 case
=======
; Diskr > 0 case
>>>>>>> fixing-bug
    CALL calc_roots

    HLT

<<<<<<< HEAD

=======
>>>>>>> fixing-bug
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
<<<<<<< HEAD
=======

>>>>>>> fixing-bug
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
<<<<<<< HEAD
    // Check b == 0
    PUSH rbx
    PUSH 0
    JE no_soll
    //------------
    //Check c == 0
=======
    ; Check b == 0
    PUSH rbx
    PUSH 0
    JE no_soll
    ;------------
    ;Check c == 0
>>>>>>> fixing-bug
    PUSH rcx
    PUSH 0
    JE inf_sol

    PUSH rbx

    PUSH -1
    PUSH rcx
    MUL

<<<<<<< HEAD

=======
>>>>>>> fixing-bug
    DIV

    OUT
    HLT

solve_zero_b:
<<<<<<< HEAD
    //Check  if c < 0
    PUSH rcx
    PUSH 0
    JE nulldiskr
    //---------------
=======
    ;Check  if c < 0
    PUSH rcx
    PUSH 0
    JE nulldiskr
    ;---------------
>>>>>>> fixing-bug
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
<<<<<<< HEAD
    // 2a
=======
    ; 2a
>>>>>>> fixing-bug
    PUSH rax
    PUSH 2
    MUL
    POP [1]
<<<<<<< HEAD
    // -b
=======
    ; -b
>>>>>>> fixing-bug
    PUSH rbx
    PUSH -1
    MUL
    POP [2]
<<<<<<< HEAD
    // sqr(diskr)
    PUSH rdx
    SQR
    POP [3]
    // Calc first root
=======
    ; sqr(diskr)
    PUSH rdx
    SQR
    POP [3]
    ; Calc first root
>>>>>>> fixing-bug
    PUSH [1]

    PUSH [2]
    PUSH [3]
    ADD
    DIV
<<<<<<< HEAD
    // First root
    POP [4]

    // Calc second root
=======
    ; First root
    POP [4]

    ; Calc second root
>>>>>>> fixing-bug
    PUSH [1]

    PUSH [3]
    PUSH [2]
    SUB
    DIV
<<<<<<< HEAD
    // Second root
    POP [5]
    // OUtput
=======
    ; Second root
    POP [5]
    ; OUtput
>>>>>>> fixing-bug
    PUSH [4]
    PUSH [5]
    OUT
    OUT

    HLT

inf_sol:
    INF_SOL
    HLT
