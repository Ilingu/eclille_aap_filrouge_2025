#include <stdio.h>
#include <stdlib.h>

#include "graphn.h"
#include "graph.h"

FILE* changement(FILE* fch_nom){

    FILE* tkt = fopen(fch_nom, "r");
    FILE* temp = fopen(temp, "w+");
    char* a; // sert a r 
    fgets(a, 1, tkt);
    int nb_sommet = a[0] - '0'; 
    int k;
    for(k = 0; k<4; k++){ // on veut arriver au moment ou ya les mots
        fgets(a, 1, tkt);
    }
    while(*fgets(a, 1, tkt) == ' ' || *fgets(a, 1, tkt) == '\n'){
        fgets(a, 1, tkt);
    }
    char** t1 = malloc(sizeof(nb_sommet*(char*))); // pour mettre les mots clefs
      
    int i;
    for(i = 0; i<nb_sommet; i++){
    while(fgets(t1, 1, tkt) != ' '){
        fgets(t[i], 1, tkt);
        }
    }
    int i;
    char** t2 = malloc(sizeof(2*nb_sommet*(char*))); // pour l'ensemble des arcs version mot
    for(i = 0; i<nb_sommet; i++){
        char* mot_temp;
        while(*fgets(t2, 1, tkt) != ' ' || *fgets(a, 1, tkt) == '\n'){
            fgets(t2[i], 1, tkt);
        }
    }
    int* t_nb = malloc(sizeof(2*nb_sommet*(int))); // pour l'ensemble des arcs version nombre
    int m, n;
    for(m = 0; m< 2*nb_sommet; m++){
        for(n = 0; n< nb_sommet; n++){
          if(strcmp(t2[m], t1[n]) == 0){
            t_nb[m] = n;
          }
        }
      }
     //ecriture sur nv fichier en mode nombre
    fputc(nb_sommet, temp);
    fputc('\n \n \n', temp);
    int p;
    for(p = 0; p<nb_sommet; p++){
        fputc(t_nb[p], temp);
        fputc(' ', temp);
        fputc(t_nb[p+1], temp);
        fputc('\n', temp);
    }
    return temp;
}

t_graphm * new_graphmN(int size){
    return (new_graphm(size));
}
t_graphl * new_graphlN(int size){
    return(new_graphl(size));
}

t_graphm * graphm_from_fileN(char* file_path){
    
}
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

