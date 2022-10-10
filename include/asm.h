#ifndef ASM_H
#define ASM_H

#include "fileUtils.h"
#include "stringUtils.h"
#include "stack.h"


const int PUSH_LEN = 4 ;
const int POP_LEN = 3 ;
const int JMP_LEN = 3 ;
const int JMP_LEN_SHORT = 2;
const int VERSION = 2;

const int INT_OFFSET = 4;
const int DEFAULT_CMD_OFFSET = 1;
const int DEFAULT_TWO_CMD_OFFSET = 5;
const int ZERO_OFFSET = 0;
const int WORK_DATA_LEN = 5;
const int MAX_LABELS = 10;

const int FILL_LABEL_FLAG = -1;


enum Commands
{
    HLT = 0,
    PUSH = 1,
    MLT,
    ADD,
    SUB,
    DIV,
    POP,
    OUT,
    DMP,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
    IN
};


enum Registers
{
    RAX = 0,
    RBX,
    RCX,
    RDX
};


enum ParseData
{
    VERSION_INDX = 0,
    CMD_AMT_INDX,
    SG_INDX1,
    SG_INDX2,
    SG_INDX3,
};


enum BitMasks
{
    CMD_BITMASK = 0b00011111,
    SPEC_BITMASK = 0b11100000,
    ARG_IMMED = 0b00100000,
    ARG_REG   = 0b01000000,
    ARG_MEM   = 0b10000000,
};

void RawToBin (Text RawCmd, FILE* CmdFile);

int LineToCommands (char* line, char* commands, int bin_size);

int GetCmdNum (char* cmd);

void StartAsm();

void IntToChar (char* arr, const int* num);

int ParseCmd (char* commands, int cmd_iter, char* cur_cmd_line, int operation);

int GetRegNum (char* reg);

void ProcessPush (Stack* self, int arr);

int ParseJmp (char* commands, int cmd_iter, char* cur_cmd_line, int operation);

bool HandleRam (char* cmd_line);

int ParseLabel (char* line, int bin_size);

int RecognizeJmp (char* commands, int bin_size, char* line);

#endif