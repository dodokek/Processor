
#define LABLE_POS int(*(elem_t*)(code + 1) - 1)


DEF_CMD(HLT, 0, ZERO_OFFSET, 
{
    printf ("End of commands\n");
    return 1;
})


DEF_CMD(PUSH, 1, MULTI_BYTE_OFFSET, 
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


DEF_CMD(POP, 6, MULTI_BYTE_OFFSET,
{
    elem_t value = 0;
    elem_t* arg = GetArg (*code, code, CpuInfo, &value);
    // printf ("Popping\n");    

    if (arg) *arg = StackPop (self);  
    else
    {
        printf ("=======SIGIL=======\n");
    }
    
    if (*code & ARG_MEM) DrawMemory (CpuInfo);

    (*ip) += MULTI_BYTE_OFFSET;

})


DEF_CMD(MUL, 2, ZERO_OFFSET,
{
    // printf ("Multiplying\n");
    StackPush (self, StackPop (self) * StackPop (self));
})

DEF_CMD(ADD, 3, ZERO_OFFSET,
{
    // printf ("Adding\n");
    StackPush (self, StackPop(self) + StackPop(self));
})

DEF_CMD(SUB, 4, ZERO_OFFSET,
{
    StackPush (self, StackPop (self) - StackPop (self));
})

DEF_CMD(DIV, 5, ZERO_OFFSET,
{
    StackPush (self, StackPop(self) / StackPop(self));
})

DEF_CMD(OUT, 7, ZERO_OFFSET,
{
    // StackDump (self);        
    printf ("------STACK OUT: %lg-------\n", StackPop (self));
})

DEF_CMD(DMP, 8, ZERO_OFFSET,
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

DEF_CMD(JMP, 9, MULTI_BYTE_OFFSET,
{
    *ip = LABLE_POS;
    // printf ("Jumping to %d\n", *ip);
})

DEF_CMD(JB, 10, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) < StackPop(self))
    {
        *ip = LABLE_POS;
        // printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;   
    }
})

DEF_CMD(JBE, 11, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) <= StackPop(self))
    {
        *ip = LABLE_POS;
        // printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;   
    }
})

DEF_CMD(JA, 12, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) > StackPop(self))
    {
        *ip = LABLE_POS;
        // printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    }
})

DEF_CMD(JAE, 13, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) >= StackPop(self))
        {
            *ip = LABLE_POS;
            // printf ("Jumping to %d\n", *ip);
        }
        else
        {
            *ip += MULTI_BYTE_OFFSET;
        }  
})

DEF_CMD(JE, 14, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) == StackPop(self))
    {
        *ip = LABLE_POS;
        // printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    }   
})

DEF_CMD(JNE, 15, MULTI_BYTE_OFFSET,
{
    if (StackPop(self) != StackPop(self))
    {
        *ip = LABLE_POS;
        // printf ("Jumping to %d\n", *ip);
    }
    else
    {
        *ip += MULTI_BYTE_OFFSET;
    } 
})

DEF_CMD(IN, 16, ZERO_OFFSET,
{
    double tmp_num;

    printf ("Enter the number: \n");
    scanf ("%lg", &tmp_num);

    StackPush (self, tmp_num);
})


DEF_CMD(CALL, 17, MULTI_BYTE_OFFSET,
{
    StackPush (&CpuInfo->CallStack, *ip + MULTI_BYTE_OFFSET);
    *ip = LABLE_POS;
    // printf ("Calling ip %d\n", *ip);
})


DEF_CMD(RET, 18, ZERO_OFFSET,
{
    *ip = StackPop(&CpuInfo->CallStack);
    // printf ("Returning to %d\n", *ip);
})

DEF_CMD(SQR, 19, ZERO_OFFSET,
{
    // printf ("Getting root\n");
    StackPush(self, sqrt(StackPop(self)));
})

DEF_CMD(ZEROSOL, 20, ZERO_OFFSET,
{
    printf ("----Equation has zero solutions-----\n");
})

DEF_CMD(MEOW, 21, ZERO_OFFSET,
{
    printf ("Meow meow\n");
})

DEF_CMD(INF_SOL, 22, ZERO_OFFSET,
{
    printf ("----Infinite amount of solutions------\n");
})

DEF_CMD(SIN, 23, ZERO_OFFSET,
{
    // printf ("===Calculating sinus\n");
    StackPush (self, sin(StackPop(self)));
})

DEF_CMD(COS, 25, ZERO_OFFSET, 
{
    // printf ("===Calculating cosinus\n");
    StackPush (self, cos(StackPop(self)));
})

DEF_CMD(ABS, 26, ZERO_OFFSET, 
{
    StackPush (self, abs(StackPop(self)));
})

DEF_CMD(ROUND, 27, ZERO_OFFSET, 
{
    StackPush (self, ceil(StackPop(self)));
})