
#include "../include/asm.h"


void StartAsm()
{
    Text RawCmd = {};

    FILE* RawCmdFile = get_file ("../data/asm.txt", "r");
    FILE* CmdFile    = get_file ("../data/cmds.bin", "wb+");

    // process
    
    ProccessTextStruct (&RawCmd, RawCmdFile);

    RawToBin (RawCmd, CmdFile);

    TextDestr (&RawCmd);
    fclose (RawCmdFile);
    fclose (CmdFile);
}


void RawToBin (Text RawCmd, FILE* CmdFile)
{
    // AsmInfo ?
    // 

    Assembler AsmInfo = {};
    AsmInfoCtor (&AsmInfo, &RawCmd);

    // Processing every line

    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        AsmInfo.cur_len += LineToCommands (RawCmd.lines_array[line_ctr].begin_ptr, &AsmInfo);
    }

    //Filling stuff info

    FillWorkData (&AsmInfo);

    //Second lap to fill labels

    FillMissingLabels (&AsmInfo);

    // Destructors

    fwrite (AsmInfo.commands, sizeof (int), AsmInfo.cur_len, CmdFile);
    
    AsmInfoDtor (&AsmInfo);
}


void FillWorkData (Assembler* AsmInfo)
{
    AsmInfo->commands[VERSION_INDX] = VERSION;                 //VERSION TO COMPARE WITH CPU VERSION
    AsmInfo->commands[CMD_AMT_INDX] = AsmInfo->cur_len;
    AsmInfo->commands[SG_INDX1] = 'C';
    AsmInfo->commands[SG_INDX2] = 'U';
    AsmInfo->commands[SG_INDX3] = 'M';
}


int LineToCommands (char* line, Assembler* AsmInfo)
{
    printf ("Ip %d: ", AsmInfo->cur_len);
    
    int last_char_indx = 0;
    char tmp_line[MAX_CMD_LEN] = "";
    sscanf (line, "%s%n", tmp_line, &last_char_indx);

    if (line[last_char_indx - 1] == ':') 
    {
        line[last_char_indx - 1] = '\0';

        return ParseLabel (AsmInfo, line);   
    }


    // Cringe
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        return ParseCmd (AsmInfo, line + PUSH_LEN + 1, PUSH);
    }
    else if (strncmp ("POP", line, POP_LEN) == 0)
    {
        printf ("Need to parse pop\n");
        return ParseCmd (AsmInfo, line + POP_LEN + 1, POP);
    }
    else if (strncmp ("JMP", line, JMP_LEN) == 0)
    {
        return ParseJmp (AsmInfo, line + JMP_LEN, JMP);
    }
    else if (line[0] == 'J')
    {
        return IsJmp (AsmInfo, line);
    }
    else
    {
        AsmInfo->commands[AsmInfo->cur_len] = GetCmdNum (line);
        return DEFAULT_CMD_OFFSET;
    }
}


