
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

    self->cmds = code; //Служебная информация хз как перевести
    
}


void Execute (Processor* Stream)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int ip = WORK_DATA_LEN; ip < Stream->cmds_amount; ip++)
    {
        ProcessCommand (&MainStack, Stream->cmds + ip, &ip, Stream);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, const char* code, int* ip, Processor* Stream)
{

    printf ("----Currently working on %d-----\n", *code & CMD_BITMASK);
    printf ("Ip %4d: ", *ip);
    switch (*code & CMD_BITMASK)
    {
    case PUSH:
        ProcessPush (self, code, *ip, Stream);
        (*ip) += INT_OFFSET;
        // StackDump (self);
        break;

    case ADD:
        printf ("Adding\n");
        StackPush (self, StackPop(self) + StackPop(self));
        // StackDump (self);  
        break;

    case SUB:
        StackPush (self, -StackPop (self) + StackPop (self));
        break;

    case DIV:
        StackPush (self, StackPop(self) / StackPop(self));
        // StackDump (self);
        break;

    case POP:
        ProcessPop (self, code, *ip, Stream);
        printf ("Popping.\n");
        (*ip) += INT_OFFSET;
        break;

    case OUT:
        // StackDump (self);
        printf ("Stack out: %d\n", StackPop (self));
        break;

    case HLT:
        printf ("End of commands\n");
        break;

    case JMP:
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
        break;
    
    case JB:
        if (StackPop(self) < StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;   
        }
        break;

    case JBE:
        if (StackPop(self) <= StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;   
        }

        break;

    case JA:
        if (StackPop(self) > StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;
        }   

        break;

    case JAE:
        if (StackPop(self) >= StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;
        }   

        break;

    case JE:
        if (StackPop(self) == StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;
        }   

        break;
    
    case JNE:
        if (StackPop(self) != StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += INT_OFFSET;
        }   

        break;

    default:
        printf ("UK %d\n", *code);
        break;
    }
}


void ProcessPush (Stack* self, const char* code, int ip, Processor* Stream)
{
    int* arg = GetArg (*code, code, Stream);

    // printf ("We got arg: %d\n", *arg);

    StackPush (self, *arg);    
}


void ProcessPop (Stack* self, const char* code, int ip, Processor* Stream)
{
    int* arg = GetArg (*code, code, Stream);

    // printf ("We got arg: %d\n", *arg);

    *arg = StackPop (self);  
}


int* GetArg (int cmd, const char* code, Processor* Stream)
{
    // printf ("Rn we have command: %d\n", cmd & SPEC_BITMASK);

    int* arg = nullptr;
    
    if (cmd & ARG_IMMED) arg = (int*)(code + 1);
    if (cmd & ARG_REG)   arg = Stream->Regs + *(int*)(code + 1); 
    if (cmd & ARG_MEM)   arg = Stream->Ram + *arg;

    // printf ("arg or ind %d\n", *(int*)(code + 1));

    return arg;
}


void ProcCtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;

    memset (self->Regs, 0, sizeof(int) * REG_AMOUNT);

    self->Ram = (int*) calloc (RAM_SIZE, sizeof (int));

}


void ProcDtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;   

    memset (self->Regs, -1, sizeof(int) * REG_AMOUNT); 

    FREE(self->Ram);
}

