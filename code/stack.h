#ifndef STACK_H
#define STACK_H

#include "ll.h"

// Pile (de sommets)
typedef t_list t_stack;

// Sig
void stack_show(t_stack * ps);
t_stack * stack_new();
int stack_is_empty(t_stack * ps);
void stack_push(t_vertex e, t_stack * ps);
t_vertex stack_pop(t_stack * ps);
t_vertex stack_top(t_stack * ps);
t_stack* reverse_stack(t_stack * ps);
void free_stack(t_stack * ps);

#endif