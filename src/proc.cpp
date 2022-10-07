
#include "../include/proc.h"

void StartProc ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");
    char* buffer = (char*) calloc (sizeof (int), MAX_BIN_SIZE);
    fread (buffer, sizeof (int), MAX_CMDS_AMOUNT, CmdFile);

    Processor Stream = {};
    ProcCtor (&Stream);

    ParseBinFile (&Stream, buffer); 
    if (Stream.version != PROC_VERSION) printf ("Wrong bin file!");
    Execute (&Stream);

    FREE(buffer);
    fclose (CmdFile);
    ProcDtor (&Stream);
}


void ParseBinFile (Processor* self, char* code)
{
    self->version = code[VERSION_INDX];
    self->cmds_amount = code[CMD_AMT_INDX];

    if (code[SG_INDX1] != 'C' || code[SG_INDX2] != 'U' || code[SG_INDX3] != 'M') printf ("Wrong signature!\n");

    printf ("Cur Version: %d, Curr Cmd Amount: %d \n\n", code[VERSION_INDX], code[CMD_AMT_INDX]);

    self->cmds = code + WORK_DATA_LEN; //Служебная информация хз как перевести
    
}


void Execute (Processor* Stream)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int cmd_counter = 0; cmd_counter < Stream->cmds_amount; cmd_counter++)
    {
        ProcessCommand (&MainStack, Stream->cmds + cmd_counter, &cmd_counter, Stream);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, const char* code, int* ip, Processor* Stream)
{
    printf ("Currently working on %d\n", *code & CMD_BITMASK);
    switch (*code & CMD_BITMASK)
    {
    case PUSH:
        ProcessPush (self, code, *ip, Stream);
        (*ip) += INT_OFFSET;
        break;

    case ADD:
        StackPush (self, StackPop(self) + StackPop(self));
        break;

    case SUB:
        StackPush (self, -StackPop (self) + StackPop (self));
        break;

    case DIV:
        StackPush (self, StackPop(self) / StackPop(self));
        break;

    case OUT:
        printf ("Stack out: %d\n", StackPop (self));
        break;

    case HLT:
        printf ("End of commands\n");
        break;

    default:
        printf ("UK %d\n", *code);
        break;
    }
}


void ProcessPush (Stack* self, const char* code, int ip, Processor* Stream)
{
    int cmd = *code;
    printf ("Rn we have command: %d\n", cmd & SPEC_BITMASK);
    int arg = 0;

    if (cmd & ARG_IMMED) arg += *(int*)(code + 1);
    if (cmd & ARG_REG)   arg += Stream->Regs[*(int*)(code + 1)];

    printf ("We got arg: %d\n", arg);

    StackPush (self, arg);    
}


void ProcCtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;

    self->Regs[0] = 100;  //For testing
    self->Regs[1] = 200;
    self->Regs[2] = 300;
    self->Regs[3] = 400;
}


void ProcDtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;   

    for (int i = 0; i < REG_AMOUNT; i++)
    {
        self->Regs[i] = -1;
    } 
}

