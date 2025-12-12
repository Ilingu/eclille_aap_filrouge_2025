#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "ll.h"

#define MAX_READ_LINE 100

/* NEW EMPTY GRAPH */
t_graphm *new_graphm(int size) {
  t_graphm *p_g;

  // Allocation de la structure t_graphm
  p_g = malloc(sizeof(*p_g));
  assert(p_g != NULL);
  p_g->size = size;

  // Allocation de la première dimension de la matrice d'adjacence
  p_g->m = malloc(size * sizeof(t_bool *));
  assert(p_g->m != NULL);

  // Allocation de la deuxième dimension de la matrice d'adjacence
  for (int i = 0; i < size; i++) {
    p_g->m[i] = malloc(size * sizeof(t_bool));
    assert(p_g->m[i] != NULL);
    for (int j = 0; j < size; j++)
      p_g->m[i][j] = 0;
  }

  return p_g;
}
t_graphl *new_graphl(int size) {
  // Allocation de la structure t_graphl
  t_graphl *pg = malloc(sizeof(*pg));
  assert(pg != NULL);
  pg->size = size;

  // Allocation du tableau de têtes de listes d'adjacence
  pg->l = malloc(pg->size * sizeof(*(pg->l)));
  assert(pg->l != NULL);

  for (int i = 0; i < size; i++)
    pg->l[i] = NULL;
  return pg;
}

/* FROM FILE */
t_graphm *graphm_from_file(char *file_path) {
  FILE *file;
  if (file_path == NULL)
    file = stdin;
  else
    file = fopen(file_path, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, file);

  int size = atoi(g_data);
  t_graphm *gm = new_graphm(size);

  // gather data
  while (fgets(g_data, MAX_READ_LINE, file)) {
    if (g_data[0] == '\n')
      continue; // Skip empty line

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

    graphm_add_arc(gm, from, to);
  }
  fclose(file);

  return gm;
}
t_graphl *graphl_from_file(char *file_path) {
  FILE *file;
  if (file_path == NULL)
    file = stdin;
  else
    file = fopen(file_path, "r");
  assert(file != NULL);

  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, file);

  int size = atoi(g_data);
  t_graphl *gl = new_graphl(size);

  // gather data
  while (fgets(g_data, MAX_READ_LINE, file)) {
    if (g_data[0] == '\n')
      continue; // Skip empty line

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

/* ADD ARC */
void graphm_add_arc(t_graphm *gm, int from, int to) {
  assert(from < gm->size && to < gm->size);
  gm->m[from][to] = 1;
}
void graphl_add_arc(t_graphl *gl, int from, int to) {
  assert(from < gl->size && to < gl->size);
  gl->l[from] = list_add_head(to, gl->l[from]);
}

/* SHOW GRAPH */
void show_graphm(t_graphm *gm) {
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      if (j == gm->size - 1)
        printf("%d", gm->m[i][j]);
      else
        printf("%d, ", gm->m[i][j]);
    }
    printf("\n");
  }
}
void show_graphl(t_graphl *gl) {
  for (int i = 0; i < gl->size; i++) {
    printf("Ligne %d:\n", i);
    list_show(gl->l[i]);
  }
}

/* TO .DOT FILE */
void graphm_to_dot(t_graphm *gm, char *out_path) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "digraph graphm {\n");
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      if (gm->m[i][j] == 1)
        fprintf(file, "  %d -> %d;\n", i, j);
    }
  }
  fprintf(file, "}\n");

  fclose(file);
}
void graphl_to_dot(t_graphl *gl, char *out_path) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "digraph graphm {\n");
  for (int i = 0; i < gl->size; i++) {
    t_node *n = gl->l[i];
    while (n != NULL) {
      fprintf(file, "  %d -> %d;\n", i, n->val);
      n = n->p_next;
    }
  }
  fprintf(file, "}\n");

  fclose(file);
}

/* FREE */
void free_graphm(t_graphm *graph) {
  for (int i = 0; i < graph->size; i++)
    free(graph->m[i]);
  free(graph->m);
  free(graph);
}
void free_graphl(t_graphl *graph) {
  for (int i = 0; i < graph->size; i++)
    list_free(graph->l[i]);

  free(graph->l);
  free(graph);
}
