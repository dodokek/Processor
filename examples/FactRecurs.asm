IN

POP rcx

PUSH 1
POP rax

CALL fact

PUSH rax
OUT
HLT


fact:

    PUSH rcx
    PUSH rax
    MUL
    POP rax

    PUSH rcx
    PUSH -1
    ADD
    POP rcx

    PUSH 1
    PUSH rcx
    JBE greaterone

    CALL fact

    greaterone:

RET



