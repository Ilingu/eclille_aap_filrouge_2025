#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef int t_vertex;  // Sommet de graphe

// Maillon de liste chaînée
typedef struct node {
  t_vertex val;          // Valeur (sommet)
  struct node * p_next;  // Pointeur vers le maillon suivant
} t_node;
// Liste chaînée (de sommets)
typedef t_node * t_list;

// Sig
t_list list_new();
t_list list_add_head(t_vertex e, t_list l);
int list_is_empty(t_list l);
void list_show(t_list l);
t_vertex list_get_first_val(t_list l);
t_list list_remove_head(t_list l);
t_list list_free(t_list l);
t_node *list_cursor_new(t_list l);
int list_cursor_at_end(t_node *lc);
int list_cursor_get_val(t_node *lc);
t_node *list_cursor_next(t_node *lc);

#endif