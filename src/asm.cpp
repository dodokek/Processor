
#include "../include/asm.h"

int main()
{
    Text RawCmd = {};

    FILE* RawCmdFile = get_file ("../data/asm.txt", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "ab    +");

    ProccessTextStruct (&RawCmd, RawCmdFile);

    RawToBin (RawCmd, CmdFile);

    TextDestr (&RawCmd);

}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    int* commands = (int*) calloc (sizeof (int), RawCmd.lines_amount * 2);

    int bin_size = 3;

    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        bin_size += LineToCommands (RawCmd.lines_array[line_ctr].begin_ptr, commands, bin_size);
    }

    commands[0] = VERSION;
    commands[1] = 0xDEAD;
    commands[2] = bin_size;

    fwrite (commands, sizeof(int), bin_size * sizeof (int), CmdFile);
}


int LineToCommands (char* line, int* commands, int cmds_amount)
{
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        commands[cmds_amount]     = PUSH;
        commands[cmds_amount + 1] = atoi (line + PUSH_LEN);

        return 2;
    }
    else
    {
        commands[cmds_amount] = GetCmdNum (line);

        return 1;
    }
}


int GetCmdNum (char* cmd)
{
    if (strcmp (cmd, "PUSH") == 0) return PUSH;
    else if (strcmp (cmd, "POP") == 0) return POP;
    else if (strcmp (cmd, "ADD") == 0) return ADD;
    else if (strcmp (cmd, "SUB") == 0) return SUB;
    else if (strcmp (cmd, "DIV") == 0) return DIV;
    else if (strcmp (cmd, "OUT") == 0) return OUT;
    else if (strcmp (cmd, "HLT") == 0) return HLT;
    else if (strcmp (cmd, "DMP") == 0) return DMP;
    else if (strcmp (cmd, "IN") == 0) return IN;

    return 0;
}

