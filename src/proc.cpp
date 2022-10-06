
#include "../include/proc.h"

void StartProc ()
{
    FILE* CmdFile = fopen ("../data/cmds.bin", "rb");

    int* tmp = (int*) calloc (sizeof (int), MAX_BIN_SIZE);

    fread (tmp, sizeof (int), MAX_CMDS_AMOUNT, CmdFile);

    BinData MainData = {};

    ParseBin (&MainData, tmp); 

    if (MainData.version != PROC_VERSION || MainData.password != 0xDEAD) 
    {
        printf ("Wrong bin file!");
    }

    Execute (MainData.cmds, MainData.cmds_amount);

    FREE(tmp);
}


void ParseBin (BinData* self, int* arr)
{
    self->version = arr[0];
    self->password = arr[1];
    self->cmds_amount = arr[2];

    self->cmds = arr + 3;
    
}


void Execute (int* arr, int len)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int i = 0; i < len; i++)
    {
        ProcessCommand (&MainStack, arr);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, int* arr)
{
    switch (*arr)
    {
    case PUSH:
        StackPush (self, *(arr+1));
        break;

    case ADD:
        StackPush (self, StackPop (self) + StackPop (self));
        break;

    case SUB:
        StackPush (self, -StackPop (self) + StackPop (self));
        break;

    case DIV:
        StackPush (self, StackPop (self) / StackPop (self));
        break;

    case OUT:
        printf ("Stack result: %d\n", StackPop (self));
        break;

    case HLT:
        printf ("End of commands\n");
        break;

    default:
        break;
    }
}


int main ()
{
    StartProc();
}