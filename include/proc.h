#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "fileUtils.h"
#include "fileUtils.h"
#include "asm.h"

struct BinData
{
    int version;
    int password;
    int cmds_amount;
    int* cmds;
};


const int MAX_BIN_SIZE = 100;
const int MAX_CMDS_AMOUNT = 50;

const int PROC_VERSION = 2;

void StartProc ();

void ParseBin (BinData* self, int* arr);

void Execute (int* arr, int len);

void ProcessCommand (Stack* self, int* arr);

#endif