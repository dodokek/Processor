
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
    char* commands = (char*) calloc (sizeof (int), RawCmd.lines_amount * 2);

    int bin_size = WORK_DATA_LEN;

    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        bin_size += LineToCommands (RawCmd.lines_array[line_ctr].begin_ptr, commands, bin_size);
    }

    commands[VERSION_INDX] = VERSION;
    commands[CMD_AMT_INDX] = bin_size - WORK_DATA_LEN;
    commands[SG_INDX1] = 'C';
    commands[SG_INDX2] = 'U';
    commands[SG_INDX3] = 'M';

    fwrite (commands, sizeof (int), bin_size, CmdFile);

    FREE(commands);
}


int LineToCommands (char* line, char* commands, int cmds_amount)
{
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        return ParsePush (commands, cmds_amount, line + PUSH_LEN, PUSH);
    }
    else
    {
        commands[cmds_amount] = GetCmdNum (line);

        return DEFAULT_CMD_OFFSET;
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
    else if (strcmp (cmd, "IN")  == 0)  return IN;

    return 0;
}


int ParsePush (char* commands, int cmd_iter, char* cur_cmd_line, int operation)
{
    commands[cmd_iter] = PUSH;
    int tmp_dig   = 0;

    if (sscanf (cur_cmd_line, "%d", &tmp_dig))
    {
        commands[cmd_iter] |= ARG_IMMED;
        IntToChar (commands + cmd_iter + 1, &tmp_dig);
    }
    else
    {
        int reg_number = GetRegNum (cur_cmd_line + 1);

        IntToChar(commands + cmd_iter + 1, &reg_number);
        commands[cmd_iter] |= ARG_REG;
    }

    return DEFAULT_PUSH_OFFSET;
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


int GetRegNum (char* reg)
{
    if (reg[0] == 'r' && reg[2] == 'x')
    {
        printf ("NUM %d\n", reg[1] - 'a');
        return reg[1] - 'a';
    }

    printf("recieved chars %c and %c, WA\n", reg[0], reg[1]);
    return 0;
    
}
