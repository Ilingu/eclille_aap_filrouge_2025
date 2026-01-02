#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

#include "ll.h"
#include "stack.h"

#define MAX_READ_LINE 500

typedef int t_bool; // Type personnalisé pour les booléens (0 = Faux, 1 = Vrai)

/* STRUCTURES DE DONNÉES */

// 1. Graphe représenté par une Matrice d'adjacence
typedef struct {
  int size;      // Nombre de sommets du graphe
  t_bool ** m;   // Tableau à 2 dimensions. m[i][j] = 1 si arc de i vers j.
} t_graphm;

// 2. Graphe représenté par des Listes d'adjacence
typedef struct {
  int size;      // Nombre de sommets du graphe
  t_list * l;    // Tableau de listes. l[i] contient la liste des voisins de i.
} t_graphl;

/* NOTES SUR LE NOMMAGE ET L'AFFICHAGE :
   - "graphm" désigne les fonctions utilisant la matrice.
   - "graphl" désigne les fonctions utilisant les listes.

   Concernant les arguments "t_bool is_nom" et "char** map_nom" :
   Ces arguments apparaissent dans les fonctions d'affichage.
   En interne, tous les algorithmes (chemins, CFC) travaillent avec des nombres (0, 1, 2...).
   Si le graphe d'origine utilisait des MOTS ("start", "stop", ...), on les reconvertie
   au moment de l'affichage pour l'utilisateur.
   
   - map_nom : Tableau de correspondance (ex: map_nom[0] = "start").
   - is_nom  : Booléen indiquant si on doit utiliser ce tableau.
   (Note: is_nom est techniquement redondant car si map_nom != NULL, is_nom==1,
   mais il est conservé pour rendre le code plus lisible).
*/

/* --- FONCTIONS UTILITAIRES CORE --- */

// Création de graphes vides (allocation mémoire)
t_graphm * new_graphm(int size);
t_graphl * new_graphl(int size);

// Création d'un graphe en lisant un fichier texte
t_graphm * graphm_from_file(char* file_path);
t_graphl * graphl_from_file(char* file_path);

// Convertit un fichier avec des noms ("A", "B") en fichier temporaire avec des nombres ("0", "1")
// Retourne le tableau de correspondance (la "carte" des noms)
char** convert_word_to_num(char* file_path, char *outpath, int *size);

// Ajoute un arc entre deux sommets
void graphm_add_arc(t_graphm * gm, int from, int to);
void graphl_add_arc(t_graphl * gl, int from, int to);

// Crée une copie identique d'un graphe (pour ne pas modifier l'original)
t_graphm * copy_graphm(t_graphm * gm);
t_graphl * copy_graphl(t_graphl * gl);

// Calcul la transposée d'un graphe pour l'algorithme de Kosaraju (CFC)
t_graphm * transpose_graphm(t_graphm * gm);
t_graphl * transpose_graphl(t_graphl * gl);

/* --- EXPORT VISUEL (DOT) --- */
// Génère un fichier compatible avec GraphViz pour dessiner le graphe
void graphm_to_dot(t_graphm * gm, char* out_path, t_bool is_nom, char** map_nom);
void graphl_to_dot(t_graphl * gl, char* out_path, t_bool is_nom, char** map_nom);

/* --- RECHERCHE DE CHEMIN --- */

// Fonctions principales (appellent soit la version récursive, soit l'itérative)
t_stack* graphm_find_path(t_graphm * gm, int from, int to, t_bool rec);
t_stack* graphl_find_path(t_graphl * gl, int from, int to, t_bool rec);

// Affiche le chemin trouvé de manière lisible et dans le bonne ordre (A -> B -> C)
void graph_show_path(t_stack* path, char* out_path, t_bool is_nom, char** map_nom);

// Version Récursif de DFS (Parcours en Profondeur)
t_bool graphm_find_path_rec(t_graphm * gm, int from, int to, t_bool* visisted, t_stack* path);
t_bool graphl_find_path_rec(t_graphl * gl, int from, int to, t_bool* visited, t_stack* path);

// Version Itérative de DFS (utilise une pile)
t_stack* graphm_find_path_it(t_graphm * gm, int from, int to);
t_stack* graphl_find_path_it(t_graphl * gl, int from, int to);

// Affiche le contenu brut du graphe dans la console - utilisée pour le debbugage du code.
// CES FONCTIONS NE SONT PLUS UTILISÉES DANS LE CODE.
void show_graphm(t_graphm * gm, t_bool is_nom, char** map_nom);
void show_graphl(t_graphl * gl, t_bool is_nom, char** map_nom);

/* --- RECHERCHE DE COMPOSANTES FORTEMENT CONNEXES (CFC) via l'algorithme de Kosaraju --- */

// Fonction principale : orchestre l'algo et retourne la liste des CFC
int** graphm_search_cfc(t_graphm * gm);
int** graphl_search_cfc(t_graphl * gl);

// Étape 1 de Kosaraju : Parcours DFS pour noter l'ordre de fin de traitement
void graphm_kosaraju_1_rec(t_graphm * gm, int x, int* marking, int* order, int* step);
void graphl_kosaraju_1_rec(t_graphl * gl, int x, int* marking, int* order, int* step);

// Étape 2 de Kosaraju : Parcours sur le graphe transposé pour identifier les groupes
void graphm_kosaraju_2_rec(t_graphm * gm, int x, int* marking, int* curr_cfc);
void graphl_kosaraju_2_rec(t_graphl * gl, int x, int* marking, int* curr_cfc);

// Affiche les CFC trouvées
void show_cfc(int** cfc, int size, FILE *file, t_bool is_nom, char** map_nom);
// Libère la mémoire du tableau de CFC
void free_cfc(int** cfc, int size);

/* --- FILTRE CFC TERMINAL (CFCT) --- */
// Filtre les CFC pour ne garder que celles qui n'ont pas de sortie vers d'autres CFC

// Prend les CFC calculées et renvoie uniquement celles terminales
int** cfctm_filter(t_graphm * gm, int** cfc);
int** cfctl_filter(t_graphl * gl, int** cfc);

/* --- NETTOYAGE MÉMOIRE --- */
void free_graphm(t_graphm* graph);
void free_graphl(t_graphl* graph);

#endif