#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion des autres modules
#include "graph.h"
#include "ll.h"
#include "stack.h"

// Fichier de conversion temporaire utilisé si le graphe utilise les noms
#define TEMP_CONVERTED_REPRESENTATION "./converted.txt"

// enum des commandes possibles du programme
typedef enum {
  DOT,  // Conversion en format DOT pour visualisation
  FIND, // Recherche de chemin
  CFC   // Recherche des Composantes Fortement Connexes
} t_command;

// Prototype de la fonction utilitaire pour détecter le format du fichier
t_bool is_name_representation(char *inpath);

// Entrée principale du programme
int main(int argc, char **argv) {
  /* --------- ANALYSE DES ARGUMENTS (PARSER) --------- */
  // Vérifie que le nombre d'arguments est cohérent (entre 2 et 12)
  assert(argc >= 2 && argc <= 12);

  // 1. Détermination de la commande principale
  t_command command;
  if (strcmp(argv[1], "dot") == 0)
    command = DOT;
  else if (strcmp(argv[1], "find") == 0)
    command = FIND;
  else if (strcmp(argv[1], "cfc") == 0)
    command = CFC;
  else {
    // Gestion d'erreur si la commande n'est pas reconnue
    printf("FATAL: Argument non reconnu ou omis.\n");
    printf("Exemple:\n");
    printf("./fil_rouge dot -i ex1.txt -o out.txt\n");
    printf("./fil_rouge find -start 0 -goal 3 -i ex1.txt -o out.txt\n");
    printf("./fil_rouge cfc -i ./tests/graphes/numeros/ex2.txt -cfct\n");
    exit(1); // Arrêt du programme
  }

  // 2. Initialisation des variables pour les flags
  char *input = NULL;  // Chemin du fichier d'entrée
  char *output = NULL; // Chemin du fichier de sortie

  int start = -1; // Sommet de départ (pour la commande "find")
  int goal = -1;  // Sommet d'arrivée (pour la commande "find")

  t_bool is_rec = 0;     // Mode récursif pour la recherche de chemin (par défaut itératif)
  t_bool use_ll = 0;     // Utiliser les listes d'adjacence (par défaut matrices)
  t_bool print_cfct = 0; // afficher les CFC Terminales

  // 3. boucle pour recupérer les flags
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      output = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      input = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-start") == 0 && i + 1 < argc) {
      start = atoi(argv[i + 1]); // Conversion chaîne vers entier
      i++;
    } else if (strcmp(argv[i], "-goal") == 0 && i + 1 < argc) {
      goal = atoi(argv[i + 1]);
      i++;
    } else if (strcmp(argv[i], "-rec") == 0)
      is_rec = 1;
    else if (strcmp(argv[i], "-ll") == 0)
      use_ll = 1;
    else if (strcmp(argv[i], "-cfct") == 0)
      print_cfct = 1;
    else {
      // Gestion d'erreur si un flag est inconnu
      printf("FATAL: flag non reconnu ou omis.\n");
      printf("Exemple:\n");
      printf("./fil_rouge dot -i ex1.txt -o out.txt\n");
      printf("./fil_rouge find -start 0 -goal 3 -i ex1.txt -o out.txt\n");
      printf("./fil_rouge cfc -i ./tests/graphes/numeros/ex2.txt -cfct\n");
      exit(1);
    }
  }

  // Si on cherche un chemin mais que l'utilisateur n'a pas donné les points, on lui demande
  if (command == FIND) {
    while (start < 0) {
      printf("Veuillez choisir un point de depart.\n");
      scanf("%d", &start);
    }
    while (goal < 0) {
      printf("Veuillez choisir un point d'arrivee.\n");
      scanf("%d", &goal);
    }
  }

  /* --------- GESTION DES NOMS VS NUMEROS --------- */
  // Vérifie si le fichier utilise des mots ou des numéros pour les sommets
  char **map_nom; // Tableau pour stocker la correspondance (0 -> "start", 1 -> "stop")
  int *map_nom_len;
  t_bool is_nom = is_name_representation(input);

  if (is_nom) {
    // Si c'est des noms, on crée une map et un fichier temporaire converti en numéros
    // car nos algorithmes fonctionnent mathématiquement avec des entiers.
    map_nom_len = malloc(sizeof(int));
    assert(map_nom_len != NULL);
    // On génère le fichier temporaire et remplit map_nom
    map_nom = convert_word_to_num(input, TEMP_CONVERTED_REPRESENTATION, map_nom_len);
    // On change l'entrée pour lire le fichier temporaire (qui ne contient que des nombres)
    input = TEMP_CONVERTED_REPRESENTATION;
  }

  /* --------- PROGRAMME PRINCIPAL --------- */
  // Le code est séparé en deux blocs quasi identiques :
  // L'un pour l'implémentation par Listes Chainées (if use_ll)
  // L'autre pour l'implémentation par Matrices d'Adjacence (else)

  if (use_ll) {
    /* --- Implémentation : Listes d'adjacence --- */

    // Chargement du graphe en mémoire
    t_graphl *gl = graphl_from_file(input);

    // Exécution de la commande choisie
    if (command == DOT)
      // Export en format DOT
      graphl_to_dot(gl, output, is_nom, map_nom);

    else if (command == FIND) {
      // Recherche de chemin (récursif ou itératif selon le flag is_rec)
      t_stack *path = graphl_find_path(gl, start, goal, is_rec);
      // Affichage du chemin trouvé
      graph_show_path(path, output, is_nom, map_nom);
      free_stack(path); // Nettoyage mémoire du chemin

    } else { // Commande CFC
      FILE *file;
      // Gestion de la sortie (fichier ou terminal)
      if (output == NULL)
        file = stdout;
      else
        file = fopen(output, "w");
      assert(file != NULL);

      // Calcul des Composantes Fortement Connexes
      int **cfc = graphl_search_cfc(gl);
      fprintf(file, "CFC:\n");
      show_cfc(cfc, gl->size, file, is_nom, map_nom);

      // Si demandé, filtre pour ne garder que les CFC Terminales
      if (print_cfct) {
        fprintf(file, "CFC Terminal:\n");
        int **cfct = cfctl_filter(gl, cfc);
        show_cfc(cfct, gl->size, file, is_nom, map_nom);
        free_cfc(cfct, gl->size); // Nettoyage CFC Terminales
      }

      free_cfc(cfc, gl->size); // Nettoyage CFC
    }

    free_graphl(gl); // Nettoyage du graphe complet

  } else {
    /* --- Implémentation : Matrice d'adjacence --- */
    // La logique est identique au bloc ci-dessus, mais avec les fonctions graphm_*

    t_graphm *gm = graphm_from_file(input);

    if (command == DOT)
      graphm_to_dot(gm, output, is_nom, map_nom);

    else if (command == FIND) {
      t_stack *path = graphm_find_path(gm, start, goal, is_rec);

      graph_show_path(path, output, is_nom, map_nom);
      free_stack(path);

    } else { // CFC
      FILE *file;
      if (output == NULL)
        file = stdout;
      else
        file = fopen(output, "w");
      assert(file != NULL);

      int **cfc = graphm_search_cfc(gm);
      fprintf(file, "CFC:\n");
      show_cfc(cfc, gm->size, file, is_nom, map_nom);

      if (print_cfct) {
        fprintf(file, "CFC Terminal:\n");
        int **cfct = cfctm_filter(gm, cfc);
        show_cfc(cfct, gm->size, file, is_nom, map_nom);
        free_cfc(cfct, gm->size);
      }

      free_cfc(cfc, gm->size);
      fclose(file);
    }

    free_graphm(gm); // Nettoyage du graphe complet
  }

  /* --------- NETTOYAGE FINAL --------- */
  // Si on a utilisé le mode "noms", il faut libérer la mémoire associée
  // et supprimer le fichier temporaire
  if (is_nom) {
    for (int i = 0; i < *map_nom_len; i++)
      free(map_nom[i]); // Libère chaque mot

    free(map_nom); // Libère le tableau
    free(map_nom_len);

    // Supprime le fichier temporaire 'converted.txt'
    if (remove(TEMP_CONVERTED_REPRESENTATION) != 0) {
      printf("Echec lors de la suppression du fichier temporaire converti\n");
      return 1;
    }
  }

  return 0; // Fin normale du programme
}

// Fonction utilitaire: vérifie si le fichier d'entrée utilise des noms ou des numéros
// Cherche sur la 1er ligne la présence d'un " n"
t_bool is_name_representation(char *inpath) {
  FILE *file;
  if (inpath == NULL) {
    printf("Vous serez invite a specifier votre graphe avec une liste chainee adjacente\n");
    file = stdin;
  } else
    file = fopen(inpath, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  // Lit la première ligne
  fgets(g_data, MAX_READ_LINE, file);

  for (int i = 0; i < MAX_READ_LINE; i++) {
    if (g_data[i] == '\n')
      break;
    // Vérification du format spécifique du fichier
    if ((g_data[i] == ' ' || g_data[i] == '\t') && i + 1 < MAX_READ_LINE && g_data[i + 1] == 'n') {
      fclose(file);
      return 1; // Le graphes est en repésentation mots
    }
  }

  fclose(file);
  return 0; // Le graphes est en repésentation nombre
}