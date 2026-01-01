

#include "ll.h"
#include "stack.h"
#include "graph.h"

typedef int t_bool; // Booléen

typedef struct{
char* t
}mot;

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
- "graphmn" se réfère au matrice d’adjacence 
- "graphln" se réfère au listes d’adjacence
*/


t_graphm * new_graphmN(int size);
t_graphl * new_graphlN(int size);

t_graphm * graphm_from_fileN(char* file_path);
t_graphl * graphl_from_fileN(char* file_path);

void graphm_add_arcN(t_graphm * gm, int from, int to);
void graphl_add_arcN(t_graphl * gl, int from, int to);

t_graphm * copy_graphmN(t_graphm * gm);
t_graphl * copy_graphlN(t_graphl * gl);

t_graphm * transpose_graphmN(t_graphm * gm);
t_graphl * transpose_graphlN(t_graphl * gl);

/* CONVERT TO DOT */
void graphm_to_dotN(t_graphm * gm, char* out_path);
void graphl_to_dotN(t_graphl * gl, char* out_path);

/* FIND PATH */
// global
t_stack* graphm_find_pathN(t_graphm * gm, int from, int to, t_bool rec);
t_stack* graphl_find_pathN(t_graphl * gl, int from, int to, t_bool rec);

void graph_show_pathN(t_stack* path, char* out_path);

// DFS
t_bool graphm_find_path_recN(t_graphm * gm, int from, int to, t_bool* visisted, t_stack* path);
t_bool graphl_find_path_recN(t_graphl * gl, int from, int to, t_bool* visited, t_stack* path);

// BFS
t_stack* graphm_find_path_itN(t_graphm * gm, int from, int to);
t_stack* graphl_find_path_itN(t_graphl * gl, int from, int to);

void show_graphmN(t_graphm * gm);
void show_graphlN(t_graphl * gl);

/* CFC SEARCH */
// global
int** graphm_search_cfcN(t_graphm * gm);
int** graphl_search_cfcN(t_graphl * gl);

// step 1
void graphm_kosaraju_1_recN(t_graphm * gm, int x, int* marking, int* order, int* step);
void graphl_kosaraju_1_recN(t_graphl * gl, int x, int* marking, int* order, int* step);
