#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "ll.h"
#include "stack.h"

/* --- CREATION DE GRAPHES VIDES --- */

// Crée un nouveau graphe sous forme de MATRICE d'adjacence
t_graphm *new_graphm(int size) {
  // Réservation mémoire pour la structure graphm
  t_graphm *p_g = malloc(sizeof(t_graphm));
  assert(p_g != NULL); // verifie que ça a bien été alloué
  p_g->size = size;

  // Réservation mémoire pour les lignes
  p_g->m = malloc(size * sizeof(t_bool *));
  assert(p_g->m != NULL);

  // Réservation mémoire pour les colonnes et init à 0
  for (int i = 0; i < size; i++) {
    p_g->m[i] = malloc(size * sizeof(t_bool));
    assert(p_g->m[i] != NULL);
    for (int j = 0; j < size; j++)
      p_g->m[i][j] = 0;
  }

  return p_g;
}

// Crée un nouveau graphe sous forme de LISTES d'adjacence
t_graphl *new_graphl(int size) {
  // Réservation mémoire pour la structure
  t_graphl *pg = malloc(sizeof(t_graphl));
  assert(pg != NULL);
  pg->size = size;

  // Réservation du tableau de listes (chaque case i contiendra la liste des voisins de i)
  pg->l = malloc(pg->size * sizeof(t_list));
  assert(pg->l != NULL);

  // Initialisation à NULL
  for (int i = 0; i < size; i++)
    pg->l[i] = NULL;
  return pg;
}

/* --- CHARGEMENT DEPUIS UN FICHIER --- */

