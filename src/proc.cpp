
#include "../include/proc.h"

void StartProc ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");
    char* buffer = (char*) calloc (sizeof (int), MAX_BIN_SIZE); // ??
    fread (buffer, sizeof (int), MAX_CMDS_AMOUNT, CmdFile);

    // ??
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

    if (code[SG_INDX1] != 'C' || 
        code[SG_INDX2] != 'U' || 
        code[SG_INDX3] != 'M') 
        printf ("Wrong signature!\n");

    printf ("Cur Version: %d, Curr Cmd Amount: %d \n\n", 
            code[VERSION_INDX], code[CMD_AMT_INDX]);

    self->cmds = code; // Служебная информация хз как перевести
    
}


void Execute (Processor* Stream)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int ip = WORK_DATA_LEN; ip < Stream->cmds_amount; ip++)
    {
        printf ("Ip %3d: ", ip);
        ProcessCommand (&MainStack, Stream->cmds + ip, &ip, Stream);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, const char* code, int* ip, Processor* Stream)
{

    // printf ("----Currently working on %d-----\n", *code & CMD_BITMASK);
    // printf ("Ip %4d: ", *ip);

    #define DEF_CMD(name, len, code) \
        case name:                   \
            code                     \
            break;              

    switch (*code & CMD_BITMASK)
    {
        #include "../include/cmds.h"


        default:
            printf ("SIGILL %d\n", *code);
            break;
    }

    #undef DEF_CMD
}


void ProcessPush (Stack* self, const char* code, int ip, Processor* Stream)
{
    int* arg = GetArg (*code, code, Stream);

    // printf ("We got arg: %d\n", *arg);

    StackPush (self, *arg);    
}

// handle
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

