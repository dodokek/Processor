
#define LABLE_POS int(*(elem_t*)(code + 1) - 1)


DEF_CMD(HLT, 0,
{
    printf ("End of commands\n");
    exit(0);
})


DEF_CMD(PUSH, 1,
{
    elem_t value = 0;
    elem_t* arg = GetArg (*code, code, CpuInfo, &value);
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
    elem_t value = 0;
    elem_t* arg = GetArg (*code, code, CpuInfo, &value);
    printf ("Popping\n");    

    if (arg) *arg = StackPop (self);  
    else
    {
        printf ("=======SIGIL=======\n");
    }
    
    if (*code & ARG_MEM) DrawMemory (CpuInfo);

    (*ip) += MULTI_BYTE_OFFSET;

})


DEF_CMD(MUL, 2,
{
    printf ("Multiplying\n");
    StackPush (self, StackPop (self) * StackPop (self));
})

DEF_CMD(ADD, 3,
{
    printf ("Adding\n");
    StackPush (self, StackPop(self) + StackPop(self));
})

DEF_CMD(SUB, 4,
{
    StackPush (self, StackPop (self) - StackPop (self));
})

DEF_CMD(DIV, 5,
{
    StackPush (self, StackPop(self) / StackPop(self));
})

DEF_CMD(OUT, 7, 
{
    // StackDump (self);        
    printf ("------STACK OUT: %lg-------\n", StackPop (self));
})

DEF_CMD(DMP, 8, 
{
    printf ("\n ----- BEG OF DUMP ------ \n");

    

    StackDump(self);

    for (int i = 0; i < REG_AMOUNT; i++)
    {
        printf ("Register %d: %lg\n", i, CpuInfo->Regs[i]);
    }

    printf ("Commands: \n");

    for (int i = *ip - 5; i <= *ip + 15; i++)
    {
        printf ("%2x %d ", CpuInfo->cmds[i], i);
    }

    printf ("\n");

    for (int i = *ip - 5; i <= *ip + 20; i++)
    {
        printf ("%2c %d ", (i == *ip - 1) ? '^' : '~', i);
    }

    printf ("\nCUr ip is %d\n", *ip);

    printf ("\n\n ----- END OF DUMP ------ \n\n");
})

DEF_CMD(JMP, 9, 
{
    *ip = LABLE_POS;
    printf ("Jumping to %d\n", *ip);
})

DEF_CMD(JB, 10,
{
    if (StackPop(self) < StackPop(self))
    {
        *ip = LABLE_POS;
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
        *ip = LABLE_POS;
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
        *ip = LABLE_POS;
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
            *ip = LABLE_POS;
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
        *ip = LABLE_POS;
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
        *ip = LABLE_POS;
        printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    } 
})

DEF_CMD(IN, 16, 
{
    double tmp_num;

    printf ("Enter the number: \n");
    scanf ("%lg", &tmp_num);

    StackPush (self, tmp_num);
})


DEF_CMD(CALL, 17, 
{
    StackPush (&CpuInfo->CallStack, *ip + MULTI_BYTE_OFFSET);
    *ip = LABLE_POS;
    printf ("Calling ip %d\n", *ip);
})


DEF_CMD(RET, 18, 
{
    *ip = StackPop(&CpuInfo->CallStack);
    printf ("Returning to %d\n", *ip);
})

DEF_CMD(SQR, 19, 
{
    printf ("Getting root\n");
    StackPush(self, sqrt(StackPop(self)));
})

DEF_CMD(ZEROSOL, 20, 
{
    printf ("----Equation has zero solutions-----\n");
})

DEF_CMD(MEOW, 21, 
{
    printf ("Meow meow\n");
})

DEF_CMD(INF_SOL, 22,
{
    printf ("----Infinite amount of solutions------\n");
})

DEF_CMD(SIN, 23,
{
    StackPush (self, sin(StackPop(self)));
})

DEF_CMD(COS, 25,
{
    StackPush (self, cos(StackPop(self)));
})

DEF_CMD(ABS, 26, 
{
    StackPush (self, abs(StackPop(self)));
})

DEF_CMD(ROUND, 27, 
{
    StackPush (self, ceil(StackPop(self)));
})