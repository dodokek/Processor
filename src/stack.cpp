
#include "../include/stack.h"


void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line)
{
    ON_CANARY_PROTECTION(

        self->left_cock  = LEFT_COCK;
        self->right_cock = RIGHT_COCK;
    
    )

    self->data = (elem_t *) calloc (sizeof(elem_t), capacity);

    self->size = 0; 
    self->capacity = capacity;

    ON_HASH_PROTECTION(
        
        self->hash = 0;
        self->subhash = 0; 

    )

    StackInfoCtor (self, name, filename, funcname);          

    ON_HASH_PROTECTION(DoRehash (self));
}


void StackInfoCtor (Stack* self, const char* name, const char* filename, const char* funcname)
{
    self->stack_info.data_corrupted = false;
    self->stack_info.mother_func = funcname;
    self->stack_info.mother_file = filename;
    self->stack_info.name = name + 1;        // skips '&' symbol in the name 
}


elem_t StackPop (Stack* self)
{
    ON_DEBUG(

        Verificate (self);
        StackDump (self);

    )

    StackResize (self, DECREASE);

    elem_t tmp = self->data[self->size - 1];

    self->size--;

    ON_HASH_PROTECTION(DoRehash (self));

    return tmp;     
}


void StackPush (Stack* self, elem_t value)
{
    // printf ("Pushing elem: %lg\n", value);

    ON_DEBUG(

        Verificate (self);
        StackDump (self);

    )

    StackResize (self, INCREASE);

    self->data[self->size] = value;

    self->size++;

    ON_HASH_PROTECTION(DoRehash (self));
}


void StackResize (Stack* self, int mode)
{ 
    size_t elem_size = sizeof (elem_t);

    switch (mode)
    {
    case INCREASE:
        if (self->capacity - self->size <= 1) 
        {   
            self->capacity *= MEMORY_MULTIPLIER;
        }
        break;
    
    case DECREASE:
        if ((self->capacity / MEMORY_MULTIPLIER) - self->size >= RESIZE_OFFSET)
        {
            self->capacity /= MEMORY_MULTIPLIER;
        }
        break;

    default:
        printf ("STACK RESIZE ERROR\n");
        return;
    }

    self->data = (elem_t*) recalloc (self->data, elem_size, self->capacity);
 
    ON_DEBUG( fill_array (self->data + self->size, self->data + self->capacity, POISON_NUM) );
    
    ON_HASH_PROTECTION( DoRehash (self) );
 
}


void* recalloc (void* ptr, size_t size_new, int amount)
{
    int len_old = _msize (ptr);
    int len_new = size_new * amount;
    
    if (len_old == len_new) return ptr;

    ptr = (void*) realloc (ptr, len_new);

    for (int i = len_old; i < len_new; i++)
    {
        ((char*) ptr)[i] = 0;
    }
    
    return ptr;
}


void StackDump_ (Stack* self, const char* filename, const char* funcname, int line)
{
    assert (self != nullptr && filename != nullptr && funcname != nullptr);

    PutDividers();
    /*Verificate (self);

    printf ("At file: %s\n", filename);

    printf ("Observing stack[%p] - %s, function: %s (Line %d)):\n", self, self->stack_info.name, funcname, line);
    printf ("%c Created at %s, file %s\n", 200, self->stack_info.mother_func, self->stack_info.mother_file);

    printf ("%cHash: %I64d \n", 204, self->hash);
    printf ("%cSubhash: %I64d \n", 204, self->subhash);
    
    ON_CANARY_PROTECTION
    (
        printf ("%cLeft canary: %x\n", 204, self->left_cock);
        printf ("%cRight canary: %x\n", 204, self->right_cock);
    )
    */

    printf ("Size: %d\n", self->size);
    printf ("Capacity: %d\nData array:\n", self->capacity);

    if (self->stack_info.data_corrupted) 
        printf ("\n************Stack was corrupted, stop cringe.************\n\n");

    for (int i = 0; i < self->capacity; i++)
    {
        if (i < self->size) 
        {
            printf ("\t*[%d]: %lg\n", i, self->data[i]);         
        }
        else
        {
            printf ("\t [%d]: %lg\n", i, self->data[i]);
        }
    }

    PutDividers();
}


