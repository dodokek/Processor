#ifndef ONEGIN_H
#define ONEGIN_H


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "fileUtils.h"
#include "stringUtils.h"

//-----------------------------------------------------------------------------

struct Line
{
    char* begin_ptr;
    int   length;
};

//-----------------------------------------------------------------------------

struct Text
{
    char *buffer;
    int  symbols_amount;
    int  lines_amount;
    Line *lines_array;
};

//-----------------------------------------------------------------------------

int separate_lines (Text *MainText);

void print_single_line (Line *cur_line);

void print_lines (Line lines_array[], int line_amount);

void trim_left (Text *MainText);

void trim_right (Line lines_array[], int lines_amount);

void write_result_in_file (Text *MainText, FILE* output_file);

void calloc_lines_array (Text *MainText);

int count_lines (char *buffer, int symbols_read);

void TextDestr(Text *self);
 
void HandleTextStruct (Text* MainText, FILE* input_file);

//-----------------------------------------------------------------------------

#endif


