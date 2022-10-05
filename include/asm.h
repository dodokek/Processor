#ifndef ASM_H
#define ASM_H

#include "fileUtils.h"
#include "stringUtils.h"
#include "stack.h"

const int PUSH_LEN = 4;

enum Commands
{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    DIV,
    OUT,
    HLT,
    DMP,
    IN
};

void RawToBin (Text RawCmd, FILE* CmdFile);

void LineToBin (char* line, FILE* CmdFile);

int GetCmdNum (char* cmd);

#endif