#ifndef ASM_H
#define ASM_H

#include "fileUtils.h"
#include "stringUtils.h"
#include "stack.h"


const int PUSH_LEN = 4;
const int VERSION = 2;


enum Commands
{
    PUSH = 1,
    MLT,
    ADD,
    SUB,
    DIV,
    OUT,
    HLT,
    DMP,
    IN
};


void RawToBin (Text RawCmd, FILE* CmdFile);

int LineToCommands (char* line, int* commands, int cmds_amount);

int GetCmdNum (char* cmd);

void StartAsm();

#endif