// Lit un fichier texte pour remplir une MATRICE d'adjacence
t_graphm *graphm_from_file(char *file_path) {
  FILE *file;
  if (file_path == NULL) {
    printf("Pas de fichier spécifié, lecture depuis l'entrée standard (clavier).\n");
    file = stdin;
  } else
    file = fopen(file_path, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  // Lecture de la première ligne qui contient la taille du graphe (nombre de sommets)
  fgets(g_data, MAX_READ_LINE, file);

  int size = atoi(g_data); // Conversion texte -> entier
  t_graphm *gm = new_graphm(size);

  // Lecture des lignes suivantes (les arcs)
  while (fgets(g_data, MAX_READ_LINE, file)) {
    if (g_data[0] == '\n')
      continue; // Ignore les lignes vides

    // Buffers pour stocker les chaines de caractères avant et après l'espace
    char from_buf[MAX_READ_LINE] = {'\0'};
    char to_buf[MAX_READ_LINE] = {'\0'};
    t_bool before_to = 1; // est-on avant ou après l'espace ?
    int separator;

    // Analyse caractère par caractère pour séparer les deux nombres
    for (int i = 0; i < MAX_READ_LINE; i++) {
      if (g_data[i] == '\n')
        break;
      if (g_data[i] == ' ' || g_data[i] == '\t') {
        before_to = 0;
        separator = i + 1;
        continue;
      }

      if (before_to)
        from_buf[i] = g_data[i];
      else
        to_buf[i - separator] = g_data[i];
    }

    // Conversion des chaines en entiers
    int from = atoi(from_buf);
    int to = atoi(to_buf);

    // Ajout de l'arc dans le graphe
    graphm_add_arc(gm, from, to);
  }
  fclose(file);

  return gm;
}

// Lit un fichier texte pour remplir des LISTES d'adjacence
// (Logique identique à la fonction précédente)
t_graphl *graphl_from_file(char *file_path) {
  FILE *file;
  if (file_path == NULL) {
    printf("Pas de fichier spécifié, lecture depuis l'entrée standard.\n");
    file = stdin;
  } else
    file = fopen(file_path, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, file);

  int size = atoi(g_data);
  t_graphl *gl = new_graphl(size);

  // Lecture des données
  while (fgets(g_data, MAX_READ_LINE, file)) {
    if (g_data[0] == '\n')
      continue;

    char from_buf[MAX_READ_LINE] = {'\0'};
    char to_buf[MAX_READ_LINE] = {'\0'};
    t_bool before_to = 1;
    int separator;
    for (int i = 0; i < MAX_READ_LINE; i++) {
      if (g_data[i] == '\n')
        break;
      if (g_data[i] == ' ' || g_data[i] == '\t') {
        before_to = 0;
        separator = i + 1;
        continue;
      }

      if (before_to)
        from_buf[i] = g_data[i];
      else
        to_buf[i - separator] = g_data[i];
    }

    int from = atoi(from_buf);
    int to = atoi(to_buf);

    graphl_add_arc(gl, from, to);
  }
  fclose(file);

  return gl;
}

/* --- CONVERSION NOMS -> NUMEROS --- */

// Cette fonction est importante pour gérer les graphes définis par des mots.
// Elle lit le fichier original, crée une correspondance (Map) mot <-> numéro,
// et génère un fichier temporaire contenant uniquement des numéros pour que les algo fonctionnent dessus.
char **convert_word_to_num(char *inpath, char *outpath, int *size) {
  assert(size != NULL);

  // Ouverture des fichiers entrée/sortie
  FILE *in_file;
  if (inpath == NULL) {
    in_file = stdin;
  } else
    in_file = fopen(inpath, "r");
  assert(in_file != NULL);

  FILE *out_file;
  if (outpath == NULL) {
    printf("Erreur: chemin de sortie requis pour la conversion.\n");
    exit(1);
  } else
    out_file = fopen(outpath, "w");
  assert(out_file != NULL);

  // 1. Récupération de la taille (nombre de sommets)
  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, in_file);

  char size_buf[MAX_READ_LINE] = {'\0'};
  for (int i = 0; i < MAX_READ_LINE; i++) {
    if (g_data[i] == ' ' || g_data[i] == '\t')
      break; // On s'arrête au premier espace (au cas où il y a "5 sommets")

    size_buf[i] = g_data[i];
  }
  *size = atoi(size_buf);

  // 2. Création de la "Map" (Tableau de chaines de caractères)
  // map[0] contiendra le premier mot lu, map[1] le second, etc.
  char **map = malloc(*size * sizeof(char *));
  for (int i = 0; i < *size; i++)
    map[i] = malloc(MAX_READ_LINE * sizeof(char));

  // Écriture de la taille dans le fichier de sortie temporaire
  fprintf(out_file, "%d\n", *size);

  // 3. Lecture des Noms des sommets (au début du fichier)
  int i = 0;
  while (i < *size) {
    fgets(g_data, MAX_READ_LINE, in_file); // Lit la ligne
    if (g_data[0] == '\n')
      continue;

    // Copie le nom dans la map en enlevant le saut de ligne
    char buf[MAX_READ_LINE] = {'\0'};
    for (int j = 0; j < MAX_READ_LINE; j++) {
      if (g_data[j] == '\n') {
        strcpy(map[i], buf);
        break;
      }
      buf[j] = g_data[j];
    }

    i++;
  }

  // 4. Lecture des arcs et conversion en numéros
  while (fgets(g_data, MAX_READ_LINE, in_file)) {
    if (g_data[0] == '\n')
      continue;

    // Découpage de la ligne "start stop" en "0 1"
    char from_buf[MAX_READ_LINE] = {'\0'};
    char to_buf[MAX_READ_LINE] = {'\0'};
    t_bool before_to = 1;
    int separator;
    for (int i = 0; i < MAX_READ_LINE; i++) {
      if (g_data[i] == '\n')
        break;
      if (g_data[i] == ' ' || g_data[i] == '\t') {
        before_to = 0;
        separator = i + 1;
        continue;
      }

      if (before_to)
        from_buf[i] = g_data[i];
      else
        to_buf[i - separator] = g_data[i];
    }

    // Recherche de l'index correspondant au nom dans la map, méthode lente
    // pas le temps d'implémenter une recherche dico
    int from = -1;
    for (int j = 0; j < *size; j++) {
      if (strcmp(map[j], from_buf) == 0) {
        from = j;
        break;
      }
    }

    int to = -1;
    for (int j = 0; j < *size; j++) {
      if (strcmp(map[j], to_buf) == 0) {
        to = j;
        break;
      }
    }

    if (from < 0 || to < 0) {
      printf("Fichier corrompu : sommet inconnu trouvé dans les arcs.\n");
      exit(1);
    }

    // Écriture de la conversion en numéro dans le fichier temporaire
    fprintf(out_file, "%d %d\n", from, to);
  }

  fclose(in_file);
  fclose(out_file);
  return map; // On retourne la map pour pouvoir réafficher les noms à la fin
}

