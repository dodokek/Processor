#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "fileUtils.h"
#include "fileUtils.h"
#include "asm.h"

struct BinData
{
    int version;
    int cmds_amount;
    char* cmds;
};


const int MAX_BIN_SIZE = 100;
const int MAX_CMDS_AMOUNT = 50;

const int PROC_VERSION = 2;

void StartProc ();

void ParseBinFile (BinData* self, char* arr);

void Execute (char* arr, int len);

void ProcessCommand (Stack* self, char* arr, int* iterator);

#endif