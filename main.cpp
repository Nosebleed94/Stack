#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum stack_error 
{
    ok               = -1,
    error_ctor       =  0,
    error_push       =  1,
    error_pop        =  2,
    error_assert     =  3,
    error_empty      =  4,
    error_petux      =  5,
    error_hashfunc   =  6,
};

typedef double Stack_Elem_t;

struct stack_t 
{
    Stack_Elem_t* storage;
    long long size;
    size_t capacity;
};

enum stack_error StackCtor (struct stack_t* stk, size_t size_capacity, const long long poison, size_t petux);
enum stack_error StackPush (struct stack_t* stk, Stack_Elem_t value, const long long poison, size_t petux);
enum stack_error StackPop  (struct stack_t* stk, Stack_Elem_t* x, const long long poison);
void StackDumb(struct stack_t* stk);
void StackRealloc (struct stack_t* stk);
enum stack_error StackAssert (struct stack_t* stk, enum stack_error error, long line);
enum stack_error StackEmpty (struct stack_t* stk, const long long poison, long line);
enum stack_error StackProverkaPetuxov (struct stack_t* stk, size_t petux, long line);
Stack_Elem_t HashFunc (struct stack_t* stk);
enum stack_error CheckHash (struct stack_t* stk, enum stack_error error, Stack_Elem_t sum, long line);

int main()
{
    const long long poison = 333;
    size_t petux = 12343245654;
    struct stack_t stk = {};

    StackCtor (&stk, 2, poison, petux);

    StackPush (&stk, 100, poison, petux);
    StackPush (&stk, 200, poison, petux);
    StackPush (&stk, 300, poison, petux);
    StackPush (&stk, 400, poison, petux);
    
    Stack_Elem_t sum = HashFunc(&stk);

    // stk.storage[0] = 1;

    CheckHash (&stk, error_hashfunc, sum, __LINE__ );

    printf("HashSum = {%02.f}\n", sum);

    StackEmpty (&stk, poison, __LINE__);
    
    Stack_Elem_t x = 0;

    StackPop(&stk, &x, poison);
    StackPop(&stk, &x, poison);
    StackPop(&stk, &x, poison);
    StackPop(&stk, &x, poison);
    
    return 0;
}

enum stack_error StackCtor (stack_t* stk, size_t size_capacity, const long long poison, const size_t petux)
{   
    assert (stk != NULL);
    stk->capacity = size_capacity;
    stk->storage = (Stack_Elem_t*)calloc((stk->capacity) + 2, sizeof(Stack_Elem_t));
    stk->storage[0] = petux; 
    stk->size = 1;

    for (int i = 1; i <= (stk->capacity) + 1; i++)
    {
        stk->storage[i] = poison;
    }
    stk -> storage[(stk->capacity) + 1] = petux;
    
    // for (int i = 0; i < (stk->capacity) + 2; i++)
    // {
    //     printf("Ctor value = {%02.f}\n", stk->storage[i]);
    // }

    StackAssert(stk, error_ctor, __LINE__); 
    
    return ok;
}

enum stack_error StackPush (struct stack_t* stk, Stack_Elem_t value, const long long poison, size_t petux)
{
    StackAssert (stk, error_push, __LINE__);

    if (stk->size == stk->capacity)
    {
        stk -> storage[(stk->capacity) + 1] = poison;
        stk->capacity = 2 * (stk->capacity);
        StackRealloc (stk);
        stk->storage[(stk->capacity) + 1] = petux;
        for (int i = (stk->size) + 1; i < (stk->capacity) + 1; i++)
        {
            stk->storage[i] = poison;
        }
    }

    stk->storage[stk->size] = value;
    stk->size++; 

    // for (int i = 0; i < (stk->capacity) + 2; i++)
    // {
    //     printf("Ctor value = {%02.f}\n", stk->storage[i]);
    // }
    
    StackProverkaPetuxov(stk, petux,  __LINE__);

    StackAssert (stk, error_push, __LINE__);

    return ok;
}

enum stack_error StackPop (struct stack_t *stk, Stack_Elem_t *x, const long long poison)
{
    StackAssert (stk, error_push, __LINE__); 

    StackEmpty (stk, poison, __LINE__);

    stk->size--;
    *x = stk->storage[stk->size];

    printf("Elem = %0.2f\n", *x);
    stk->storage[stk->size] = poison;
    if (stk->size < (stk->capacity)/4)
    {
        stk->capacity = (stk->capacity)/4;
    } 

    if (StackAssert(stk, error_pop, __LINE__) > ok);
   
    return ok;
}


void StackDumb(struct stack_t* stk)
{
    printf("Information about error (Dump!)\n");
    printf("Capacity = %lu\n", stk->capacity);
    printf("Size = %lld\n", stk->size);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        printf("{%lu} Value = %f\n", i, stk->storage[i]);
    }

    printf("\n\n");
    return;
}

void StackRealloc (struct stack_t* stk)
{
    stk->storage = (Stack_Elem_t*) realloc(stk->storage, ((stk->capacity) + 2) * sizeof(Stack_Elem_t)); 
}

enum stack_error StackAssert (struct stack_t* stk, enum stack_error error, long line)
{
    int err = 0;
    if (!err && stk == NULL)          { err = 1; }
    if (!err && stk->storage == NULL) { err = 1; }
    if (!err && stk->size < 0)        { err = 1; }
    if (err == 0)
    {
        return ok;
    }
    
    printf("Code error is {%d}  is on line %d, in file %s\n", error, __LINE__, __FILE__);
    StackDumb(stk);
    return error;
}

enum stack_error StackEmpty (struct stack_t* stk, const long long poison, long line)
{
    if (stk->storage[1] == poison)
    {
        printf("Code error is {%d} from {%ld} line\n", error_empty, line);
        StackDumb(stk);
        assert (0);
    }
    return ok;
}

enum stack_error StackProverkaPetuxov (struct stack_t* stk, size_t petux, long line)
{
    if (stk->storage[0] != petux || stk->storage[(stk->capacity) + 1] != petux)
    {
        printf("Code error is {%d} from {%ld} line\n", error_petux, line);
        StackDumb(stk);
        assert (0);
    }
    return ok;
}

Stack_Elem_t HashFunc (struct stack_t* stk)
{
    Stack_Elem_t sum = 0;
    for (int j = 0; j < (stk->capacity) + 2; j++)
    {
        sum += stk->storage[j];
    }
    return sum;
}

enum stack_error CheckHash (struct stack_t* stk, enum stack_error error, Stack_Elem_t sum, long line)
{
    if (sum != HashFunc (stk))
    {
        printf("Code error is {%d} from {%ld} line\n", error_hashfunc, line);
        StackDumb(stk);
        assert (0);
    }
    return ok;
}
