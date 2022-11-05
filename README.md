# Processor
The simulation of Assembler work.

## Overview

In this project i wrote my own translator from assembly language to the binary file and subsequent execution.

### Assembly
First of all, we need to write a program on assembly language, which need to be stored in folder "data". 
~~~
PUSH 10
PUSH 20
POP rcx
PUSH rcx
ADD
OUT
HLT
~~~

To run the translation you need:
~~~
make asm
./asm.exe
~~~

After that they will be translated to binary file in the same folder.



### CPU
Execute your code with 
~~~
make proc
./proc.exe
~~~

You will be given an output in the console.\

### Commands
Here are the most common commands that you might need to use
- PUSH
- POP
- MUL
- ADD
- SUB
- JMP
- JBE
- CALL
- RET

### Code generation

Thanks to commands generation you can easily add new commands, by changing file cmds.h following the same pattern.

### RAM and registers operations

You can call the RAM, which would be visualized in the console, moreover you can operate whith registers, such as rax, rbx, rcx

