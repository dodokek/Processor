
#include "../include/asm.h"


int main()
{
    Text RawCommands = {};

    FILE* RawCmdFile = get_file ("../examples/quadratic.asm", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "wb+");

    HandleTextStruct (&RawCommands, RawCmdFile); // read the file with commands

    RawToBin (RawCommands, CmdFile);  // translate raw commands to binary file

    TextDestr (&RawCommands);
    fclose (RawCmdFile);
    fclose (CmdFile);
}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    Assembler AsmInfo = {0};
    AsmInfoCtor (&AsmInfo, &RawCmd);

    // Processing every line

    EachLineToBin (&RawCmd, &AsmInfo);

    // Second lap to fill labels

    CmdsToZero (&RawCmd, &AsmInfo);     
    EachLineToBin (&RawCmd, &AsmInfo);

    //Filling signatures and writing result in file

    WriteProgramHeader (&AsmInfo);
    fwrite (AsmInfo.commands, sizeof (elem_t), AsmInfo.cur_len, CmdFile);
    
    // Destructor
    AsmInfoDtor (&AsmInfo);
}


void EachLineToBin (Text* RawCmd, Assembler* AsmInfo)
{
    for (int line_ctr = 0; line_ctr < RawCmd->lines_amount; line_ctr++)
    {
        AsmInfo->cur_len += LineToCommands (RawCmd->lines_array[line_ctr].begin_ptr, AsmInfo);
    }    
}


void CmdsToZero (Text* RawCmd, Assembler* AsmInfo)
{
    AsmInfo->cur_len = SERVICE_DATA_LEN;
    memset (AsmInfo->commands, 0, RawCmd->lines_amount * 2);
} 


void WriteProgramHeader (Assembler* AsmInfo)
{
    AsmInfo->commands[SG_INDX1] = 'C';
    AsmInfo->commands[SG_INDX2] = 'U';
    AsmInfo->commands[SG_INDX3] = 'M';
    AsmInfo->commands[VERSION_INDX] = VERSION; //VERSION TO COMPARE WITH CPU VERSION
    
    *(int*)(AsmInfo->commands + CMD_AMT_INDX) = AsmInfo->cur_len;
}


int LineToCommands (char* line, Assembler* AsmInfo)
{
    if (*line == ';') return ZERO_OFFSET;  // Handling comments

    printf ("Ip %d commands %s: ", AsmInfo->cur_len, line);
    
    if (int label_len = IsLabel (line, AsmInfo)) 
    {
        InsertLabel (AsmInfo, line, label_len); 
        return ZERO_OFFSET;
    }

    // Generating command cases

    #define DEF_LINE(name, len, ...)                                 \
        if (strncmp (name, line, len) == 0) {return __VA_ARGS__ ;}   \
        else

    //----------------------------------------------------------------

    #include "../include/codegen/line_to_cmds.h"

    /*else*/
    {
        int CmdNum = GetCmdNum (line);
        AsmInfo->commands[AsmInfo->cur_len] = CmdNum;
        return DEFAULT_CMD_OFFSET;
    }

    //----------------------------------------------------------------

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
        return label_len; 
    }

    return 0;
} 


int InsertLabel (Assembler* AsmInfo, char* label, int label_len)
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
        }
    }

    strcpy (AsmInfo->labels[AsmInfo->labels_amount].name, label_copy);

    AsmInfo->labels[AsmInfo->labels_amount].label_pos = AsmInfo->cur_len;

    AsmInfo->labels_amount++;

    return ZERO_OFFSET;
}


int FindLabel (Assembler* AsmInfo, char* label_name)
{ 
    for (int label_indx = 0; label_indx < AsmInfo->labels_amount; label_indx++)
    {
        if (strcmp(label_name, AsmInfo->labels[label_indx].name) == 0) 
        {
            return label_indx;
        }
    }
    return -1;
}


int HandlePushPop (Assembler* AsmInfo, char* cur_cmd_line, int cmd_type)
{
    char *cmd_byte = AsmInfo->commands + AsmInfo->cur_len;
         *cmd_byte = cmd_type; // number of cmd Push or Pop

    char cmd_line_copy[MAX_CMD_LEN] = "";
    strcpy (cmd_line_copy, cur_cmd_line);
    if (HandleRam (cmd_line_copy)) *cmd_byte |= ARG_MEM;

    // Handle each case of argumen. digit, register, digit+register

    elem_t tmp_imm = 0; // value after the command   
    char tmp_reg[MAX_CMD_LEN] = "";

    if  (sscanf (cmd_line_copy, "%lg + %s",     &tmp_imm,  tmp_reg)   == 2 || 
         sscanf (cmd_line_copy, " %[^+ ] + %lg", tmp_reg, &tmp_imm  ) == 2) 
    {
        *cmd_byte |= ARG_IMMED;
        *cmd_byte |= ARG_REG;
        
        int reg_number = GetRegNum (tmp_reg);

        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1)  = tmp_imm;
                   AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET] = reg_number;
    }

    else if (sscanf (cmd_line_copy, "%lg", &tmp_imm))
    {
        *cmd_byte |= ARG_IMMED;
        
        *(elem_t*)(AsmInfo->commands + AsmInfo->cur_len + 1) = tmp_imm;
    }
    
    else    
    {
        *cmd_byte |= ARG_REG;

        int reg_number = GetRegNum (cmd_line_copy);
        AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET] = reg_number;
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int IsJmp (Assembler* AsmInfo, char* line)
{
    int cmd_len = 0;

    #define DEF_CMD(name, id, offset, code)  \
        cmd_len = strlen (#name);            \
        if (strncmp (#name,  line, cmd_len) == 0) return ParseJmp (AsmInfo, line + cmd_len, name);  
    
    //-----------------------------------------------------

    #include "../include/codegen/cmds.h"
    
    //-----------------------------------------------------

    #undef DEF_CMD

    return -1;
}


int ParseJmp (Assembler* AsmInfo, char* cur_cmd_line, int jmp_type)
{
    cur_cmd_line += BYTE_OFFSET; // Moving to label value

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


bool HandleRam (char* cmd_line)
{
    if (*cmd_line == '[')
    {
        *cmd_line = ' ';

        char tmp_str[10] = ""; 
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
    AsmInfo->labels =   (Label*) calloc (sizeof (Label), MAX_LABELS);
    assert (AsmInfo->commands && AsmInfo->labels);

    AsmInfo->cur_len = SERVICE_DATA_LEN;
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
    printf ("\nRecieved register %s\n\n", reg);
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
    #define DEF_CMD(name, num, offset, code) \
    if (strcmp (cmd, #name) == 0) return num; \
    else

    //------------------
    #include "../include/codegen/cmds.h"
    {
        printf ("=========SIGILL %d:==========\n", *cmd);
        return -1;
    }
    //------------------

    #undef DEF_CMD
}




