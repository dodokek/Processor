
#include "../include/proc.h"

void StartProc ()
{
    FILE* CmdFile = get_file ("../data/cmds.bin", "rb");

    int* tmp = (int*) calloc (sizeof (int), MAX_BIN_SIZE);

    printf("Symbols read%d\n", fread (tmp, sizeof (int), MAX_CMDS_AMOUNT, CmdFile));

    for (int i = 0; i < MAX_BIN_SIZE; i++)
    {
        printf ("%d ", tmp[i]);
    }

    BinData MainData = {};

    ParseBinFile (&MainData, tmp); 

    if (MainData.version != PROC_VERSION || MainData.password != 0xDEAD) 
    {
        printf ("Wrong bin file!");
    }

    Execute (MainData.cmds, MainData.cmds_amount);

    FREE(tmp);
    fclose (CmdFile);
}


void ParseBinFile (BinData* self, int* arr)
{
    self->version = arr[0];
    self->password = arr[1];
    self->cmds_amount = arr[2];

    printf ("Cur Version: %d, Cur pass %d, Curr Cmd Amount: %d \n\n", arr[0], arr[1], arr[2]);

    for (int i = 0; i < arr[2]; i++)
    {
        printf ("%d\n", arr[i]);
    }

    self->cmds = arr + 3;
    
}


void Execute (int* arr, int len)
{
    Stack MainStack = {};
    StackCtor (&MainStack, 2); 

    for (int cmd_counter = 0; cmd_counter < len; cmd_counter++)
    {
        ProcessCommand (&MainStack, arr + cmd_counter, &cmd_counter);
    }

    StackDtor (&MainStack);
}


void ProcessCommand (Stack* self, int* arr, int* iterator)
{
    printf ("Currently working on %d\n", *arr);
    switch (*arr)
    {
    case PUSH:
        StackPush (self, *(arr+1));
        (*iterator)++;
        break;

    case ADD:
        StackPush (self, StackPop(self) + StackPop(self));
        break;

    case SUB:
        StackPush (self, -StackPop (self) + StackPop (self));
        break;

    case DIV:
        StackPush (self, StackPop(self) / StackPop(self));
        break;

    case OUT:
        printf ("\n------------------Stack result: %d-----------------\n\n", StackPop (self));
        break;

    case HLT:
        printf ("End of commands\n");
        break;

    default:
        printf ("UK %d\n", *arr);
        break;
    }
}
