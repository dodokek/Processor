
#include "../include/asm.h"


void StartAsm()
{
    Text RawCmd = {};

    FILE* RawCmdFile = get_file ("../data/commands.asm", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "wb+");

    HandleTextStruct (&RawCmd, RawCmdFile);

    RawToBin (RawCmd, CmdFile);

    TextDestr (&RawCmd);
    fclose (RawCmdFile);
    fclose (CmdFile);
}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    Assembler AsmInfo = {};
    AsmInfoCtor (&AsmInfo, &RawCmd);

    // Processing every line

    HandleEachLine (&RawCmd, &AsmInfo);

    // Second lap to fill labels

    PrepareForSecondLap (&RawCmd, &AsmInfo);
    HandleEachLine (&RawCmd, &AsmInfo);

    //Filling signatures and writing result in file

    FillWorkData (&AsmInfo);

    fwrite (AsmInfo.commands, sizeof (elem_t), AsmInfo.cur_len, CmdFile);
    
    // Destructor
    AsmInfoDtor (&AsmInfo);
}


void HandleEachLine (Text* RawCmd, Assembler* AsmInfo)
{
    for (int line_ctr = 0; line_ctr < RawCmd->lines_amount; line_ctr++)
    {
        AsmInfo->cur_len += LineToCommands (RawCmd->lines_array[line_ctr].begin_ptr, AsmInfo);
    }    
}


void PrepareForSecondLap (Text* RawCmd, Assembler* AsmInfo)
{
    AsmInfo->cur_len = WORK_DATA_LEN;
    memset (AsmInfo->commands, 0, RawCmd->lines_amount * 2);
} 


void FillWorkData (Assembler* AsmInfo)
{
    AsmInfo->commands[VERSION_INDX] = VERSION;                 //VERSION TO COMPARE WITH CPU VERSION
    AsmInfo->commands[SG_INDX1] = 'C';
    AsmInfo->commands[SG_INDX2] = 'U';
    AsmInfo->commands[SG_INDX3] = 'M';
    *(int*)(AsmInfo->commands +CMD_AMT_INDX) = AsmInfo->cur_len;
}


int LineToCommands (char* line, Assembler* AsmInfo)
{
    if (*line == '/') return ZERO_OFFSET;  // Handling comments

    printf ("Ip %d commands %s: ", AsmInfo->cur_len, line);
    
    if (IsLabel (line, AsmInfo)) return ZERO_OFFSET;  // Handling label

    // Generating command cases

    #define DEF_LINE(name, len, ...)                                 \
        if (strncmp (name, line, len) == 0) {return __VA_ARGS__ ;}   \
        else

    #include "../include/codegen/line_to_cmds.h"

    /*else*/
    {
        int CmdNum = GetCmdNum (line);
        AsmInfo->commands[AsmInfo->cur_len] = CmdNum;
        return DEFAULT_CMD_OFFSET;
    }

    #undef DEF_LINE

    return 0;
}


int IsLabel (char* line, Assembler* AsmInfo)
{
    int label_len = 0;
    char tmp_line[MAX_CMD_LEN] = "";
    sscanf (line, "%s%n", tmp_line, &label_len);


    if (line[label_len - 1] == ':') 
    {
        printf ("parsing label\n");

        ParseLabel (AsmInfo, line, label_len);  

        return 1; 
    }

    return 0;
} 


int ParseCmd (Assembler* AsmInfo, char* cur_cmd_line, int operation)
{
    printf ("Started to parse %s\n", cur_cmd_line);
    char* cmd_num = AsmInfo->commands + AsmInfo->cur_len;

    *cmd_num = operation;

    char cmd_line_copy[MAX_CMD_LEN] = "";
    strcpy (cmd_line_copy, cur_cmd_line);


    if (HandleRam (cmd_line_copy))
    {
        // printf ("handling ram\n");
        *cmd_num |= ARG_MEM;
    }

    int  tmp_imm = 0; 
    char tmp_reg[MAX_CMD_LEN] = "";

    if  (sscanf (cmd_line_copy, "%d + %s",  &tmp_imm,  tmp_reg)   == 2 || 
         sscanf (cmd_line_copy, " %[^+] + %d", tmp_reg, &tmp_imm  ) == 2) 
    {
        printf ("Oh, we got + sign \n");
        
        *cmd_num |= ARG_IMMED;
        *cmd_num |= ARG_REG;
        
        int reg_number = GetRegNum (tmp_reg);
        printf ("Register %d\n", reg_number);

        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1)  = tmp_imm;
                   AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET] = reg_number;
    }

    else if (sscanf (cmd_line_copy, "%d", &tmp_imm))
    {
        *cmd_num |= ARG_IMMED;
        
        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1) = tmp_imm;
    }
    
    else    
    {
        *cmd_num |= ARG_REG;

        int reg_number = GetRegNum (cmd_line_copy);
        AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET] = reg_number;
    }


    return DEFAULT_TWO_CMD_OFFSET;
}


