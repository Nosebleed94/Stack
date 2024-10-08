#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

#include "stack.h"

int main ()
{
    struct stack_t stk = {};

    StackCtor (&stk, 2);

    StackPush (&stk, 100);
    StackPush (&stk, 200);
    StackPush (&stk, 300);
    StackPush (&stk, 400);

    Stack_Elem_t pickupelem = 0;

    StackPop (&stk, &pickupelem);
    StackPop (&stk, &pickupelem);
    StackPop (&stk, &pickupelem);
    StackPop (&stk, &pickupelem);
    
    return 0;
}