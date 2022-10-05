
#include "../include/asm.h"

int main()
{
    Text RawCmd = {};

    FILE* RawCmdFile = get_file ("../data/asm.txt", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "wb");

    ProccessTextStruct (&RawCmd, RawCmdFile);

    RawToBin (RawCmd, CmdFile);

    TextDestr (&RawCmd);

}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        LineToBin (RawCmd.lines_array[line_ctr].begin_ptr, CmdFile);
    }
}


void LineToBin (char* line, FILE* CmdFile)
{
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        fprintf (CmdFile, "%d %d ", PUSH, atoi (line + PUSH_LEN));
    }
    else
    {
        fprintf (CmdFile, "%d ", GetCmdNum (line));
    }
}


int GetCmdNum (char* cmd)
{
    if (strcmp (cmd, "PUSH")) return PUSH;
    else if (strcmp (cmd, "POP")) return POP;
    else if (strcmp (cmd, "SUB")) return SUB;
    else if (strcmp (cmd, "DIV")) return DIV;
    else if (strcmp (cmd, "OUT")) return OUT;
    else if (strcmp (cmd, "HLT")) return HLT;
    else if (strcmp (cmd, "DMP")) return DMP;
    else if (strcmp (cmd, "IN")) return IN;

    return 0;
}

