#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "ll.h"
#include "stack.h"

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
  if (file_path == NULL) {
    printf(
        "You'll be prompted to specify your graph with an abjacent matrix\n");
    file = stdin;
  } else
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
  if (file_path == NULL) {
    printf("You'll be prompted to specify your graph with an abjacent linked list\n");
    file = stdin;
  } else
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

/* CONVERT WORD REPRESENTATION TO NUMBER */

// returns a list of strings of the mapped words
char **convert_word_to_num(char *inpath, char *outpath, int *size) {
  // open in-out files
  assert(size != NULL);
  FILE *in_file;
  if (inpath == NULL) {
    printf("You'll be prompted to specify your graph with an abjacent linked list\n");
    in_file = stdin;
  } else
    in_file = fopen(inpath, "r");
  assert(in_file != NULL);

  FILE *out_file;
  if (outpath == NULL) {
    printf("You need an out path to convert to num representation\n");
    exit(1);
  } else
    out_file = fopen(outpath, "w");
  assert(out_file != NULL);

  // fetch size, aka number of sommets
  char g_data[MAX_READ_LINE];
  fgets(g_data, MAX_READ_LINE, in_file);

  char size_buf[MAX_READ_LINE] = {'\0'};
  for (int i = 0; i < MAX_READ_LINE; i++) {
    if (g_data[i] == ' ' || g_data[i] == '\t')
      break;

    size_buf[i] = g_data[i];
  }
  *size = atoi(size_buf);

  // compute the map list, to convert numeros->nom
  // for ex1.txt: map[0] is "start", etc...
  char **map = malloc(*size * sizeof(char *));
  for (int i = 0; i < *size; i++)
    map[i] = malloc(MAX_READ_LINE * sizeof(char));

  fprintf(out_file, "%d\n", *size);

  int i = 0;
  while (i < *size) {
    fgets(g_data, MAX_READ_LINE, in_file); // read the line
    if (g_data[0] == '\n')
      continue; // Skip empty line

    // put the line without the '\n' in the map
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

  // gather arc data of the graph
  while (fgets(g_data, MAX_READ_LINE, in_file)) {
    if (g_data[0] == '\n')
      continue; // Skip empty line

    // split a line like "start stop" into "0 1"
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
      printf("Corrupted file\n");
      exit(1);
    }

    fprintf(out_file, "%d %d\n", from, to);
  }

  fclose(in_file);
  fclose(out_file);
  return map;
}

/* COPY GRAPH */
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

