#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Splitted code
#include "graph.h"
#include "ll.h"
#include "stack.h"

#define TEMP_CONVERTED_REPRESENTATION "./converted.txt"

typedef enum {
  DOT,
  FIND,
  CFC
} t_command;

t_bool is_name_representation(char *inpath);

int main(int argc, char **argv) {
  /* ---------PARSER--------- */
  assert(argc >= 2 && argc <= 12);

  // argument parsing
  t_command command;
  if (strcmp(argv[1], "dot") == 0)
    command = DOT;
  else if (strcmp(argv[1], "find") == 0)
    command = FIND;
  else if (strcmp(argv[1], "cfc") == 0)
    command = CFC;
  else {
    printf("FATAL: Argument non reconnu ou omis.\n");
    printf("Exemple:\n");
    printf("./fil_rouge dot -i ex1.txt -o out.txt\n");
    printf("./fil_rouge find -start 0 -goal 3 -i ex1.txt -o out.txt\n");
    printf("./fil_rouge cfc -i ./tests/graphes/numeros/ex2.txt -cfct\n");
    exit(1); // leave progam
  }

  // flags parsing
  char *input = NULL;
  char *output = NULL;

  int start = -1;
  int goal = -1;

  t_bool is_rec = 0; // recursive mode for "find" command
  t_bool use_ll = 0; // whether to use or not the linked list representation for graph
  t_bool print_cfct = 0;

  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      output = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      input = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-start") == 0 && i + 1 < argc) {
      start = atoi(argv[i + 1]);
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
      printf("FATAL: flag non reconnu ou omis.\n");
      printf("Exemple:\n");
      printf("./fil_rouge dot -i ex1.txt -o out.txt\n");
      printf("./fil_rouge find -start 0 -goal 3 -i ex1.txt -o out.txt\n");
      printf("./fil_rouge cfc -i ./tests/graphes/numeros/ex2.txt -cfct\n");
      exit(1); // leave progam
    }
  }

  if (command == FIND) {
    // ask for required flags
    while (start < 0) {
      printf("Please choose a starting point.\n");
      scanf("%d", &start);
    }
    while (goal < 0) {
      printf("Please choose a target point.\n");
      scanf("%d", &goal);
    }
  }

  // verifie quelle type de représentation est utiliser (nombre/nom) et s'adapte en conséquent
  char **map_nom;
  int *map_nom_len;
  t_bool is_nom = is_name_representation(input);
  if (is_nom) {
    map_nom_len = malloc(sizeof(int));
    assert(map_nom_len != NULL);
    map_nom = convert_word_to_num(input, TEMP_CONVERTED_REPRESENTATION, map_nom_len);
    input = TEMP_CONVERTED_REPRESENTATION;
  }

  /* ---------PROGAM--------- */
  if (use_ll) {
    /* linked list implementation */
    t_graphl *gl = graphl_from_file(input);

    // which command to execute
    if (command == DOT)
      graphl_to_dot(gl, output, is_nom, map_nom);
    else if (command == FIND) {
      t_stack *path = graphl_find_path(gl, start, goal, is_rec);
      graph_show_path(path, output, is_nom, map_nom);
      free_stack(path); // free memory
    } else {
      FILE *file;
      if (output == NULL)
        file = stdout;
      else
        file = fopen(output, "w");
      assert(file != NULL);

      int **cfc = graphl_search_cfc(gl);
      fprintf(file, "CFC:\n");
      show_cfc(cfc, gl->size, file, is_nom, map_nom);

      if (print_cfct) {
        fprintf(file, "CFC Terminal:\n");
        int **cfct = cfctl_filter(gl, cfc);
        show_cfc(cfct, gl->size, file, is_nom, map_nom);
        free_cfc(cfct, gl->size);
      }

      free_cfc(cfc, gl->size);
    }

    free_graphl(gl); // free memory
  } else {
    /* matrix implementation */
    t_graphm *gm = graphm_from_file(input);

    // which command to execute
    if (command == DOT)
      graphm_to_dot(gm, output, is_nom, map_nom);
    else if (command == FIND) {
      t_stack *path = graphm_find_path(gm, start, goal, is_rec);

      graph_show_path(path, output, is_nom, map_nom);
      free_stack(path); // free memory
    } else {
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

    free_graphm(gm); // free memory
  }

  // free les variable en relation avec la représentation en "nom"
  if (is_nom) {
    for (int i = 0; i < *map_nom_len; i++)
      free(map_nom[i]);

    free(map_nom);
    free(map_nom_len);

    // supprime le fichier temporaire nom->nombre 'converted.txt'
    if (remove(TEMP_CONVERTED_REPRESENTATION) != 0) {
      printf("Failed to remove temporary converted file\n");
      return 1;
    }
  }

  return 0;
}

// check if the graph representation of the input file is name or numeros.
t_bool is_name_representation(char *inpath) {
  FILE *file;
  if (inpath == NULL) {
    printf("You'll be prompted to specify your graph with an abjacent linked list\n");
    file = stdin;
  } else
    file = fopen(inpath, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, file);

  for (int i = 0; i < MAX_READ_LINE; i++) {
    if (g_data[i] == '\n')
      break;
    if ((g_data[i] == ' ' || g_data[i] == '\t') && i + 1 < MAX_READ_LINE && g_data[i + 1] == 'n') {
      fclose(file);
      return 1;
    }
  }

  fclose(file);
  return 0;
}