#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "fileUtils.h"
#include "fileUtils.h"
#include "asm.h"

//-----------------------------------------------
const int MAX_BIN_SIZE = 100;
const int MAX_CMDS_AMOUNT = 50;
const int PROC_VERSION = 2;
const int REG_AMOUNT = 4;
const int RAM_SIZE = 100;
//-----------------------------------------------


struct Processor
{
    int version;
    int cmds_amount;
    const char* cmds;

    int Regs[4];

    int* Ram;
};




void StartProc ();

void ParseBinFile (Processor* self, char* code);

void Execute (Processor* Stream);

void ProcessPush (Stack* self, const char* code, int ip, Processor* Stream);

void ProcessPop (Stack* self, const char* code, int ip, Processor* Stream);

void ProcessCommand (Stack* self, const char* code, int* ip, Processor* Stream);

void ProcCtor (Processor* self);

void ProcDtor (Processor* self);

int* GetArg (int cmd, const char* code, Processor* Stream);

#endif