#include "../include/disasm.h"

void StartDisasm()
{
    printf ("-------Disasemblying------\n\n");

    FILE* DisasFile  = get_file ("../data/disass.txt", "w");

    char* buffer = ReadCommandsFile();

    puts(buffer);

    Processor CpuInfo = {};
    ProcCtor (&CpuInfo);

    ReadHeader (&CpuInfo, buffer); 
    if (CpuInfo.version != PROC_VERSION) printf ("Wrong bin file!");
    

    for (int ip = SERVICE_DATA_LEN; ip < CpuInfo.cmds_amount; ip++)
    {
        fprintf (DisasFile, "Ip %d: %2d \n", ip, CpuInfo.cmds[ip]& CMD_BITMASK);

        if ((CpuInfo.cmds[ip] & CMD_BITMASK) == PUSH)
        {
            fprintf (DisasFile, "%s %lg %d\n", "PUSH", *(elem_t*)(CpuInfo.cmds + ip + BYTE_OFFSET), *(CpuInfo.cmds + ip + sizeof(elem_t) + BYTE_OFFSET));
            ip += MULTI_BYTE_OFFSET;
        }
        if ((CpuInfo.cmds[ip] & CMD_BITMASK) == POP)
        {
            fprintf (DisasFile, "%s %lg %d\n", "POP", *(elem_t*)(CpuInfo.cmds + ip + BYTE_OFFSET), *(CpuInfo.cmds + ip + sizeof(elem_t) + BYTE_OFFSET));
            ip += MULTI_BYTE_OFFSET;
        }
        if ((CpuInfo.cmds[ip] & CMD_BITMASK) == CALL)
        {
            fprintf (DisasFile, "%s %lg\n", "CALL", *(elem_t*)(CpuInfo.cmds + ip + BYTE_OFFSET));
            ip += MULTI_BYTE_OFFSET;
        }
    }


    FREE(buffer);
    fclose (DisasFile);
    ProcDtor (&CpuInfo);
}


void ReadHeader (Processor* CpuInfo, char* code)
{
    if (code[SG_INDX1] != 'C' || 
        code[SG_INDX2] != 'U' || 
        code[SG_INDX3] != 'M') 
        printf ("Wrong signature!\n");

    CpuInfo->version = code[VERSION_INDX];
    CpuInfo->cmds_amount = *(int*)(code + CMD_AMT_INDX);

    printf ("Cur Version: %d, Curr Cmd Amount: %d \n\n", 
            code[VERSION_INDX], CpuInfo->cmds_amount);

    CpuInfo->cmds = code; 
}


char* ReadCommandsFile ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");


    char* buffer = (char*) calloc (sizeof (elem_t), MAX_BIN_SIZE); 
    fread (buffer, sizeof (elem_t), MAX_CMDS_AMOUNT, CmdFile);
    fclose (CmdFile);

    return buffer;
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
    StartDisasm();
}