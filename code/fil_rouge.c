#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Splitted code
#include "graph.h"
#include "ll.h"
#include "stack.h"

typedef enum {
  DOT,
  FIND,
  CFC
} t_command;

int main(int argc, char **argv) {
  /* ---------PARSER--------- */
  assert(argc >= 2 && argc <= 11);

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
    printf("./fil_rouge cfc\n");
    exit(1); // leave progam
  }

  // flags parsing
  char *input = NULL;
  char *output = NULL;

  int start = -1;
  int goal = -1;

  t_bool is_rec = 0; // recursive mode for "find" command
  t_bool use_ll = 0; // whether to use or not the linked list representation for graph

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
    else {
      printf("FATAL: flag non reconnu ou omis.\n");
      printf("Exemple:\n");
      printf("./fil_rouge dot -i ex1.txt -o out.txt\n");
      printf("./fil_rouge find -start 0 -goal 3 -i ex1.txt -o out.txt\n");
      // printf("./fil_rouge cfc\n");
      exit(1); // leave progam
    }
  }

  // ask for required flags
  while (start < 0) {
    printf("Please choose a starting point.\n");
    scanf("%d", &start);
  }
  while (goal < 0) {
    printf("Please choose a target point.\n");
    scanf("%d", &goal);
  }

  /* ---------PROGAM--------- */
  if (use_ll) {
    /* linked list implementation */
    t_graphl *gl = graphl_from_file(input);

    // which command to execute
    if (command == DOT)
      graphl_to_dot(gl, output);
    else if (command == FIND) {
      t_stack *path = graphl_find_path(gl, start, goal, is_rec);
      graph_show_path(path, output);
      free_stack(path); // free memory
    } else {
    }

    free_graphl(gl); // free memory
  } else {
    /* matrix implementation */
    t_graphm *gm = graphm_from_file(input);

    // which command to execute
    if (command == DOT)
      graphm_to_dot(gm, output);
    else if (command == FIND) {
      t_stack *path = graphm_find_path(gm, start, goal, is_rec);

      graph_show_path(path, output);
      free_stack(path); // free memory
    } else {
    }

    free_graphm(gm); // free memory
  }

  return 0;
}
