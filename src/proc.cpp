
#include "../include/proc.h"


void StartProc ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");
    char* buffer = (char*) calloc (sizeof (elem_t), MAX_BIN_SIZE); 
    fread (buffer, sizeof (elem_t), MAX_CMDS_AMOUNT, CmdFile);

    Processor CpuInfo = {};
    ProcCtor (&CpuInfo);

    ParseBinFile (&CpuInfo, buffer); 
    if (CpuInfo.version != PROC_VERSION) printf ("Wrong bin file!");
    Execute (&CpuInfo);

    FREE(buffer);
    fclose (CmdFile);
    ProcDtor (&CpuInfo);
}


void ParseBinFile (Processor* CpuInfo, char* code)
{
    CpuInfo->version = code[VERSION_INDX];
    CpuInfo->cmds_amount = *(int*)(code + CMD_AMT_INDX);

    if (code[SG_INDX1] != 'C' || 
        code[SG_INDX2] != 'U' || 
        code[SG_INDX3] != 'M') 
        printf ("Wrong signature!\n");

    printf ("Cur Version: %d, Curr Cmd Amount: %d \n\n", 
            code[VERSION_INDX], CpuInfo->cmds_amount);

    CpuInfo->cmds = code; 
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
    elem_t* arg_ptr = 0;
    int reg_indx = code[sizeof(elem_t) + BYTE_OFFSET];

    if (cmd & ARG_IMMED) *val = *(elem_t*)(code + 1);
    
    if ((cmd & ARG_REG) && (cmd & ARG_IMMED))
    {
        *val += CpuInfo->Regs[reg_indx]; 
    }

    if ((cmd & ARG_REG) && !(cmd & ARG_IMMED))
    {
        arg_ptr = CpuInfo->Regs + reg_indx; 
        *val    = CpuInfo->Regs[reg_indx];
    }

    if (cmd & ARG_MEM) arg_ptr = CpuInfo->Ram + int(*val);

    return arg_ptr;
}


void DrawMemory (Processor* CpuInfo)
<<<<<<< HEAD
{
    elem_t* ram_ptr = CpuInfo->Ram;

    putchar ('\n');

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            if (ram_ptr[i*10+j] == 0)
            {
                putchar ('.');
            }
            else
            {
                putchar ('#');
            }
        }
        putchar ('\n');
    }
}


void ProcCtor (Processor* self)
=======
>>>>>>> fixing-bug
{
    elem_t* ram_ptr = CpuInfo->Ram;

    putchar ('\n');
    int lines = 30;
    int cols  = 70;

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (ram_ptr[i*cols+j] == 0)
            {
                putchar ('.');
            }
            else
            {
                putchar ('#');
            }
        }
        putchar ('\n');
    }
}


void ProcCtor (Processor* CpuInfo)
{
    CpuInfo->version = 0;
    CpuInfo->cmds_amount = 0;
    CpuInfo->cmds = nullptr;

    memset (CpuInfo->Regs, 0, sizeof(elem_t) * REG_AMOUNT);

    CpuInfo->Ram = (elem_t*) calloc (RAM_SIZE, sizeof (elem_t));

    CpuInfo->CallStack = {};
    StackCtor (&CpuInfo->CallStack, 2);
}


void ProcDtor (Processor* CpuInfo)
{
    CpuInfo->version = 0;
    CpuInfo->cmds_amount = 0;
    CpuInfo->cmds = nullptr;   

    memset (CpuInfo->Regs, -1, sizeof(elem_t) * REG_AMOUNT); 

    FREE(CpuInfo->Ram);
}


int main()
{
    StartProc();

    return 0;
}