/* --- UTILITAIRES DE COPIE --- */

t_graphm *copy_graphm(t_graphm *gm) {
  t_graphm *gm_copy = new_graphm(gm->size);
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      gm_copy->m[i][j] = gm->m[i][j];
    }
  }
  return gm_copy;
}

t_graphl *copy_graphl(t_graphl *gl) {
  t_graphl *gl_copy = new_graphl(gl->size);
  for (int i = 0; i < gl->size; i++) {
    t_node *lc = list_cursor_new(gl->l[i]);
    while (!list_cursor_at_end(lc)) {
      int val = list_cursor_get_val(lc);
      list_add_head(val, gl_copy->l[i]);
      lc = list_cursor_next(lc);
    }
  }
  return gl_copy;
}

/* --- TRANSPOSITION DE GRAPHE --- */
// La transposée d'un graphe est le même graphe avec toutes les flèches inversées.
// C'est nécessaire pour l'algorithme de Kosaraju.

t_graphm *transpose_graphm(t_graphm *gm) {
  t_graphm *gm_transposed = copy_graphm(gm);
  for (int i = 0; i < gm->size; i++) {
    // On itère seulement sur le triangle supérieur pour ne pas échanger deux fois (#lebugquiastué1hdemavie 😮‍💨)
    for (int j = i + 1; j < gm->size; j++) {
      int temp = gm_transposed->m[i][j];
      gm_transposed->m[i][j] = gm_transposed->m[j][i];
      gm_transposed->m[j][i] = temp;
    }
  }
  return gm_transposed;
}

t_graphl *transpose_graphl(t_graphl *gl) {
  t_graphl *gl_transposed = new_graphl(gl->size); // attention c'est pas une copie là
  for (int i = 0; i < gl->size; i++) {
    // Pour chaque arc i -> val dans l'original...
    t_node *lc = list_cursor_new(gl->l[i]);
    while (!list_cursor_at_end(lc)) {
      int val = list_cursor_get_val(lc);
      // ... on ajoute un arc val -> i dans le transposé
      gl_transposed->l[val] = list_add_head(i, gl_transposed->l[val]);
      lc = list_cursor_next(lc);
    }
  }
  return gl_transposed;
}

/* --- AJOUT D'ARC --- */

void graphm_add_arc(t_graphm *gm, int from, int to) {
  assert(from < gm->size && to < gm->size);
  gm->m[from][to] = 1;
}

void graphl_add_arc(t_graphl *gl, int from, int to) {
  assert(from < gl->size && to < gl->size);
  // Ajoute 'to' dans la liste des voisins de 'from'
  gl->l[from] = list_add_head(to, gl->l[from]);
}

/* --- AFFICHAGE CONSOLE --- */

void show_graphm(t_graphm *gm, t_bool is_nom, char **map_nom) {
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      if (j == gm->size - 1) {
        if (is_nom) // Affiche en format "mots" si le graphe original était sous cette représentation
          printf("%s", map_nom[gm->m[i][j]]);
        else
          printf("%d", gm->m[i][j]);
      } else {
        if (is_nom)
          printf("%s, ", map_nom[gm->m[i][j]]);
        else
          printf("%d, ", gm->m[i][j]);
      }
    }
    printf("\n");
  }
}

