
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
        return RecognizeJmp (Stream, line);
    }
    else if (line[0] == ':')
    {
        return ParseLabel (Stream, line);
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
        // printf ("TRIMMED STR IS %s \n\n", cur_cmd_line);
    }   

    int tmp_dig   = 0;

    if (sscanf (cur_cmd_line, "%d", &tmp_dig))
    {
        Stream->commands[Stream->bin_size] |= ARG_IMMED;
        IntToChar (Stream->commands + Stream->bin_size + 1, &tmp_dig);
    }
    else
    {
        int reg_number = GetRegNum (cur_cmd_line);

        IntToChar(Stream->commands + Stream->bin_size + 1, &reg_number);
        Stream->commands[Stream->bin_size] |= ARG_REG;
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int RecognizeJmp (Assembler* Stream, char* line)
{
    #define DEF_JMP(name, len) \
        if (strncmp (#name,  line, len) == 0) return ParseJmp (Stream, line + len, name);  \
        else

    //-------
    #include "../include/jumps.h"
    {return 0;}
    //-------

    #undef DEF_JMP
}


int ParseJmp (Assembler* Stream, char* cur_cmd_line, int operation)
{
    printf ("Parsing jump:\n");

    int jump_link = 0;
    sscanf (cur_cmd_line, "%d", &jump_link);
    printf ("Jumplink is %d\n", jump_link);

    if (Stream->labels[jump_link] != 0) 
    {
        Stream->commands[Stream->bin_size] = operation;
        IntToChar (Stream->commands + Stream->bin_size + 1, (Stream->labels + jump_link));
    }
    else
    { 
        Stream->commands[Stream->bin_size] = (-1) * operation;
        IntToChar (Stream->commands + Stream->bin_size + 1, &jump_link);
    }

    return DEFAULT_TWO_CMD_OFFSET;
}


int ParseLabel (Assembler* Stream, char* line)
{
    int label_indx = 0;
    sscanf (line + 1, "%d", &label_indx); //+1 skips ':' sym

    Stream->labels[label_indx] = Stream->bin_size;

    printf ("Cur Label has val: %d\n", Stream->labels[label_indx]);

    return ZERO_OFFSET;
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


void FillMissingLabels (Assembler* Stream)
{
    for (int i = WORK_DATA_LEN; i < Stream->bin_size; i++)
    {
        #define DEF_JMP(name, len) \
            if (Stream->commands[i] == (-1) * name) Stream->commands[i] = name; \
            else

        //-----
        #include "../include/jumps.h"
        {continue;}
        //-----

        #undef DEF_JMP      

        printf ("Got command %d\n", Stream->commands[i]);

        int label_link = Stream->labels[*(int*)(Stream->commands + i + 1)]; 
        IntToChar (Stream->commands + i + 1, &label_link); 
    }
}


void StreamCtor (Assembler* Stream, Text* RawCmd)
{
    Stream->commands = (char*) calloc (sizeof (int), RawCmd->lines_amount * 2);
    Stream->labels = (int*)  calloc (sizeof(int), MAX_LABELS);
    Stream->bin_size = WORK_DATA_LEN;
}


void StreamDtor (Assembler* Stream)
{
    FREE(Stream->commands);
    FREE(Stream->labels);

    Stream->bin_size = 0;
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


int GetCmdNum (char* cmd)
{
    #define DEF_CMD(name, num, code) \
    if (strcmp (cmd, #name) == 0) return num; \
    else

    //------
    #include "../include/cmds.h"
    {return 0;}
    //------

    #undef DEF_CMD
}