int IsJmp (Assembler* AsmInfo, char* line)
{
    
    #define DEF_JMP(name, len) \
        if (strncmp (#name,  line, len) == 0) return ParseJmp (AsmInfo, line + len, name);  
    
    //-------
    #include "../include/codegen/jumps.h"
    //-------

    #undef DEF_JMP

    return -1;
}


int ParseJmp (Assembler* AsmInfo, char* cur_cmd_line, int jmp_type)
{
    cur_cmd_line++;
    printf ("Parsing jump with type %d:\n", jmp_type);
    printf ("Working with line %s \n", cur_cmd_line);

    char label_name[MAX_CMD_LEN] = "";
    
    sscanf (cur_cmd_line, "%s", label_name);

    printf (" Working with label %s\n", label_name);
    int label_indx = FindLabel (AsmInfo, label_name);

    if (label_indx != -1) 
    {
        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1) = AsmInfo->labels[label_indx].label_pos;
    }
    else
    { 
        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1) = -1;
    }

    AsmInfo->commands[AsmInfo->cur_len] = jmp_type;

    return DEFAULT_TWO_CMD_OFFSET;
}


int FindLabel (Assembler* AsmInfo, char* label_name)
{ 
    for (int i = 0; i < AsmInfo->labels_amount; i++)
    {
        if (strcmp(label_name, AsmInfo->labels[i].name) == 0) 
        {
            return i;
        }
    }
    return -1;
}

int ParseLabel (Assembler* AsmInfo, char* label, int label_len)
{
    printf ("Analysing label %s\n", label);

    char label_copy[MAX_CMD_LEN] = "";
    strcpy (label_copy, label);
    label_copy[label_len - 1] = '\0';

    for (int i = 0; i< AsmInfo->labels_amount; i++)
    {
        if (strcmp (label_copy, AsmInfo->labels[i].name) == 0) 
        {
            return ZERO_OFFSET;
            printf ("HAVE SEEN LABEL BEFORE\n");
        }
    }

    strcpy (AsmInfo->labels[AsmInfo->labels_amount].name, label_copy);

    AsmInfo->labels[AsmInfo->labels_amount].label_pos = AsmInfo->cur_len;

    AsmInfo->labels_amount++;

    return ZERO_OFFSET;
}


bool HandleRam (char* cmd_line)
{
    if (*cmd_line == '[')
    {
        *cmd_line = ' ';

        char tmp_str[10] = ""; // '\0'
        int  cmd_len = 0;
        
        sscanf (cmd_line, "%s%n", tmp_str, &cmd_len);
        *(cmd_line + cmd_len - 1) = ' ';

        return true;
    }

    return false;
}


void AsmInfoCtor (Assembler* AsmInfo, Text* RawCmd)
{
    AsmInfo->commands = (char*)  calloc (sizeof (elem_t) * 2, RawCmd->lines_amount * 2);
    AsmInfo->labels =   (Label*) calloc (sizeof(Label), MAX_LABELS);

    AsmInfo->cur_len = WORK_DATA_LEN;
    AsmInfo->labels_amount = 0;
}


void AsmInfoDtor (Assembler* AsmInfo)
{
    FREE(AsmInfo->commands);
    FREE(AsmInfo->labels);

    AsmInfo->cur_len = 0;
}


int GetRegNum (char* reg)
{
    printf("recieved chars %c and %c\n", reg[0], reg[2]);
    if (reg[0] == 'r' && reg[2] == 'x')
    {
        return reg[1] - 'a';
    }

    return 0;
}


int GetCmdNum (char* cmd)
{
    printf ("Getting number of line %s\n\n", cmd);
    #define DEF_CMD(name, num, code) \
    if (strcmp (cmd, #name) == 0) return num; \
    else

    //------------------
    #include "../include/codegen/cmds.h"
    {
        printf ("=========SIGILL %d:==========\n", cmd);
        return -1;
    }
    //------------------

    #undef DEF_CMD
}