int ParseCmd (Assembler* AsmInfo, char* cur_cmd_line, int operation)
{
    printf ("Started to parse %s\n", cur_cmd_line);
    AsmInfo->commands[AsmInfo->cur_len] = operation;

    if (HandleRam (cur_cmd_line))
    {
        AsmInfo->commands[AsmInfo->cur_len] |= ARG_MEM;
        cur_cmd_line++;                 // skipping '[' symbol
    }

    int  tmp_imm = 0; 
    char tmp_reg[MAX_CMD_LEN] = "";

    if  (sscanf (cur_cmd_line, "%d + %s",  &tmp_imm,  tmp_reg)   == 2 || 
         sscanf (cur_cmd_line, " %[^+] + %d", tmp_reg, &tmp_imm  ) == 2) 
    {
        printf ("Oh, we got + sign \n");
        
        AsmInfo->commands[AsmInfo->cur_len] |= ARG_IMMED;
        AsmInfo->commands[AsmInfo->cur_len] |= ARG_REG;
        
        int reg_number = GetRegNum (tmp_reg);
        printf ("Register %d\n", reg_number);

        *(int*)(AsmInfo->commands + AsmInfo->cur_len + 1)  = tmp_imm;
                AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET] = reg_number;
    }

    else if (sscanf (cur_cmd_line, "%d", &tmp_imm))
    {
        AsmInfo->commands[AsmInfo->cur_len] |= ARG_IMMED;
        
        *(int*)(AsmInfo->commands + AsmInfo->cur_len + 1) = tmp_imm;
    }
    
    else    
    {

        AsmInfo->commands[AsmInfo->cur_len] |= ARG_REG;

        int reg_number = GetRegNum (cur_cmd_line);
        *(int*)(AsmInfo->commands + AsmInfo->cur_len + MULTI_BYTE_OFFSET) = reg_number;
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int IsJmp (Assembler* AsmInfo, char* line)
{
    // ??
    #define DEF_JMP(name, len) \
        if (strncmp (#name,  line, len) == 0) return ParseJmp (AsmInfo, line + len, name);  
    
    //-------
    #include "../include/jumps.h"
    //-------

    #undef DEF_JMP

    return -1;
}


int ParseJmp (Assembler* AsmInfo, char* cur_cmd_line, int jmp_type)
{
    printf ("Parsing jump with type %d:\n", jmp_type);

    char label_name[MAX_CMD_LEN] = "";
    
    sscanf (cur_cmd_line, "%s", label_name);

    printf (" Working with label %s\n", label_name);
    int label_indx = FindLabel (AsmInfo, HashLabel(label_name));


    if (label_indx != -1) 
    {
        *(int*)(AsmInfo->commands + AsmInfo->cur_len + 1) = AsmInfo->labels[label_indx].label_pos;
    }
    else
    { 
        *(int*)(AsmInfo->commands + AsmInfo->cur_len + 1) = HashLabel (label_name);
        AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET]  = 1;

        printf ("SOMETHING WRONG, filling byte %d in pos %d\n",
                AsmInfo->commands[AsmInfo->cur_len + MULTI_BYTE_OFFSET],
                AsmInfo->cur_len + MULTI_BYTE_OFFSET);

        printf ("Leaving hash func %d\n", *(int*)(AsmInfo->commands + AsmInfo->cur_len + 1));
    }

    AsmInfo->commands[AsmInfo->cur_len] = jmp_type;

    return DEFAULT_TWO_CMD_OFFSET;
}


int FindLabel (Assembler* AsmInfo, int label_hash)
{
    
    for (int i = 0; i < AsmInfo->labels_amount; i++)
    {
        if (label_hash == AsmInfo->labels[i].hash) 
        {
            return i;
        }
    }
    return -1;
}


int ParseLabel (Assembler* AsmInfo, char* line)
{
    printf ("Analysing label %s\n", line);

    AsmInfo->labels[AsmInfo->labels_amount].name = line;

    AsmInfo->labels[AsmInfo->labels_amount].label_pos = AsmInfo->cur_len;

    AsmInfo->labels[AsmInfo->labels_amount].hash = HashLabel (line);

    printf ("----\nCur Label has pos: %d\n And hash %d\n----\n", AsmInfo->labels[AsmInfo->labels_amount].label_pos, AsmInfo->labels[AsmInfo->labels_amount].hash);

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
        
        // MAX_LEN
        sscanf (cmd_line, "%s%n", tmp_str, &cmd_len);
        // ?
        *(cmd_line + cmd_len - 1) = ' ';

        return true;
    }

    return false;
}


void FillMissingLabels (Assembler* AsmInfo)
{
    // jmp_type
    for (int i = WORK_DATA_LEN; i < AsmInfo->cur_len; i++)
    {
        #define DEF_JMP(type, len)                                                  \
            if (AsmInfo->commands[i] == type)                                        \
            {                                                                       \
                printf ("Got unfilled jump %d NOW CHECKING BYTE %d\n", type, AsmInfo->commands[i + JMP_OFFSET]);   \
                if (AsmInfo->commands[i + JMP_OFFSET] == 0) continue;                \
            } else                                                   

        //-----
        #include "../include/jumps.h"
        {continue;}
        //-----

        #undef DEF_JMP      

        printf ("NEED TO FILL JUMP in %d\n", i);
        printf ("Got command %d\\n", AsmInfo->commands[i]);

        int* hashed_label = (int*)(AsmInfo->commands + i + 1);
        printf ("Need to replace hash %d\n", *hashed_label);
        int  label_indx   = FindLabel (AsmInfo, *hashed_label);
        printf ("Replacing with %d", AsmInfo->labels[label_indx].label_pos);
            *hashed_label = AsmInfo->labels[label_indx].label_pos; 
    }
}


void AsmInfoCtor (Assembler* AsmInfo, Text* RawCmd)
{
    AsmInfo->commands = (char*)  calloc (sizeof (int), RawCmd->lines_amount * 2);
    AsmInfo->labels =   (Label*) calloc (sizeof(Label), MAX_LABELS);
    // ??

    AsmInfo->cur_len = WORK_DATA_LEN;
    AsmInfo->labels_amount = 0;
}


void AsmInfoDtor (Assembler* AsmInfo)
{
    FREE(AsmInfo->commands);
    FREE(AsmInfo->labels);

    AsmInfo->cur_len = 0;
}


// ?
void IntToChar (char* arr, const int* num)
{
    char* ptr = (char*) num;

    // *(int*)arr = num; 
    
    for (size_t i = 0; i < sizeof (int); i++)
    {
        arr[i] = *ptr;
        // printf ("Cur value: %d", arr[i]);
        ptr++; 
    }
}


int GetRegNum (char* reg)
{
    // 
    printf("recieved chars %c and %c\n", reg[0], reg[2]);
    if (reg[0] == 'r' && reg[2] == 'x')
    {
        return reg[1] - 'a';
    }

    return 0;
}


int GetCmdNum (char* cmd)
{
    // ?
    #define DEF_CMD(name, num, code) \
    if (strcmp (cmd, #name) == 0) return num; \
    else

    //------
    #include "../include/cmds.h"
    {return 0;}
    //------

    #undef DEF_CMD
}


int HashLabel (char* label)
{
    int h = 0xFACFAC;

    printf ("Recieved label: %s\n", label);

    while (*label != '\0')
    {
        h = ((h + (*label)) * SALT) % (HASH_MOD);
        label++;
    }
    printf ("Got hash %d\n", h);

    return h;
}
