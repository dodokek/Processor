
#include "../include/asm.h"

int LABELS[MAX_LABELS] = {};

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

    for (int i = WORK_DATA_LEN; i < bin_size; i++)
    {
        if (commands[i] == FILL_LABEL_FLAG)
        {
            commands[i] = JMP;
            int label_link = LABELS[*(int*)(commands + i + 1)]; 
            IntToChar (commands + i + 1, &label_link);    
        }
    }

    fwrite (commands, sizeof (int), bin_size, CmdFile);

    FREE(commands);
}


int LineToCommands (char* line, char* commands, int bin_size)
{
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        return ParseCmd (commands, bin_size, line + PUSH_LEN + 1, PUSH);
    }
    else if (strncmp ("POP", line, POP_LEN) == 0)
    {
        return ParseCmd (commands, bin_size, line + POP_LEN + 1, POP);
    }
    else if (strncmp ("JMP", line, JMP_LEN) == 0)
    {
        return ParseJmp (commands, bin_size, line + JMP_LEN, JMP);
    }
    else if (line[0] == ':')
    {
        return ParseLabel (line, bin_size);
    }
    else
    {
        commands[bin_size] = GetCmdNum (line);

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
    else if (strcmp (cmd, "IN")  == 0) return IN;
    else if (strcmp (cmd, "JMP") == 0) return JMP;

    return 0;
}


int ParseCmd (char* commands, int cmd_iter, char* cur_cmd_line, int operation)
{
    commands[cmd_iter] = operation;

    if (HandleRam (cur_cmd_line))
    {
        commands[cmd_iter] |= ARG_MEM;
        cur_cmd_line++;
        printf ("TRIMMED STR IS %s \n\n", cur_cmd_line);

    }

    int tmp_dig   = 0;

    if (sscanf (cur_cmd_line, "%d", &tmp_dig))
    {
        commands[cmd_iter] |= ARG_IMMED;
        IntToChar (commands + cmd_iter + 1, &tmp_dig);
    }
    else
    {
        int reg_number = GetRegNum (cur_cmd_line);

        IntToChar(commands + cmd_iter + 1, &reg_number);
        commands[cmd_iter] |= ARG_REG;
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int ParseJmp (char* commands, int cmd_iter, char* cur_cmd_line, int operation)
{

    printf ("Parsing jump:\n");

    int jump_link = 0;
    sscanf (cur_cmd_line, "%d", &jump_link);
    printf ("Jumplink is %d\n", jump_link);

    if (LABELS[jump_link] != 0) 
    {
        commands[cmd_iter] = operation;
        IntToChar (commands + cmd_iter + 1, (LABELS + jump_link));
    }
    else
    { 
        commands[cmd_iter] = FILL_LABEL_FLAG;
        IntToChar (commands + cmd_iter + 1, &jump_link);
    }



    return DEFAULT_TWO_CMD_OFFSET;
}


int ParseLabel (char* line, int bin_size)
{
    int label_indx = 0;
    sscanf (line + 1, "%d", &label_indx);

    LABELS[label_indx] = bin_size;
    printf ("Cur Label has val: %d\n", LABELS[label_indx]);

    return 0;
}


bool HandleRam (char* cmd_line)
{
    if (*cmd_line == '[')
    {
        *cmd_line = ' ';

        char tmp_str[] = "";
        int  cmd_len = 0;
        
        sscanf (cmd_line, "%s%n", tmp_str, &cmd_len);
        *(cmd_line + cmd_len - 1) = ' ';

        return true;
    }
    return false;
}


void IntToChar (char* arr, const int* num)
{
    char* ptr = (char*) num;
    
    for (size_t i = 0; i < sizeof (int); i++)
    {
        arr[i] = *ptr;
        // printf ("Cur value: %d", arr[i]);
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

    printf("recieved chars %c and %c, WA\n", reg[0], reg[2]);
    return 0;
    
}
