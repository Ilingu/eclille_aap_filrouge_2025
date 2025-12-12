#include <assert.h>
#include <stdlib.h>

#include "stack.h"

// Crée une nouvelle pile vide
// Utilisation : t_stack * ps = stack_new();
t_stack *stack_new() {
  t_stack *ps = malloc(sizeof(*ps));
  assert(ps != NULL);
  *ps = list_new();
  return ps;
}

// Affiche tout le contenu d'une pile (pour le débogage seulement)
void stack_show(t_stack *ps) { list_show(*ps); }

// Retourne 1 si la pile *ps est vide, 0 sinon
int stack_is_empty(t_stack *ps) { return list_is_empty(*ps); }

// Empile la valeur e en haut de la pile *ps
void stack_push(t_vertex e, t_stack *ps) { *ps = list_add_head(e, *ps); }

// Dépile et retourne la valeur en haut de la pile *ps
t_vertex stack_pop(t_stack *ps) {
  t_vertex res = stack_top(ps);
  *ps = list_remove_head(*ps);
  return res;
}

// Retourne la valeur en haut de la pile *ps sans la dépiler
t_vertex stack_top(t_stack *ps) {
  assert(!list_is_empty(*ps));
  return list_get_first_val(*ps);
}
