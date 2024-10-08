#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "stack.h"

enum stack_error StackCtor (stack_t* stk, size_t size_capacity)
{   
    assert (stk != NULL);

    stk->capacity = size_capacity;
    stk->storage = (Stack_Elem_t*) calloc ((stk->capacity) + 2, sizeof(Stack_Elem_t));
    memcpy (stk->storage, &petux ,sizeof(petux)); 
    Stack_Elem_t* skip_petux = (Stack_Elem_t*)((char*)stk->storage + sizeof(petux));
    stk->storage = skip_petux;
    memset ((char*) stk->storage, poison, ((stk->capacity) * sizeof(Stack_Elem_t)));
    memcpy (stk->storage + stk->capacity, &petux, sizeof(petux));
    stk->size = 0;          
    memcpy (stk->storage +((stk->capacity)), &petux, sizeof(petux));

    stk->hashsum = HashFunc((char*)stk->storage, stk->size * sizeof (Stack_Elem_t));
    
    StackAssert (stk, error_push, __LINE__, __func__, __FILE__);
    
    return ok;
}

enum stack_error StackPush (struct stack_t* stk, Stack_Elem_t value)
{
    assert (stk != NULL);
    StackAssert (stk, error_push, __LINE__, __func__, __FILE__);

    if (stk->size == stk->capacity)
    {
        StackRealloc (stk);
    }

    stk->storage[stk->size] = value; 
    stk->size++; 

    stk->hashsum = HashFunc((char*)stk->storage, stk->size * sizeof (Stack_Elem_t));

    StackAssert (stk, error_push, __LINE__, __func__, __FILE__);

    return ok;
}

enum stack_error StackPop (struct stack_t *stk, Stack_Elem_t* pickupelem)
{
    assert (stk != NULL);

    StackAssert (stk, error_push, __LINE__, __func__, __FILE__); 

    StackEmpty (stk, __LINE__);

    stk->size--;
    memcpy (pickupelem, stk->storage + stk->size, sizeof(petux));
    fprintf (stderr, "Elem = %0.2f\n", *pickupelem);

    memcpy (stk->storage + stk->size, &poison, sizeof(poison));
    if (stk->size < (stk->capacity) / down_factor) 
    {
        stk->capacity = (stk->capacity) / down_factor;
        memcpy (stk->storage +((stk->capacity)), &petux, sizeof(petux));
    } 

    stk->hashsum = HashFunc((char*)stk->storage, stk->size * sizeof (Stack_Elem_t));

    StackAssert (stk, error_push, __LINE__, __func__, __FILE__);
   
    return ok;
}

void StackDumb (struct stack_t* stk)
{ 
    assert(stk != NULL);

    fprintf (stderr, "--------------\n");
    fprintf (stderr,"Information about error (Dump!)\n");
    fprintf (stderr,"Capacity = %lu\n", stk->capacity);
    fprintf (stderr,"Size = %lld\n", stk->size);
    fprintf (stderr,"hashsum = %lu\n", stk->hashsum);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        fprintf (stderr, "{%lu} Value = %f\n", i, stk->storage[i]);
    }
    fprintf (stderr, "--------------\n\n");
    return;
}

void StackRealloc (struct stack_t* stk) 
{
    assert (stk != NULL);

    stk->storage = (Stack_Elem_t*) realloc ((char*)stk->storage - sizeof(petux), ((stk->capacity) * scale_factor) * sizeof(Stack_Elem_t) + 2 * sizeof (petux)); 
    Stack_Elem_t* skip_petux = ((Stack_Elem_t*)((char*)stk->storage + sizeof(petux)));
    stk->storage = skip_petux;
    stk->capacity = scale_factor * (stk->capacity);  
    memset (stk->storage + stk->capacity / scale_factor, poison, (stk->capacity / scale_factor) * sizeof (Stack_Elem_t));
    memcpy (stk->storage + stk->capacity, &petux, sizeof(petux));
}

enum stack_error StackAssert (struct stack_t* stk, enum stack_error error, long line, const char* func, const char* file)
{
    assert (stk != NULL);

    CheckHash ((char*)stk->storage, error, line, stk->size *sizeof(Stack_Elem_t), stk);
    
    StackProverkaPetuxov (stk, __LINE__, __func__, __FILE__ ); 

    int err = 0;
    if (!err && stk == NULL)          { err = 1; }
    if (!err && stk->storage == NULL) { err = 1; }
    if (!err && stk->size < 0)        { err = 1; }
    if (err == 0)
    {
        return ok;
    }

    fprintf (stderr, "Code error is {%d}  is on line %d, in file %s\n", error, __LINE__, __FILE__);
    
    return error;
}

enum stack_error StackEmpty (struct stack_t* stk, long line)
{
    assert (stk != NULL);

    if (stk->size != 0) 
    { 
        return ok;
    }

    fprintf (stderr, "Code error is {%d} from {%ld} line\n", error_empty, line);

    assert (0);
}

enum stack_error StackProverkaPetuxov (struct stack_t* stk, long line, const char* func, const char* file)
{
    assert (stk != NULL);
    
    if ((memcmp ((char*)stk->storage - sizeof(petux), &petux, sizeof(petux)) != 0) || (memcmp (stk->storage + stk->capacity, &petux, sizeof(petux)) != 0))// X
    {
        fprintf (stderr, "Code error is {%d} from {%s} function in {%s:%ld} file\n", error_petux, func, file, line);
        assert (0);
    }
    
    return ok;
}

uint64_t HashFunc (char* buffer, size_t hashsize)
{ 
    assert(buffer != NULL); 
    uint64_t sum = 0;
    for (uint64_t j = 0; j < hashsize; j++)
    {
        sum += buffer[j];
    }
    return sum;
}

enum stack_error CheckHash (char* buffer, enum stack_error error, long line, size_t  hashsize, struct stack_t* stk)
{
    assert(buffer != NULL); 
    uint64_t sum = 0;

    for (uint64_t j = 0; j < hashsize; j++)
    {
        sum += buffer[j];
    }
    
    if (sum != stk->hashsum)
    {
        fprintf (stderr, "Code error is {%d} from {%ld} line\n", error_hashfunc, line);
        StackDumb (stk);
        assert (0);
    }
    return ok;
}