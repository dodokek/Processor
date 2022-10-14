
#include "../include/proc.h"

void StartProc ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");
    char* buffer = (char*) calloc (sizeof (int), MAX_BIN_SIZE); 
    fread (buffer, sizeof (int), MAX_CMDS_AMOUNT, CmdFile);

    Processor CpuInfo = {};
    ProcCtor (&CpuInfo);

    ParseBinFile (&CpuInfo, buffer); 
    if (CpuInfo.version != PROC_VERSION) printf ("Wrong bin file!");
    Execute (&CpuInfo);

    FREE(buffer);
    fclose (CmdFile);
    ProcDtor (&CpuInfo);
}


void ParseBinFile (Processor* self, char* code)
{
    self->version = code[VERSION_INDX];
    self->cmds_amount = *(int*)(code + CMD_AMT_INDX);
    if (code[SG_INDX1] != 'C' || 
        code[SG_INDX2] != 'U' || 
        code[SG_INDX3] != 'M') 
        printf ("Wrong signature!\n");

    printf ("Cur Version: %d, Curr Cmd Amount: %d \n\n", 
            code[VERSION_INDX], self->cmds_amount);

    self->cmds = code; 
}


void Execute (Processor* CpuInfo)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int ip = WORK_DATA_LEN; ip < CpuInfo->cmds_amount; ip++)
    {
        printf ("Ip %3d: ", ip);
        ProcessCommand (&MainStack, CpuInfo->cmds + ip, &ip, CpuInfo);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, const char* code, int* ip, Processor* CpuInfo)
{
    #define DEF_CMD(name, len, code) \
        case name:                   \
            code                     \
            break;              

    switch (*code & CMD_BITMASK)
    {
        #include "../include/codegen/cmds.h"

        default:
            printf ("SIGILL %d\n", *code);
            break;
    }

    #undef DEF_CMD
}


elem_t* GetArg (int cmd, const char* code, Processor* CpuInfo, elem_t* val)          
{
    // printf ("Rn we have command: %d\n", cmd & SPEC_BITMASK);

    // printf ("Lets get some args!\n");
    // printf ("Imm: %d, reg: %d\n", cmd & ARG_IMMED, cmd & ARG_REG);

    elem_t* arg_ptr = 0;
    int reg_indx = code[sizeof(elem_t) + BYTE_OFFSET];

    // printf ("Reg indx %d\n", reg_indx);

    if (cmd & ARG_IMMED) *val = *(elem_t*)(code + 1);
    
    if ((cmd & ARG_REG) && (cmd & ARG_IMMED))
    {
        *val += CpuInfo->Regs[reg_indx]; 
    }

    if ((cmd & ARG_REG) && !(cmd & ARG_IMMED))
    {
        // printf ("Working with register value \n");
        arg_ptr = CpuInfo->Regs + reg_indx; 
        *val    = CpuInfo->Regs[reg_indx];
    }

    if (cmd & ARG_MEM) arg_ptr = CpuInfo->Ram + int(*val);

    // printf ("arg ptr %p\n", arg_ptr);

    return arg_ptr;
}


void ProcCtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;

    memset (self->Regs, 0, sizeof(elem_t) * REG_AMOUNT);

    self->Ram = (elem_t*) calloc (RAM_SIZE, sizeof (elem_t));
}


void ProcDtor (Processor* self)
{
    self->version = 0;
    self->cmds_amount = 0;
    self->cmds = nullptr;   

    memset (self->Regs, -1, sizeof(elem_t) * REG_AMOUNT); 

    FREE(self->Ram);
}

