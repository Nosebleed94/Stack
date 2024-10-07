#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdint.h>

const long long poison = 333; 
const size_t petux = 1234324;
const size_t scale_factor = 2;
const size_t down_factor = 4;

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

enum stack_error StackCtor (struct stack_t* stk, size_t size_capacity);
enum stack_error StackPush (struct stack_t* stk, Stack_Elem_t value);
enum stack_error StackPop  (struct stack_t* stk, Stack_Elem_t* pickupelem);
void StackDumb (struct stack_t* stk);
void StackRealloc (struct stack_t* stk);
enum stack_error StackAssert (struct stack_t* stk, enum stack_error error, long line, const char* func, const char* file);
enum stack_error StackEmpty (struct stack_t* stk, long line);
enum stack_error StackProverkaPetuxov (struct stack_t* stk, long line, const char* func, const char* file);
uint64_t HashFunc (char* buffer, size_t size);
enum stack_error CheckHash (char* buffer, enum stack_error error, uint64_t sum, long line, size_t size, struct stack_t* stk);

#endif // FUNCTION_