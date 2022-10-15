#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "fileUtils.h"
#include "fileUtils.h"
#include "asm.h"


//-----------------------------------------------
enum ProcInfo
{
    MAX_BIN_SIZE = 1000,
    MAX_CMDS_AMOUNT = 1000,
    PROC_VERSION = 2,
    REG_AMOUNT = 4,
    RAM_SIZE = 1000,
};
//-----------------------------------------------


struct Processor
{
    int version;

    int cmds_amount; 
    const char* cmds;

    elem_t Regs[4];
    elem_t* Ram;

    Stack CallStack;
};


void StartProc ();

void ParseBinFile (Processor* self, char* code);

void Execute (Processor* CpuInfo);

void ProcessCommand (Stack* self, const char* code, int* ip, Processor* CpuInfo);

void ProcCtor (Processor* self);

void ProcDtor (Processor* self);

elem_t* GetArg (int cmd, const char* code, Processor* CpuInfo, elem_t* val);

void DrawMemory (Processor* CpuInfo);

#endif