
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
    // Stream ?
    // 

    Assembler Stream = {};
    StreamCtor (&Stream, &RawCmd);

    // Processing every line

    for (int line_ctr = 0; line_ctr < RawCmd.lines_amount; line_ctr++)
    {
        Stream.bin_size += LineToCommands (RawCmd.lines_array[line_ctr].begin_ptr, &Stream);
    }

    //Filling stuff info

    FillWorkData (&Stream);

    //Second lap to fill labels

    FillMissingLabels (&Stream);

    // Destructors

    fwrite (Stream.commands, sizeof (int), Stream.bin_size, CmdFile);
    
    StreamDtor (&Stream);
}


void FillWorkData (Assembler* Stream)
{
    Stream->commands[VERSION_INDX] = VERSION;                 //VERSION TO COMPARE WITH CPU VERSION
    Stream->commands[CMD_AMT_INDX] = Stream->bin_size;
    Stream->commands[SG_INDX1] = 'C';
    Stream->commands[SG_INDX2] = 'U';
    Stream->commands[SG_INDX3] = 'M';
}


int LineToCommands (char* line, Assembler* Stream)
{
    
    int last_char_indx = 0;
    char tmp_line[MAX_CMD_LEN] = "";
    sscanf (line, "%s%n", tmp_line, &last_char_indx);

    if (line[last_char_indx - 1] == ':') 
    {
        line[last_char_indx - 1] = '\0';

        return ParseLabel (Stream, line);   
    }


    // Cringe
    if (strncmp ("PUSH", line, PUSH_LEN) == 0)
    {
        return ParseCmd (Stream, line + PUSH_LEN + 1, PUSH);
    }
    else if (strncmp ("POP", line, POP_LEN) == 0)
    {
        return ParseCmd (Stream, line + POP_LEN + 1, POP);
    }
    else if (strncmp ("JMP", line, JMP_LEN) == 0)
    {
        return ParseJmp (Stream, line + JMP_LEN, JMP);
    }
    else if (line[0] == 'J')
    {
        return IsJmp (Stream, line);
    }
    else
    {
        Stream->commands[Stream->bin_size] = GetCmdNum (line);

        return DEFAULT_CMD_OFFSET;
    }
}


int ParseCmd (Assembler* Stream, char* cur_cmd_line, int operation)
{
    Stream->commands[Stream->bin_size] = operation;

    if (HandleRam (cur_cmd_line))
    {
        Stream->commands[Stream->bin_size] |= ARG_MEM;
        cur_cmd_line++;                 // skipping '[' symbol
    }   

    int tmp_imm = 0; 
    if (sscanf (cur_cmd_line, "%d", &tmp_imm))
    {
        Stream->commands[Stream->bin_size] |= ARG_IMMED;
        IntToChar (Stream->commands + Stream->bin_size + 1, &tmp_imm);
    }
    else
    {
        int reg_number = GetRegNum (cur_cmd_line);

        IntToChar(Stream->commands + Stream->bin_size + 1, &reg_number);
        Stream->commands[Stream->bin_size] |= ARG_REG;
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int IsJmp (Assembler* Stream, char* line)
{
    // ??
    #define DEF_JMP(name, len) \
        if (strncmp (#name,  line, len) == 0) return ParseJmp (Stream, line + len, name);  
    
    //-------
    #include "../include/jumps.h"
    //-------

    #undef DEF_JMP

    return -1;
}


int ParseJmp (Assembler* Stream, char* cur_cmd_line, int jmp_type)
{
    printf ("Parsing jump with type %d:\n", jmp_type);

    char label_name[MAX_CMD_LEN] = "";
    
    sscanf (cur_cmd_line, "%s", label_name);

    printf (" Working with label %s\n", label_name);
    int label_indx = FindLabel (Stream, HashLabel(label_name));


    if (label_indx != -1) 
    {
        *(int*)(Stream->commands + Stream->bin_size + 1) = Stream->labels[label_indx].label_pos;
    }
    else
    { 
        *(int*)(Stream->commands + Stream->bin_size + 1) = HashLabel (label_name);
        Stream->commands[Stream->bin_size + BIG_OFFSET]  = 1;
        printf ("SOMETHING WRONG, filling byte %d in pos %d\n", Stream->commands[Stream->bin_size + BIG_OFFSET], Stream->bin_size + BIG_OFFSET);

        printf ("@@@@Leaving hash func %d\n", *(int*)(Stream->commands + Stream->bin_size + 1));
    }

    Stream->commands[Stream->bin_size] = jmp_type;

    return JMP_OFFSET;
}


int FindLabel (Assembler* Stream, int label_hash)
{
    
    for (int i = 0; i < Stream->labels_amount; i++)
    {
        if (label_hash == Stream->labels[i].hash) 
        {
            return i;
        }
    }
    return -1;
}


int ParseLabel (Assembler* Stream, char* line)
{
    printf ("Analysing label %s\n", line);

    Stream->labels[Stream->labels_amount].name = line;

    Stream->labels[Stream->labels_amount].label_pos = Stream->bin_size;

    Stream->labels[Stream->labels_amount].hash = HashLabel (line);

    printf ("----\nCur Label has pos: %d\n And hash %d\n----\n", Stream->labels[Stream->labels_amount].label_pos, Stream->labels[Stream->labels_amount].hash);

    Stream->labels_amount++;

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


void FillMissingLabels (Assembler* Stream)
{
    // jmp_type
    for (int i = WORK_DATA_LEN; i < Stream->bin_size; i++)
    {
        #define DEF_JMP(name, len)                                                  \
            if (Stream->commands[i] == name)                                        \
            {                                                                       \
                printf ("NOW CHECKING BYTE %d\n", Stream->commands[i + JMP_OFFSET]);   \
                if (Stream->commands[i + JMP_OFFSET] == 0) continue;                \
            } else                                                   

        //-----
        #include "../include/jumps.h"
        {continue;}
        //-----

        #undef DEF_JMP      

        printf ("NEED TO FILL JUMP in %d\n", i);

        printf ("Got command %d\\n", Stream->commands[i]);

        int* hashed_label = (int*)(Stream->commands + i + 1);
        printf ("Need to replace hash %d\n", *hashed_label);
        int  label_indx   = FindLabel (Stream, *hashed_label);
        printf ("Replacing with %d", Stream->labels[label_indx].label_pos);
            *hashed_label = Stream->labels[label_indx].label_pos; 
    }
}


void StreamCtor (Assembler* Stream, Text* RawCmd)
{
    Stream->commands = (char*)  calloc (sizeof (int), RawCmd->lines_amount * 2);
    Stream->labels =   (Label*) calloc (sizeof(Label), MAX_LABELS);
    // ??

    Stream->bin_size = WORK_DATA_LEN;
    Stream->labels_amount = 0;
}


void StreamDtor (Assembler* Stream)
{
    FREE(Stream->commands);
    FREE(Stream->labels);

    Stream->bin_size = 0;
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
    if (reg[0] == 'r' && reg[2] == 'x')
    {
        printf ("NUM %d\n", reg[1] - 'a');
        return reg[1] - 'a';
    }

    printf("recieved chars %c and %c, WA\n", reg[0], reg[2]);
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