/* TRANSPOSE GRAPH */
t_graphm *transpose_graphm(t_graphm *gm) {
  t_graphm *gm_transposed = copy_graphm(gm);
  for (int i = 0; i < gm->size; i++) {
    // iterate only on the upper triangle, to prevent double transposing (HI the bug that killed 1h of my life 😮‍💨)
    for (int j = i + 1; j < gm->size; j++) {
      int temp = gm_transposed->m[i][j];
      gm_transposed->m[i][j] = gm_transposed->m[j][i];
      gm_transposed->m[j][i] = temp;
    }
  }
  return gm_transposed;
}
t_graphl *transpose_graphl(t_graphl *gl) {
  t_graphl *gl_transposed = new_graphl(gl->size);
  for (int i = 0; i < gl->size; i++) {
    t_node *lc = list_cursor_new(gl->l[i]);
    while (!list_cursor_at_end(lc)) {
      int val = list_cursor_get_val(lc);
      gl_transposed->l[val] = list_add_head(i, gl_transposed->l[val]);
      lc = list_cursor_next(lc);
    }
  }
  return gl_transposed;
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
void show_graphm(t_graphm *gm, t_bool is_nom, char **map_nom) {
  for (int i = 0; i < gm->size; i++) {
    for (int j = 0; j < gm->size; j++) {
      if (j == gm->size - 1) {
        if (is_nom)
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

/* TO .DOT FILE */
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
        if (is_nom)
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

/* PATH FINDING */
t_stack *graphm_find_path(t_graphm *gm, int from, int to, t_bool rec) {
  assert(from < gm->size);
  assert(to < gm->size);
  if (rec) {
    t_stack *path = stack_new();
    t_bool *visited = malloc(gm->size * sizeof(t_bool));
    for (int i = 0; i < gm->size; i++)
      visited[i] = 0;

    t_bool found = graphm_find_path_rec(gm, from, to, visited, path);
    if (!found)
      printf("Aucun chemin trouvé pour cette configuration\n");

    free(visited);
    return path;
  }

  return graphm_find_path_it(gm, from, to);
}
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

void graph_show_path(t_stack *path, char *out_path, t_bool is_nom, char **map_nom) {
  FILE *file;
  if (out_path == NULL)
    file = stdout;
  else
    file = fopen(out_path, "w");
  assert(file != NULL);

  fprintf(file, "The path is: ");

  t_node *lc = list_cursor_new(*path);
  while (!list_cursor_at_end(lc)) {
    int v = list_cursor_get_val(lc);

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

// DFS
t_bool graphm_find_path_rec(t_graphm *gm, int from, int to, t_bool *visited, t_stack *path) {
  if (from == to) {
    stack_push(from, path);
    return 1;
  }

  if (visited[from] == 0) {
    visited[from] = 1;
    for (int i = 0; i < gm->size; i++) {
      if (gm->m[from][i] == 0)
        continue;
      t_bool found = graphm_find_path_rec(gm, i, to, visited, path);
      if (found) {
        stack_push(from, path);
        return 1;
      }
    }
  }

  return 0;
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
      int w = list_cursor_get_val(lc);
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

// BFS
t_stack *graphm_find_path_it(t_graphm *gm, int from, int to) {
  t_bool *visited = malloc(gm->size * sizeof(t_bool));
  for (int i = 0; i < gm->size; i++)
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
      for (int i = 0; i < gm->size; i++) {
        if (gm->m[curr][i] == 1)
          stack_push(i, traversal);
      }
    }
  }

  free(visited);
  free_stack(traversal);

  return reverse_stack(path);
}
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

/* CFC SEARCH */
// global
int **graphm_search_cfc(t_graphm *gm) {
  t_bool *marking = malloc(gm->size * sizeof(t_bool));
  for (int i = 0; i < gm->size; i++)
    marking[i] = 0;

  t_bool *order = malloc(gm->size * sizeof(t_bool));

  int step = 0;
  for (int x = 0; x < gm->size; x++)
    if (marking[x] == 0)
      graphm_kosaraju_1_rec(gm, x, marking, order, &step);

  // invsersed graph
  t_graphm *gm_transposed = transpose_graphm(gm);

  // inversed order
  t_bool *inv_order = malloc(gm->size * sizeof(t_bool));
  for (int x = 0; x < gm->size; x++)
    inv_order[(gm->size - 1) - order[x]] = x;

  // CFC
  for (int i = 0; i < gm->size; i++)
    marking[i] = 0;

  unsigned int nb_scc = 0;

  // init
  int **cfc = malloc(gm->size * sizeof(int *));
  for (int i = 0; i < gm->size; i++) {
    cfc[i] = malloc(gm->size * sizeof(int));
    for (int j = 0; j < gm->size; j++) {
      cfc[i][j] = -1;
    }
  }

  for (int i = 0; i < gm->size; i++) {
    int u = inv_order[i];
    if (marking[u] == 0) {
      graphm_kosaraju_2_rec(gm_transposed, u, marking, cfc[nb_scc]);
      nb_scc++;
    }
  }

  free_graphm(gm_transposed);
  free(inv_order);
  free(order);
  free(marking);

  return cfc;
}
int **graphl_search_cfc(t_graphl *gl) {
  t_bool *marking = malloc(gl->size * sizeof(t_bool));
  for (int i = 0; i < gl->size; i++)
    marking[i] = 0;

  t_bool *order = malloc(gl->size * sizeof(t_bool));

  int step = 0;
  for (int x = 0; x < gl->size; x++)
    if (marking[x] == 0)
      graphl_kosaraju_1_rec(gl, x, marking, order, &step);

  // invsersed graph
  t_graphl *gl_transposed = transpose_graphl(gl);

  // inversed order
  t_bool *inv_order = malloc(gl->size * sizeof(t_bool));
  for (int x = 0; x < gl->size; x++)
    inv_order[(gl->size - 1) - order[x]] = x;

  // CFC
  for (int i = 0; i < gl->size; i++)
    marking[i] = 0;

  unsigned int nb_scc = 0;

  // init
  int **cfc = malloc(gl->size * sizeof(int *));
  for (int i = 0; i < gl->size; i++) {
    cfc[i] = malloc(gl->size * sizeof(int));
    for (int j = 0; j < gl->size; j++) {
      cfc[i][j] = -1;
    }
  }

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

// step 1
void graphm_kosaraju_1_rec(t_graphm *gm, int x, int *marking, int *order, int *step) {
  marking[x] = 1;
  for (int y = 0; y < gm->size; y++) {
    if (gm->m[x][y] == 1 && marking[y] == 0)
      graphm_kosaraju_1_rec(gm, y, marking, order, step);
  }
  order[x] = *step;
  (*step)++;
}
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

// step 2
void graphm_kosaraju_2_rec(t_graphm *gm, int x, int *marking, int *curr_cfc) {
  marking[x] = 1;
  int curr_index = 0;
  while (curr_cfc[curr_index] != -1 && curr_index < gm->size)
    curr_index++;
  curr_cfc[curr_index] = x;

  for (int y = 0; y < gm->size; y++) {
    if (gm->m[x][y] == 1 && marking[y] == 0)
      graphm_kosaraju_2_rec(gm, y, marking, curr_cfc);
  }
}
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

void show_cfc(int **cfc, int size, FILE *file, t_bool is_nom, char **map_nom) {
  for (int i = 0; i < size; i++) {
    if (cfc[i][0] == -1)
      continue;
    fprintf(file, "Composante #%d: [", i + 1);
    for (int j = 0; j < size; j++) {
      if (j == size - 1 || cfc[i][j + 1] == -1) {
        if (is_nom)
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

/* CFC Terminal FILTER */

int **cfctm_filter(t_graphm *gm, int **cfc) {
  int **cfct = malloc(gm->size * sizeof(int *));
  for (int i = 0; i < gm->size; i++) {
    cfct[i] = malloc(gm->size * sizeof(int));
    for (int j = 0; j < gm->size; j++) {
      cfct[i][j] = -1;
    }
  }

  for (int i = 0; i < gm->size; i++) {
    if (cfc[i][0] == -1)
      continue;
    for (int j = 0; j < gm->size; j++) {
      if (cfc[i][j] == -1)
        break;

      int curr_cfc_member = cfc[i][j];

      t_bool not_a_cfct = 0;
      for (int k = 0; k < gm->size; k++) {
        if (gm->m[curr_cfc_member][k] == 1) {
          // check if k is in cfc, if no, then it's not a cfct
          t_bool k_in_cfc = 0;
          for (int l = 0; l < gm->size; l++) {
            if (cfc[i][l] == k) {
              k_in_cfc = 1;
              break;
            }
          }

          if (!k_in_cfc) {
            not_a_cfct = 1;
            break;
          }
        }
      }

      if (not_a_cfct) {
        for (int k = 0; k < gm->size; k++)
          cfct[i][k] = -1;
        break;
      } else {
        // we only know that curr_cfc_member is in a potential cfct
        cfct[i][j] = curr_cfc_member;
      }
    }
  }

  return cfct;
}
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
        // check if k is in cfc, if no, then it's not a cfct
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
        // we only know that curr_cfc_member is in a potential cfct
        cfct[i][j] = curr_cfc_member;
      }
    }
  }

  return cfct;
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
