#ifndef STACK_H
#define STACK_H

//#define CANARY 1
//#define HASH 1
//#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"

struct StackInfo
{
    const char* name;
    const char* mother_func;
    const char* mother_file;
    bool  data_corrupted;
};


struct Stack
{
    #ifdef CANARY
    intmax_t left_cock;
    #endif

    StackInfo stack_info;

    int size;
    int capacity; 
    intmax_t hash;
    intmax_t subhash;  
    elem_t* data;

    #ifdef CANARY
    intmax_t right_cock;
    #endif 
};


#define StackDump(X ) StackDump_ (X, __FILE__, __PRETTY_FUNCTION__,  __LINE__) 
#define StackCtor(X, Y) StackCtor_ (X, Y, #X, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#ifdef HASH

    #define ON_HASH_PROTECTION(...)  __VA_ARGS__

#else

    #define ON_HASH_PROTECTION(...) ;

#endif

//-----------------------------------------

#ifdef CANARY

    #define ON_CANARY_PROTECTION(...)  __VA_ARGS__

#else 

    #define ON_CANARY_PROTECTION(...)  ;

#endif

//-----------------------------------------

#ifdef DEBUG

    #define ON_DEBUG(...)  __VA_ARGS__

#else
 
    #define ON_DEBUG(...) ;

#endif

//-----------------------------------------

#define FREE(X) (free(X), (X) = nullptr)

enum RESIZE_MODE
{
    INCREASE = 0,
    DECREASE
};


enum ERR_CODES 
{
    NULL_STACK = (1 << 1),
    NULL_DATA  = (1 << 2),
    INVALID_SIZE = (1 << 3),
    N_ENOUGH_SIZE = (1 << 4),
    INVALID_CAPACITY = (1 << 5),
    DATA_ACCESS_VIOLATION = (1 << 6),
    STACK_MEMORY_CORRUPTION = (1 << 7),
    STACK_DATA_CORRUPTION = (1 << 8)
    // Don't forget to change ERROS_COUNT const
};

//----------------------------------------------------

const int RESIZE_OFFSET = 2;

const int MEMORY_MULTIPLIER = 2;

const int ERRORS_COUNT = 8;

//----------------------------------------------------



void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line);

void StackInfoCtor (Stack* self, const char* name, const char* filename, const char* funcname);

void StackDtor (Stack* self);

intmax_t StackVerificator (Stack* self);

int GetBit (intmax_t n, int pos);

void PutErrCodes (intmax_t err);

void PrintError (int error_code);

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line);

void PutDividers ();

void Verificate (Stack* self);

elem_t StackPop (Stack* self);

void StackPush (Stack* self, elem_t value);

void StackResize (Stack* self, int mode);

void* recalloc (void* ptr, size_t size_new, int amount);

intmax_t HashFunc (void* ptr, size_t size);

void DoRehash (Stack* self);

elem_t min (elem_t elem1, elem_t elem2);

void fill_array (elem_t* cur_ptr, elem_t* end_ptr, elem_t filler);

bool is_valid (Stack *self);

#endif
