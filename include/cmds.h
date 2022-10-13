DEF_CMD(HLT, 0,
{
    printf ("End of commands\n");
    exit(0);
})

DEF_CMD(PUSH, 1,
{
    int value = 0;
    int* arg = GetArg (*code, code, CpuInfo, &value);
    // printf ("Got value %d", value);

    
    if (arg) StackPush (self, *arg);  
    else
    {
        StackPush (self, value);
    }

    (*ip) += MULTI_BYTE_OFFSET;
})


DEF_CMD(POP, 6,
{
    int value = 0;
    int* arg = GetArg (*code, code, CpuInfo, &value);
    
    printf ("Arg %d", *arg);

    if (arg) *arg = StackPop (self);  
    else
    {
        printf ("SIGIL\n");
    }

    printf ("Popping.\n");

    (*ip) += MULTI_BYTE_OFFSET;

})


DEF_CMD(MLT, 2,
{
    StackPush (self, StackPop (self) + StackPop (self));
})

DEF_CMD(ADD, 3,
{
    printf ("Adding\n");
    StackPush (self, StackPop(self) + StackPop(self));
})

DEF_CMD(SUB, 4,
{
    StackPush (self, -StackPop (self) + StackPop (self));
})

DEF_CMD(DIV, 5,
{
    StackPush (self, StackPop(self) / StackPop(self));
})



DEF_CMD(OUT, 7, 
{
    // StackDump (self);
    printf ("Stack out: %d\n", StackPop (self));
})

DEF_CMD(DMP, 8, {})

DEF_CMD(JMP, 9, 
{
    *ip = *(int*)(code + 1) - 1;
    printf ("Jumping to %d\n", *ip);
})

DEF_CMD(JB, 10,
{
    if (StackPop(self) < StackPop(self))
    {
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;   
    }
})

DEF_CMD(JBE, 11,
{
    if (StackPop(self) <= StackPop(self))
    {
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;   
    }
})

DEF_CMD(JA, 12, 
{
    if (StackPop(self) > StackPop(self))
    {
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    }
})

DEF_CMD(JAE, 13, 
{
    if (StackPop(self) >= StackPop(self))
        {
            *ip = *(int*)(code + 1) - 1;
            printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += MULTI_BYTE_OFFSET;
        }  
})

DEF_CMD(JE, 14, 
{
    if (StackPop(self) == StackPop(self))
    {
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    }   
})

DEF_CMD(JNE, 15, 
{
     if (StackPop(self) != StackPop(self))
    {
        *ip = *(int*)(code + 1) - 1;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    } 
})

DEF_CMD(IN, 16, {})
