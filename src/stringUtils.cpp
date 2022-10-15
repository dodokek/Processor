
#include "../include/stringUtils.h"


void calloc_lines_array (Text *MainText)
{
    assert (MainText->buffer != nullptr);

    MainText->lines_array = (Line*) calloc (sizeof(Line), count_lines (MainText->buffer, MainText->symbols_amount));

    return;
}


int separate_lines (Text *MainText)
{
    assert (MainText->buffer != nullptr && MainText->lines_array != NULL && MainText->symbols_amount > 0);

    int lines_indx = 0, cur_len = 0;

    char *cur_ptr = MainText->buffer;

    char *end_ptr = cur_ptr + MainText->symbols_amount;

    for (; cur_ptr != end_ptr; cur_ptr++)
    {
        cur_len++;
                                                                                             //01234
        if (*cur_ptr == '\n')                                                                //abcd\ndef\n
        {                                                                                    //\n
            if (cur_len > 1) // ignore "\n"
            {
                MainText->lines_array[lines_indx].begin_ptr = cur_ptr - cur_len + 1;
                MainText->lines_array[lines_indx].length    = cur_len;
                *cur_ptr = '\0';
                lines_indx++;
            }

            cur_len = 0;
        }
    }

    MainText->lines_amount = lines_indx;

    return 1;
}


void HandleTextStruct (Text* MainText, FILE* input_file)
{
    read_file (input_file, MainText);

    calloc_lines_array (MainText);

    separate_lines (MainText);

    trim_left (MainText);    
}


int count_lines (char *buffer, int symbols_read)
{
    char *cur_ptr = buffer;
    char *end_ptr = cur_ptr + symbols_read;

    int line_counter = 0;

    for (; cur_ptr != end_ptr; cur_ptr++)
    {
        if (*cur_ptr == '\n') line_counter++;
        printf("%c", *cur_ptr);
    }

    return line_counter;
}


void write_result_in_file (Text *MainText, FILE* output_file)
{
    for (int i = 0; i < MainText->lines_amount; i++)
    {
        fputs (MainText->lines_array[i].begin_ptr, output_file);
        fputc ('\n', output_file);
    }
}


void print_lines (Line lines_array[], int lines_amount)
{

    assert (lines_array != NULL && lines_amount > 0);
    for (int i = 0; i < lines_amount; i++)
    {
        //printf("%.*s", lines_array[i].length, lines_array[i].begin_ptr);
        //printf ("Line length %d\n", lines_array[i].length);
        puts (lines_array[i].begin_ptr);

    }
}


void trim_left (Text *MainText)

{
    printf ("Lines amount: %d\n", MainText->lines_amount);

    for (int i = 0; i < (MainText->lines_amount); i++)
    {
        while (*(MainText->lines_array[i].begin_ptr) == ' ' || *(MainText->lines_array[i].begin_ptr) == 9)
        {
            MainText->lines_array[i].begin_ptr++;
        }
    }
}


void TextDestr (Text *self)
{
    free (self->buffer);
    free (self->lines_array);
}








