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
<<<<<<< HEAD
<<<<<<< HEAD
    RAM_SIZE = 1000,
=======
    RAM_SIZE = 3000,
>>>>>>> fixing-bug
=======
    RAM_SIZE = 3000,
>>>>>>> fixing-bug
};
//-----------------------------------------------


struct Processor
{
    int version;

    int cmds_amount; 
    const char* cmds;

    elem_t Regs[10];
    elem_t* Ram;

    Stack CallStack;
};


void StartProc ();

<<<<<<< HEAD
void ParseBinFile (Processor* CpuInfo, char* code);
=======
void ReadHeader (Processor* CpuInfo, char* code);
>>>>>>> fixing-bug

void Execute (Processor* CpuInfo);

int ProcessCommand (Stack* self, const char* code, int* ip, Processor* CpuInfo);

void ProcCtor (Processor* CpuInfo);

void ProcDtor (Processor* CpuInfo);

elem_t* GetArg (int cmd, const char* code, Processor* CpuInfo, elem_t* val);

void DrawMemory (Processor* CpuInfo);

<<<<<<< HEAD
=======
char* ReadCommandsFile ();

>>>>>>> fixing-bug
#endif