void show_graphl(t_graphl *gl, t_bool is_nom, char **map_nom) {
  for (int i = 0; i < gl->size; i++) {
    printf("Ligne %d:\n", i);
    if (is_nom)
      list_show_with_map(gl->l[i], map_nom);
    else
      list_show(gl->l[i]);
  }
}

/* --- EXPORT FORMAT DOT (GraphViz) --- */
// Génère un fichier .dot décrivant le graphe pour le visualiser graphiquement

void graphm_to_dot(t_graphm *gm, char *out_path, t_bool is_nom, char **map_nom) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "digraph graphm {\n");
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      if (gm->m[i][j] == 1) {
        if (is_nom) // si représentation en "mots" on map le sommet en son sommet mot
          fprintf(file, "  %s -> %s;\n", map_nom[i], map_nom[j]);
        else
          fprintf(file, "  %d -> %d;\n", i, j);
      }
    }
  }
  fprintf(file, "}\n");
  fclose(file);
}

void graphl_to_dot(t_graphl *gl, char *out_path, t_bool is_nom, char **map_nom) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "digraph graphm {\n");
  for (int i = 0; i < gl->size; i++) {
    t_node *lc = list_cursor_new(gl->l[i]);
    while (!list_cursor_at_end(lc)) {
      if (is_nom)
        fprintf(file, "  %s -> %s;\n", map_nom[i], map_nom[list_cursor_get_val(lc)]);
      else
        fprintf(file, "  %d -> %d;\n", i, list_cursor_get_val(lc));

      lc = list_cursor_next(lc);
    }
  }
  fprintf(file, "}\n");
  fclose(file);
}

/* --- RECHERCHE DE CHEMIN (PATH FINDING) --- */

// Fonction wrapper pour la matrice
t_stack *graphm_find_path(t_graphm *gm, int from, int to, t_bool rec) {
  assert(from < gm->size);
  assert(to < gm->size);

  if (rec) {
    // Mode DFS Récursif
    t_stack *path = stack_new();                         // La pile qui contiendra le chemin
    t_bool *visited = malloc(gm->size * sizeof(t_bool)); // Tableau pour marquer les sommets visités
    for (int i = 0; i < gm->size; i++)
      visited[i] = 0;

    t_bool found = graphm_find_path_rec(gm, from, to, visited, path);
    if (!found)
      printf("Aucun chemin trouvé pour cette configuration\n");

    free(visited);
    return path;
  }

  // Mode DFS Itératif
  return graphm_find_path_it(gm, from, to);
}

// Fonction wrapper pour les listes
t_stack *graphl_find_path(t_graphl *gl, int from, int to, t_bool rec) {
  assert(from < gl->size);
  assert(to < gl->size);
  if (rec) {
    t_stack *path = stack_new();
    t_bool *visited = malloc(gl->size * sizeof(t_bool));
    for (int i = 0; i < gl->size; i++)
      visited[i] = 0;

    t_bool found = graphl_find_path_rec(gl, from, to, visited, path);
    if (!found)
      printf("Aucun chemin trouvé pour cette configuration\n");

    free(visited);
    return path;
  }

  return graphl_find_path_it(gl, from, to);
}

// Affiche le chemin trouvé de manière lisible (A -> B -> C)
void graph_show_path(t_stack *path, char *out_path, t_bool is_nom, char **map_nom) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "Le chemin est: ");

  t_node *lc = list_cursor_new(*path);
  while (!list_cursor_at_end(lc)) {
    int v = list_cursor_get_val(lc);

    // Vérifie si c'est le dernier élément pour ne pas mettre de flèche "->"
    if (list_cursor_at_end(list_cursor_next(lc))) {
      if (is_nom)
        fprintf(file, "%s;", map_nom[v]);
      else
        fprintf(file, "%d;", v);
    } else {
      if (is_nom)
        fprintf(file, "%s -> ", map_nom[v]);
      else
        fprintf(file, "%d -> ", v);
    }

    lc = list_cursor_next(lc);
  }

  fprintf(file, "\n");
  fclose(file);
}

