#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ll.h"

// Crée une nouvelle liste chaînée
// Utilisation : t_list l = list_new();
t_list list_new() { return NULL; }

// Ajoute un maillon avec la valeur e au début de la liste l
t_list list_add_head(t_vertex e, t_list l) {
  t_node *n = malloc(sizeof(t_node));
  assert(n != NULL);
  n->val = e;
  n->p_next = l;
  return n;
}

// Retourne 1 si la liste l est vide, 0 sinon
int list_is_empty(t_list l) { return l == NULL; }

// Affiche la liste l
void list_show(t_list l) {
  t_node *n = l;
  int i = 0;
  while (n != NULL) {
    printf("Maillon %d, valeur = %d\n", i++, n->val);
    n = n->p_next;
  }
}

// Retourne la valeur du premier maillon de la liste l
t_vertex list_get_first_val(t_list l) {
  assert(l != NULL);
  return l->val;
}

// Retire le premier maillon de la liste l et retourne la nouvelle tête
// Utilisation : l = list_remove_head(l);
t_list list_remove_head(t_list l) {
  t_list l2;
  assert(l != NULL);
  l2 = l->p_next;
  free(l);
  return l2;
}

// Libère la liste l de la mémoire (maillon par maillon)
// Utilisation : l = list_free(l);
t_list list_free(t_list l) {
  t_node *n = l, *nNext;
  while (n != NULL) {
    nNext = n->p_next;
    free(n);
    n = nNext;
  }
  return NULL;
}

// Retourne un curseur de liste pour parcourir l
// Utilisation : t_node * lc = list_cursor_new(l);
// Remarque : en fait, un curseur est aussi une liste chaînée !
t_node *list_cursor_new(t_list l) { return l; }

// Retourne 1 si la fin de la liste a été atteinte
// Remarque : la fin d'une liste (NULL) est la même chose qu'une liste vide !
int list_cursor_at_end(t_node *lc) { return list_is_empty(lc); }

// Retourne la valeur du maillon actuellement pointé par lc
// Remarque : ça revient en fait à prendre la valeur du premier maillon de lc !
int list_cursor_get_val(t_node *lc) { return list_get_first_val(lc); }

// Avance le curseur vers le maillon suivant
// Utilisation : lc = list_cursor_next(lc);
t_node *list_cursor_next(t_node *lc) {
  assert(lc != NULL);
  return lc->p_next;
}
