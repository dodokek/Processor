
#include "../include/asm.h"

void StartAsm()
{
    Text RawCmd = {};

    FILE* RawCmdFile = get_file ("../data/asm.txt", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "wb+");

    ProccessTextStruct (&RawCmd, RawCmdFile);

    RawToBin (RawCmd, CmdFile);

    TextDestr (&RawCmd);

    fclose (RawCmdFile);
    fclose (CmdFile);

}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    char* commands = (char*) calloc (sizeof (char) * 4, RawCmd.lines_amount * 2);

    int bin_size = 5;

    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        bin_size += LineToCommands (RawCmd.lines_array[line_ctr].begin_ptr, commands, bin_size);
    }

    commands[0] = VERSION;
    commands[1] = bin_size - 5;
    commands[2] = 'C';
    commands[3] = 'U';
    commands[4] = 'M';

    fwrite (commands, sizeof (int), bin_size, CmdFile);

    FREE(commands);
}


int LineToCommands (char* line, char* commands, int cmds_amount)
{
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        commands[cmds_amount] = PUSH;

        int tmp_cmd = atoi (line + PUSH_LEN);
        IntToChar (commands + cmds_amount + 1, &tmp_cmd);

        return 5;
    }
    else
    {
        commands[cmds_amount] = GetCmdNum (line);

        return 1;
    }
}


int GetCmdNum (char* cmd)
{
    if      (strcmp (cmd, "PUSH") == 0) return PUSH;
    else if (strcmp (cmd, "MLT") == 0) return MLT;
    else if (strcmp (cmd, "ADD") == 0) return ADD;
    else if (strcmp (cmd, "SUB") == 0) return SUB;
    else if (strcmp (cmd, "DIV") == 0) return DIV;
    else if (strcmp (cmd, "OUT") == 0) return OUT;
    else if (strcmp (cmd, "HLT") == 0) return HLT;
    else if (strcmp (cmd, "DMP") == 0) return DMP;
    else if (strcmp (cmd, "IN") == 0)  return IN;

    return 0;
}


void IntToChar (char* arr, int* num)
{
    char* ptr = (char*) num;
    
    for (size_t i = 0; i < sizeof (int); i++)
    {
        arr[i] = *ptr;
        printf ("Cur value: %d", arr[i]);
        ptr++; 
    }
}