// --- DFS Récursif ---
// Explore un chemin aussi loin que possible avant de revenir en arrière
t_bool graphm_find_path_rec(t_graphm *gm, int from, int to, t_bool *visited, t_stack *path) {
  // Cas de base : on est arrivé
  if (from == to) {
    stack_push(from, path);
    return 1;
  }

  // Si pas encore visité
  if (visited[from] == 0) {
    visited[from] = 1; // Marque comme visité pour ne pas tourner en rond

    // Teste tous les voisin
    for (int i = 0; i < gm->size; i++) {
      if (gm->m[from][i] == 0)
        continue; // Pas de lien

      // Appel récursif sur le voisin
      t_bool found = graphm_find_path_rec(gm, i, to, visited, path);
      if (found) {
        stack_push(from, path); // Si le voisin a trouvé le but, je m'ajoute au chemin
        return 1;
      }
    }
  }

  return 0; // Impasse, aucun chemin ne permet de relié les deux sommet
}

t_bool graphl_find_path_rec(t_graphl *gl, int from, int to, t_bool *visited, t_stack *path) {
  if (from == to) {
    stack_push(from, path);
    return 1;
  }

  if (visited[from] == 0) {
    visited[from] = 1;
    t_node *lc = list_cursor_new(gl->l[from]);
    while (!list_cursor_at_end(lc)) {
      int w = list_cursor_get_val(lc); // w est un voisin (PS: ne pas me demander pk je l'ai appelé w...)
      t_bool found = graphl_find_path_rec(gl, w, to, visited, path);
      if (found) {
        stack_push(from, path);
        return 1;
      }
      lc = list_cursor_next(lc);
    }
  }

  return 0;
}

// --- Recherche Itérative (DFS avec Pile) ---
t_stack *graphm_find_path_it(t_graphm *gm, int from, int to) {
  t_bool *visited = malloc(gm->size * sizeof(t_bool));
  for (int i = 0; i < gm->size; i++)
    visited[i] = 0;

  t_stack *traversal = stack_new(); // Pile pour gérer l'exploration
  t_stack *path = stack_new();      // Pile pour construire le chemin

  stack_push(from, traversal);
  int curr;

  while (!stack_is_empty(traversal)) {
    curr = stack_pop(traversal);

    // Marqueur spécial -1 indique qu'on remonte dans l'arbre d'exploration
    if (curr == -1) {
      stack_pop(path);
      continue;
    }

    if (curr == to) {
      stack_push(curr, path);
      break; // Trouvé ! 🥳
    }

    if (visited[curr] == 0) {
      visited[curr] = 1;
      stack_push(curr, path);
      stack_push(-1, traversal); // On empile un marqueur de retour

      // Ajout des voisins à explorer
      for (int i = 0; i < gm->size; i++) {
        if (gm->m[curr][i] == 1)
          stack_push(i, traversal);
      }
    }
  }

  free(visited);
  free_stack(traversal);

  return reverse_stack(path); // On inverse la stack pour avoir le bonne ordre
}

// même logique que `graphm_find_path_it`
t_stack *graphl_find_path_it(t_graphl *gl, int from, int to) {
  t_bool *visited = malloc(gl->size * sizeof(t_bool));
  for (int i = 0; i < gl->size; i++)
    visited[i] = 0;

  t_stack *traversal = stack_new();
  t_stack *path = stack_new();

  stack_push(from, traversal);
  int curr;
  while (!stack_is_empty(traversal)) {
    curr = stack_pop(traversal);
    if (curr == -1) {
      stack_pop(path);
      continue;
    }

    if (curr == to) {
      stack_push(curr, path);
      break;
    }

    if (visited[curr] == 0) {
      visited[curr] = 1;
      stack_push(curr, path);
      stack_push(-1, traversal);

      t_node *lc = list_cursor_new(gl->l[curr]);
      while (!list_cursor_at_end(lc)) {
        int w = list_cursor_get_val(lc);
        stack_push(w, traversal);
        lc = list_cursor_next(lc);
      }
    }
  }

  free(visited);
  free_stack(traversal);

  return reverse_stack(path);
}

