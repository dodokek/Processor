IN
POP rcx

PUSH 1
POP rbx

beb:

PUSH 1 + rax
POP rax

PUSH rax
PUSH rbx
MLT
POP rbx

PUSH rax
PUSH rcx

JAE beb

PUSH rbx


OUT
HTL
