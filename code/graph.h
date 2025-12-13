#ifndef GRAPH_H
#define GRAPH_H

#include "ll.h"
#include "stack.h"

typedef int t_bool; // Booléen

// Graphe en matrice d’adjacence
typedef struct {
int size; // Taille: nombre de sommets du graphe
t_bool ** m; // Contenu
} t_graphm;

// Graphe en listes d’adjacence
typedef struct {
int size; // Taille: nombre de sommets du graphe
t_list * l; // Contenu
} t_graphl;

/* DANS LA SUITE:
- "graphm" se réfère au matrice d’adjacence
- "graphl" se réfère au listes d’adjacence
*/

// sig
t_graphm * new_graphm(int size);
t_graphl * new_graphl(int size);

t_graphm * graphm_from_file(char* file_path);
t_graphl * graphl_from_file(char* file_path);

void graphm_to_dot(t_graphm * gm, char* out_path);
void graphl_to_dot(t_graphl * gl, char* out_path);

void graphm_add_arc(t_graphm * gm, int from, int to);
void graphl_add_arc(t_graphl * gl, int from, int to);

// Aux
t_stack* graphm_find_path(t_graphm * gm, int from, int to, t_bool rec);
t_stack* graphl_find_path(t_graphl * gl, int from, int to, t_bool rec);

void graph_show_path(t_stack* path, char* out_path);

// DFS
t_bool graphm_find_path_rec(t_graphm * gm, int from, int to, t_bool* visisted, t_stack* path);
t_bool graphl_find_path_rec(t_graphl * gl, int from, int to, t_bool* visited, t_stack* path);

// BFS
t_stack* graphm_find_path_it(t_graphm * gm, int from, int to);
t_stack* graphl_find_path_it(t_graphl * gl, int from, int to);


void show_graphm(t_graphm * gm);
void show_graphl(t_graphl * gl);

int** graphm_search_cfc(t_graphm * gm);
int** graphl_search_cfc(t_graphl * gl);

// CFC terminal
int** graphm_search_cfct(t_graphm * gm);
int** graphl_search_cfct(t_graphl * gl);

void free_graphm(t_graphm* graph);
void free_graphl(t_graphl* graph);

#endif