/* --- RECHERCHE DE CFC (Composantes Fortement Connexes) avec l'algorithme de Kosaraju --- */

// Fonction principale pour Matrices
int **graphm_search_cfc(t_graphm *gm) {
  t_bool *marking = malloc(gm->size * sizeof(t_bool));
  for (int i = 0; i < gm->size; i++)
    marking[i] = 0;

  t_bool *order = malloc(gm->size * sizeof(t_bool)); // Stocke l'ordre de fin de traitement

  // ÉTAPE 1: Premier parcours DFS pour calculer l'ordre (dates de fin)
  int step = 0;
  for (int x = 0; x < gm->size; x++)
    if (marking[x] == 0)
      graphm_kosaraju_1_rec(gm, x, marking, order, &step);

  // ÉTAPE 2: Inversion du graphe (Transposée)
  t_graphm *gm_transposed = transpose_graphm(gm);

  // Inversion de l'ordre pour parcourir du dernier fini au premier
  t_bool *inv_order = malloc(gm->size * sizeof(t_bool));
  for (int x = 0; x < gm->size; x++)
    inv_order[(gm->size - 1) - order[x]] = x;

  // Réinitialisation des marqueurs
  for (int i = 0; i < gm->size; i++)
    marking[i] = 0;

  unsigned int nb_scc = 0; // Compteur de CFC

  // Allocation du tableau de résultats (ordre 2, par ex: [[2,3], [7,8,9]])
  int **cfc = malloc(gm->size * sizeof(int *));
  for (int i = 0; i < gm->size; i++) {
    cfc[i] = malloc(gm->size * sizeof(int));
    for (int j = 0; j < gm->size; j++) {
      cfc[i][j] = -1; // -1 = vide
    }
  }

  // ÉTAPE 3: Second parcours DFS sur le graphe transposé, selon l'ordre calculé
  for (int i = 0; i < gm->size; i++) {
    int u = inv_order[i];
    if (marking[u] == 0) {
      // Chaque appel ici découvre un nouvel CFC entière
      graphm_kosaraju_2_rec(gm_transposed, u, marking, cfc[nb_scc]);
      nb_scc++;
    }
  }

  // Nettoyage
  free_graphm(gm_transposed);
  free(inv_order);
  free(order);
  free(marking);

  return cfc;
}

// Fonction principale pour Listes (même logique)
int **graphl_search_cfc(t_graphl *gl) {
  t_bool *marking = malloc(gl->size * sizeof(t_bool));
  for (int i = 0; i < gl->size; i++)
    marking[i] = 0;

  t_bool *order = malloc(gl->size * sizeof(t_bool));

  // Étape 1
  int step = 0;
  for (int x = 0; x < gl->size; x++)
    if (marking[x] == 0)
      graphl_kosaraju_1_rec(gl, x, marking, order, &step);

  // Transposée
  t_graphl *gl_transposed = transpose_graphl(gl);

  // Ordre inverse
  t_bool *inv_order = malloc(gl->size * sizeof(t_bool));
  for (int x = 0; x < gl->size; x++)
    inv_order[(gl->size - 1) - order[x]] = x;

  // Réinit
  for (int i = 0; i < gl->size; i++)
    marking[i] = 0;

  unsigned int nb_scc = 0;

  // Init résultat
  int **cfc = malloc(gl->size * sizeof(int *));
  for (int i = 0; i < gl->size; i++) {
    cfc[i] = malloc(gl->size * sizeof(int));
    for (int j = 0; j < gl->size; j++) {
      cfc[i][j] = -1;
    }
  }

  // Étape 2
  for (int i = 0; i < gl->size; i++) {
    int u = inv_order[i];
    if (marking[u] == 0) {
      graphl_kosaraju_2_rec(gl_transposed, u, marking, cfc[nb_scc]);
      nb_scc++;
    }
  }

  free_graphl(gl_transposed);
  free(inv_order);
  free(order);
  free(marking);

  return cfc;
}

