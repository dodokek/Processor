#include "../include/disasm.h"

void StartDisasm()
{
    printf ("-------Disasemblying------\n\n");

    FILE* CmdFile    = get_file ("../data/cmds.bin", "rb");
    FILE* DisasFile  = get_file ("../data/disass.txt", "w");

    char* buffer = (char*) calloc (sizeof (int), MAX_BIN_SIZE); 
    fread (buffer, sizeof (int), MAX_CMDS_AMOUNT, CmdFile);

    puts(buffer);

    Processor CpuInfo = {};
    ProcCtor (&CpuInfo);

    ReadHeader (&CpuInfo, buffer); 
    if (CpuInfo.version != PROC_VERSION) printf ("Wrong bin file!");
    
    for (int ip = WORK_DATA_LEN; ip < CpuInfo.cmds_amount; ip++)
    {
        printf ("Ip %d: %2d \n", ip, CpuInfo.cmds[ip]& CMD_BITMASK);
        if ((CpuInfo.cmds[ip] & CMD_BITMASK) == PUSH)
        {
            fprintf (DisasFile, "%s %d %d\n", "PUSH", *(int*)(CpuInfo.cmds + ip + BYTE_OFFSET), *(CpuInfo.cmds + ip + sizeof(int) + BYTE_OFFSET));
        }
        ip += sizeof(int) + BYTE_OFFSET;
    }


    FREE(buffer);
    fclose (CmdFile);
    fclose (DisasFile);
    ProcDtor (&CpuInfo);
}