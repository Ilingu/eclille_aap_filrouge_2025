#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

#include "ll.h"
#include "stack.h"

#define MAX_READ_LINE 500

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

Note: dans certains fonction vous verrez dans les arguments un: "t_bool is_nom, char** map_nom".
C'est souvent parce que ce sont des fonctions d'affichage, et comme interieurement toute les graph sont en représentation nombre,
ceux en représentation mot, doivent, au moment de l'affichage être reconvertie en mot pour garder l'illusion.
En ce sens, l'arguement "is_nom" est inutile car "map_nom" en contient déjà l'information (map_nom == NULL est équivalent à is_nom==false),
cependant par facilité de lecture, choix a été fait de le garder. 
*/

/* HELPERS */
t_graphm * new_graphm(int size);
t_graphl * new_graphl(int size);

t_graphm * graphm_from_file(char* file_path);
t_graphl * graphl_from_file(char* file_path);

// returns a list of strings of the mapped words
char** convert_word_to_num(char* file_path, char *outpath, int *size);

void graphm_add_arc(t_graphm * gm, int from, int to);
void graphl_add_arc(t_graphl * gl, int from, int to);

t_graphm * copy_graphm(t_graphm * gm);
t_graphl * copy_graphl(t_graphl * gl);

t_graphm * transpose_graphm(t_graphm * gm);
t_graphl * transpose_graphl(t_graphl * gl);

/* CONVERT TO DOT */
void graphm_to_dot(t_graphm * gm, char* out_path, t_bool is_nom, char** map_nom);
void graphl_to_dot(t_graphl * gl, char* out_path, t_bool is_nom, char** map_nom);

/* FIND PATH */
// global
t_stack* graphm_find_path(t_graphm * gm, int from, int to, t_bool rec);
t_stack* graphl_find_path(t_graphl * gl, int from, int to, t_bool rec);

void graph_show_path(t_stack* path, char* out_path, t_bool is_nom, char** map_nom);

// DFS
t_bool graphm_find_path_rec(t_graphm * gm, int from, int to, t_bool* visisted, t_stack* path);
t_bool graphl_find_path_rec(t_graphl * gl, int from, int to, t_bool* visited, t_stack* path);

// BFS
t_stack* graphm_find_path_it(t_graphm * gm, int from, int to);
t_stack* graphl_find_path_it(t_graphl * gl, int from, int to);

void show_graphm(t_graphm * gm, t_bool is_nom, char** map_nom);
void show_graphl(t_graphl * gl, t_bool is_nom, char** map_nom);

/* CFC SEARCH */
// global
int** graphm_search_cfc(t_graphm * gm);
int** graphl_search_cfc(t_graphl * gl);

// step 1
void graphm_kosaraju_1_rec(t_graphm * gm, int x, int* marking, int* order, int* step);
void graphl_kosaraju_1_rec(t_graphl * gl, int x, int* marking, int* order, int* step);

// step 2
void graphm_kosaraju_2_rec(t_graphm * gm, int x, int* marking, int* curr_cfc);
void graphl_kosaraju_2_rec(t_graphl * gl, int x, int* marking, int* curr_cfc);

void show_cfc(int** cfc, int size, FILE *file, t_bool is_nom, char** map_nom);
void free_cfc(int** cfc, int size);

/* CFCT FILTER */
int** graphm_search_cfct(t_graphm * gm);
int** graphl_search_cfct(t_graphl * gl);

int** cfctm_filter(t_graphm * gm, int** cfc);
int** cfctl_filter(t_graphl * gl, int** cfc);

/* FREE GRAPH*/
void free_graphm(t_graphm* graph);
void free_graphl(t_graphl* graph);

#endif