// Sous-fonction Kosaraju Étape 1 : DFS simple qui note l'order de fin
void graphm_kosaraju_1_rec(t_graphm *gm, int x, int *marking, int *order, int *step) {
  marking[x] = 1;
  for (int y = 0; y < gm->size; y++) {
    if (gm->m[x][y] == 1 && marking[y] == 0)
      graphm_kosaraju_1_rec(gm, y, marking, order, step);
  }
  order[x] = *step; // On note l'étape à laquelle on a fini de traiter x
  (*step)++;
}

// même logique
void graphl_kosaraju_1_rec(t_graphl *gl, int x, int *marking, int *order, int *step) {
  marking[x] = 1;

  t_node *lc = list_cursor_new(gl->l[x]);
  while (!list_cursor_at_end(lc)) {
    int y = list_cursor_get_val(lc);
    if (marking[y] == 0)
      graphl_kosaraju_1_rec(gl, y, marking, order, step);

    lc = list_cursor_next(lc);
  }
  order[x] = *step;
  (*step)++;
}

// Sous-fonction Kosaraju Étape 2 : DFS qui remplit la CFC courante
void graphm_kosaraju_2_rec(t_graphm *gm, int x, int *marking, int *curr_cfc) {
  marking[x] = 1;

  // Ajoute x à la liste des membres de la CFC actuelle
  int curr_index = 0;
  while (curr_cfc[curr_index] != -1 && curr_index < gm->size)
    curr_index++;
  curr_cfc[curr_index] = x;

  for (int y = 0; y < gm->size; y++) {
    if (gm->m[x][y] == 1 && marking[y] == 0)
      graphm_kosaraju_2_rec(gm, y, marking, curr_cfc);
  }
}

// même logique
void graphl_kosaraju_2_rec(t_graphl *gl, int x, int *marking, int *curr_cfc) {
  marking[x] = 1;
  int curr_index = 0;
  while (curr_cfc[curr_index] != -1 && curr_index < gl->size)
    curr_index++;
  curr_cfc[curr_index] = x;

  t_node *lc = list_cursor_new(gl->l[x]);
  while (!list_cursor_at_end(lc)) {
    int y = list_cursor_get_val(lc);
    if (marking[y] == 0)
      graphl_kosaraju_2_rec(gl, y, marking, curr_cfc);

    lc = list_cursor_next(lc);
  }
}

// Affiche les Composantes Fortement Connexes trouvées
void show_cfc(int **cfc, int size, FILE *file, t_bool is_nom, char **map_nom) {
  for (int i = 0; i < size; i++) {
    if (cfc[i][0] == -1)
      continue; // CFC vide
    fprintf(file, "Composante #%d: [", i + 1);
    for (int j = 0; j < size; j++) {
      // Si dernier élément, affichage différent
      if (j == size - 1 || cfc[i][j + 1] == -1) {
        if (is_nom) // si représentation nom, on map le sommet en son sommet mot
          fprintf(file, "%s", map_nom[cfc[i][j]]);
        else
          fprintf(file, "%d", cfc[i][j]);

        break;
      }

      if (is_nom)
        fprintf(file, "%s, ", map_nom[cfc[i][j]]);
      else
        fprintf(file, "%d, ", cfc[i][j]);
    }
    fprintf(file, "]\n");
  }
}

void free_cfc(int **cfc, int size) {
  for (int i = 0; i < size; i++)
    free(cfc[i]);
  free(cfc);
}

/* --- FILTRE CFC TERMINAL --- */
// Un CFC est "terminal" si aucun arc ne sort de ce cfc pour aller vers un autre sommet.

