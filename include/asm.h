#ifndef ASM_H
#define ASM_H

#include "fileUtils.h"
#include "stringUtils.h"
#include "stack.h"


enum LEN
{
    PUSH_LEN = 4,
    POP_LEN = 3,
    JMP_LEN = 3,
    J_LEN = 1,
    JMP_LEN_SHORT = 2,
    SERVICE_DATA_LEN = 8,
};


enum OFFSETS
{
    INT_OFFSET = 4,
    MULTI_BYTE_OFFSET = sizeof (elem_t) + 1,
    DEFAULT_CMD_OFFSET = sizeof (char),
    DEFAULT_TWO_CMD_OFFSET = sizeof (elem_t) + 2,
    BYTE_OFFSET = 1,
    JMP_OFFSET = 6,
    ZERO_OFFSET = 0,
};


enum SIZES
{
    VERSION = 2,
    MAX_LABELS = 100, 
    FILL_LABEL_FLAG = -1,
    MAX_CMD_LEN = 20,
};



struct Label 
{
    char name[MAX_CMD_LEN];
    int label_pos; 
};


// 
struct Assembler
{
    char* commands;
    int cur_len; 
    Label* labels; 
    int labels_amount;
};


enum Commands
{
    #define DEF_CMD(name, num, offset, code) \
    name = num,

    #include "codegen/cmds.h"

    #undef DEF_CMD
};


enum Registers
{
    RAX = 0,
    RBX,
    RCX,
    RDX,
    REX,
    RFX,
};


enum ParseData
{
    SG_INDX1 = 0,
    SG_INDX2,
    SG_INDX3,
    VERSION_INDX = 3,
    CMD_AMT_INDX = 4,
};


enum BitMasks
{
    CMD_BITMASK = 0b00011111,
    SPEC_BITMASK = 0b11100000,
    ARG_IMMED = 0b00100000,
    ARG_REG   = 0b01000000,
    ARG_MEM   = 0b10000000,
};

#define _TO_STR(X) #X
#define TO_STR(X) _TO_STR(X)


void RawToBin (Text RawCmd, FILE* CmdFile);

int LineToCommands (char* line, Assembler* AsmInfo);

int GetCmdNum (char* cmd);

void StartAsm();

int HandlePushPop (Assembler* AsmInfo, char* cur_cmd_line, int cmd_type);

int GetRegNum (char* reg);

void ProcessPush (Stack* self, int arr);

int ParseJmp (Assembler* AsmInfo, char* cur_cmd_line, int jmp_type);

bool HandleRam (char* cmd_line);

int InsertLabel (Assembler* AsmInfo, char* label, int label_len);

int IsJmp (Assembler* AsmInfo, char* line);

void FillMissingLabels (Assembler* AsmInfo);

void WriteProgramHeader (Assembler* AsmInfo);

void AsmInfoCtor (Assembler* AsmInfo, Text* RawCmd);

void AsmInfoDtor (Assembler* AsmInfo);

int FindLabel (Assembler* AsmInfo, char* label_name);

void EachLineToBin (Text* RawCmd, Assembler* AsmInfo);

void CmdsToZero (Text* RawCmd, Assembler* AsmInfo);

int IsLabel (char* line, Assembler* AsmInfo);

#endif