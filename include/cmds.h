DEF_CMD(HLT, 0,
{
    printf ("End of commands\n");

})

DEF_CMD(PUSH, 1,
{
    ProcessPush (self, code, *ip, Stream);
    (*ip) += INT_OFFSET;
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

DEF_CMD(POP, 6,
{
    ProcessPop (self, code, *ip, Stream);
    printf ("Popping.\n");
    (*ip) += INT_OFFSET;
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
        *ip += INT_OFFSET;   
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
        *ip += INT_OFFSET;   
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
        *ip += INT_OFFSET;
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
            *ip += INT_OFFSET;
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
        *ip += INT_OFFSET;
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
        *ip += INT_OFFSET;
    } 
})

DEF_CMD(IN, 16, {})