int **cfctm_filter(t_graphm *gm, int **cfc) {
  // Prépare le tableau résultat
  int **cfct = malloc(gm->size * sizeof(int *));
  for (int i = 0; i < gm->size; i++) {
    cfct[i] = malloc(gm->size * sizeof(int));
    for (int j = 0; j < gm->size; j++) {
      cfct[i][j] = -1;
    }
  }

  // Pour chaque CFC 'i'
  for (int i = 0; i < gm->size; i++) {
    if (cfc[i][0] == -1)
      continue;

    // Pour chaque membre 'j' de la CFC 'i'
    for (int j = 0; j < gm->size; j++) {
      if (cfc[i][j] == -1)
        break;

      int curr_cfc_member = cfc[i][j];
      t_bool not_a_cfct = 0; // Flag: "n'est pas terminal"

      // On regarde tous les voisins 'k' de ce membre
      for (int k = 0; k < gm->size; k++) {
        if (gm->m[curr_cfc_member][k] == 1) {
          // Si il y a un lien vers k, est-ce que k est dans la même CFC ?
          t_bool k_in_cfc = 0;
          for (int l = 0; l < gm->size; l++) {
            if (cfc[i][l] == k) {
              k_in_cfc = 1;
              break;
            }
          }

          // Si k n'est pas dans la même CFC, alors on sort du groupe -> PAS Terminal
          if (!k_in_cfc) {
            not_a_cfct = 1;
            break;
          }
        }
      }

      if (not_a_cfct) {
        // On vide la ligne car ce groupe n'est pas terminal
        for (int k = 0; k < gm->size; k++)
          cfct[i][k] = -1;
        break; // On passe à la CFC suivante
      } else {
        // Pour l'instant c'est bon, on copie le membre
        cfct[i][j] = curr_cfc_member;
      }
    }
  }

  return cfct;
}

// Même logique que cfctm_filter mais adaptée aux listes
int **cfctl_filter(t_graphl *gl, int **cfc) {
  int **cfct = malloc(gl->size * sizeof(int *));
  for (int i = 0; i < gl->size; i++) {
    cfct[i] = malloc(gl->size * sizeof(int));
    for (int j = 0; j < gl->size; j++) {
      cfct[i][j] = -1;
    }
  }

  for (int i = 0; i < gl->size; i++) {
    if (cfc[i][0] == -1)
      continue;
    for (int j = 0; j < gl->size; j++) {
      if (cfc[i][j] == -1)
        break;

      int curr_cfc_member = cfc[i][j];

      t_bool not_a_cfct = 0;
      t_node *lc = list_cursor_new(gl->l[curr_cfc_member]);
      while (!list_cursor_at_end(lc)) {
        int k = list_cursor_get_val(lc);
        // Vérifie si le voisin k est dans la même CFC
        t_bool k_in_cfc = 0;
        for (int l = 0; l < gl->size; l++) {
          if (cfc[i][l] == k) {
            k_in_cfc = 1;
            break;
          }
        }

        if (!k_in_cfc) {
          not_a_cfct = 1;
          break;
        }
        lc = list_cursor_next(lc);
      }

      if (not_a_cfct) {
        for (int k = 0; k < gl->size; k++)
          cfct[i][k] = -1;
        break;
      } else {
        cfct[i][j] = curr_cfc_member;
      }
    }
  }

  return cfct;
}

/* --- LIBERATION MEMOIRE --- */

void free_graphm(t_graphm *graph) {
  for (int i = 0; i < graph->size; i++)
    free(graph->m[i]); // Libère chaque ligne
  free(graph->m);      // Libère le tableau de lignes
  free(graph);         // Libère la structure
}

void free_graphl(t_graphl *graph) {
  for (int i = 0; i < graph->size; i++)
    list_free(graph->l[i]); // Libère chaque liste chaînée

  free(graph->l); // Libère le tableau de listes
  free(graph);    // Libère la structure
}