intmax_t StackVerificator (Stack *self)
{
    assert (self != nullptr);

    intmax_t err = 0;
    
    printf ("---------------------Verifying stack: %p---------------------\n", self);

    if (self == nullptr)
    {
        err |= NULL_STACK;
        return err;
    }
    if (self->data == nullptr)
    {
        err |= NULL_DATA; 
        return err;
    }   
    if (self->size     <= 0)          err |= INVALID_SIZE;
    if (self->capacity <  self->size) err |= N_ENOUGH_SIZE;
    if (self->capacity <= 0)          err |= INVALID_CAPACITY;

    ON_CANARY_PROTECTION
    (   

        if (self->left_cock != LEFT_COCK || self->right_cock != RIGHT_COCK)
        {
            err |= 32;
            self->stack_info.data_corrupted = true;
        }

    )

    ON_HASH_PROTECTION
    (

        Stack tmp = *self;
        tmp.hash = 0;
        tmp.subhash = 0;

        intmax_t hash = HashFunc (&tmp, sizeof (Stack));
        intmax_t subhash = HashFunc (self->data, sizeof (elem_t) * self->capacity);

        if (self->hash != hash)
        {
            err += STACK_MEMORY_CORRUPTION;
            self->stack_info.data_corrupted = true;        
        }

        if (self->subhash != subhash)
        {
            err += STACK_DATA_CORRUPTION;
            self->stack_info.data_corrupted = true;        
        }

    )

    return err; 
}


void Verificate (Stack* self)
{
    assert (self != nullptr);

    intmax_t err = StackVerificator (self);

    printf ("Error code = %I64d\n", err);
    PutErrCodes (err);

    if (err == 0) printf ("ok\n");
}


void PutErrCodes (intmax_t err)
{
    for (int i = 0; i <= ERRORS_COUNT; i++)
    {
        int cur_bit = GetBit(err, i);

        if (cur_bit)
        {
            PrintError (1 << i);
        }
    }
}


int GetBit (intmax_t n, int pos)
{
    int mask = 1 << pos;
    int masked_n = n & mask;
    int thebit = masked_n >> pos;

    return thebit;
}


void PrintError (int error_code)
{
    switch (error_code)
    {
    case NULL_STACK:
        printf ("NULL STACK POINTER\n");
        break;

    case NULL_DATA:
        printf ("NULL DATA POINTER\n");
        break;

    case INVALID_SIZE:
        printf ("INVALID SIZE, below zero probably.\n");
        break;

    case N_ENOUGH_SIZE:
        printf ("N ENOUGH SIZE, Not enought size was located.\n");
        break;

    case INVALID_CAPACITY:
        printf ("INVALID CAPACITY\n");
        break;

    case DATA_ACCESS_VIOLATION:
        printf ("DATA ARRAY WAS CHANGED WTIHOUT THE PERMISSION, HANDS OFF!\n");
        break;

    case STACK_MEMORY_CORRUPTION:
        printf ("MEMORY GIVEN FOR CURRENT STACK WAS CORRUPTED, further actions are unsafe.\n");
        break;

    case STACK_DATA_CORRUPTION:
        printf ("DATA MEMORY WAS CORRUPTED, further actions are unsafe");

    default:
        printf("Idk, man. Something spooky. Error code %d\n", error_code);
    };
}


void DoRehash (Stack* self)
{
    assert (self != nullptr);

    self->hash = self->subhash = 0;                                           
    self->hash = HashFunc (self, sizeof (Stack));                             
    self->subhash =  HashFunc (self->data, sizeof (elem_t) * self->capacity);    
}


intmax_t HashFunc (void* ptr, size_t size)
{
    assert (ptr != nullptr);

    intmax_t h = 0xFACFAC;

    char* cur_ptr = (char*) ptr;
    char* end_ptr = cur_ptr + size - 1;

    for (; cur_ptr < end_ptr; cur_ptr++)
    {
        h = ((h + (*cur_ptr)) * SALT) % (HASH_MOD);
    }

    return h;
}


elem_t min (elem_t elem1, elem_t elem2)
{
    return (elem1 < elem2) ? elem1 : elem2;
}


void fill_array (elem_t* cur_ptr, elem_t* end_ptr, elem_t filler)
{
    while (cur_ptr < end_ptr)
    {
        *cur_ptr = filler;
        cur_ptr++;
    }
}


void StackDtor (Stack* self)
{
    assert (self != nullptr);

    FREE(self->data);

    self->size = -1;
    self->capacity = -1;
    self->hash = 0;
    self->subhash = 0;

    ON_CANARY_PROTECTION
    (
        self->left_cock = 0;
        self->right_cock = 0;
    )

    StackInfo tmp = {}; // memset()
    self->stack_info = tmp;
}


bool is_valid (Stack *self)
{
    return self->stack_info.data_corrupted;
}


void PutDividers()
{
    putc ('\n', stdout);
    for (int i = 0; i < 50; i++)
    {
        putc (219, stdout);
    }
    putc ('\n', stdout);
}



