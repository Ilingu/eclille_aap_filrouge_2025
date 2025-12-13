#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Splitted code
#include "graph.h"
#include "ll.h"
#include "stack.h"

int main(int argc, char **argv) {
  assert(argc <= 5);
  char *input = NULL;
  char *output = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
      output = argv[i + 1];
    else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
      input = argv[i + 1];
    else
      exit(1);
    i++;
  }

  /* GRAPHM TESTS */
  // printf("------GRAPHM TESTS------\n");
  // t_graphm *gm = graphm_from_file(input);
  // show_graphm(gm);
  // graphm_to_dot(gm, output);

  // free_graphm(gm);

  /* GRAPHL TESTS */
  printf("------GRAPHL TESTS------\n");
  t_graphl *gl = graphl_from_file(input);
  show_graphl(gl);
  graphl_to_dot(gl, output);

  free_graphl(gl);

  return 0;
}
