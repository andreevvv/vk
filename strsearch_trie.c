#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

/* const for file validation */
#define MAX_FILE_SIZE     (128L * 1024L * 1024L)
#define INVALID_FILE_SIZE (-1L)

/* const for string validation */
#define MIN_CHAR      (32)
#define MAX_CHAR      (127)
#define CHAR_NEW_LINE ('\n')

// string validation
bool is_valid_str (char *str) {
  char *ptr = NULL;

  for (ptr = str; *ptr; ++ptr) {
    if (*ptr >= MAX_CHAR || *ptr <= MIN_CHAR && *ptr != CHAR_NEW_LINE) {
      return false;
    }
  }
  return true;
}

/* struct for trie tree */
#define CHAR_COUNT ((MAX_CHAR) - (MIN_CHAR))
#define CHAR_INDEX(i) ((i) - (MIN_CHAR))

struct trie_node {
  struct trie_node *next[CHAR_COUNT];
  int              leaf;
};
typedef struct trie_node trie_node_t;

/* trie tree implemenation */
trie_node_t *trie_create_node() {
  int i = 0;
  trie_node_t *node =  malloc( sizeof( trie_node_t));

  if (!node) {
    return NULL;
  }
  node->leaf = false;
  for (; i < CHAR_COUNT; ++i) {
    node->next[i] = NULL;
  }
  return node;
}

void trie_destroy_node (trie_node_t *node) {
  int i = 0;

  for (; i < CHAR_COUNT; ++i) {
    if (node->next[i]) {
      trie_destroy_node( node->next[i]);
    }
  }
  free( node);
}

bool trie_find_node (trie_node_t *tree, char *str, size_t str_len) {
  size_t i = 0;
  int pos = 0;
  trie_node_t *cur = tree;

  if (!cur) {
    return false;
  }
  for (i = 0; i < str_len; ++i) {
    if (str[i] == CHAR_NEW_LINE) {
      continue;
    }
    pos = CHAR_INDEX( str[i]);
    if (!cur->next[pos]) {
      return false;
    }
    cur = cur->next[pos];
  }
  if( cur) {
    return cur->leaf;
  }
  return false;
}

bool trie_insert_node (trie_node_t *tree, char *str, size_t str_len) {
  size_t i = 0;
  int pos = 0;
  trie_node_t *cur = tree;

  if (!cur) {
      return false;
  }
  for (i = 0; i < str_len; ++i) {
    if (str[i] == CHAR_NEW_LINE) {
      continue;
    }
    pos = CHAR_INDEX( str[i]);
    if (!cur->next[pos]) {
       cur->next[pos] = trie_create_node();
      if(!cur->next[pos]) {
        return false;
      }
    }
    cur = cur->next[pos];
  }
  cur->leaf = true;
  return true;
}

/* main function impemenation */
int main (int argc, char **argv) {
  FILE *file = NULL;
  long fsize = 0L;
  char *str = NULL;
  size_t str_len = 0;
  ssize_t str_read = 0;
  unsigned long line_num = 0L;
  trie_node_t *tree = NULL;

  if (argc < 2) {
    printf("Usage: strsearch filename\n");
    return 0;
  }

  file = fopen( argv[1], "r");
  if (!file) {
    printf("Error while opening file: %s\n", argv[1]);
    return 1;
  }

  if (fseek( file, 0, SEEK_END) != 0) {
    printf("Error while seek end file: %s\n", argv[1]);
    fclose( file);
    return 1;
  }

  fsize = ftell( file);
  if (INVALID_FILE_SIZE == fsize) {
    printf("Error file size: %s\n", argv[1]);
    fclose( file);
    return 1;
  }

  if (fsize > MAX_FILE_SIZE) {
    printf("The %s file size %ld bigger than %ld\n", argv[1], fsize, MAX_FILE_SIZE);
    fclose( file);
    return 1;
  }
  rewind( file);

  tree = trie_create_node();
  if (!tree) {
    printf("Error memory allocation\n");
    fclose( file);
    return 1;
  }

  while ((str_read = getline( &str, &str_len, file)) != -1) {
    if (is_valid_str( str)) {
      if (trie_find_node( tree, str, str_read)) {
        printf("YES\n");
      } else {
        printf("NO\n");
        if (!trie_insert_node( tree, str, str_read)) {
          printf("Error memory allocation\n");
          fclose( file);
          return 1;
        }
      }
      /*printf("Line: %lu hash: %" PRIu32 " %s\n", line_num, hash, str);*/
      ++line_num;
    } else {
      printf("Line: %lu the string contains invalid data: %s", line_num, str);
      fclose( file);
      return 1;
    }
  }
  trie_destroy_node( tree);
  fclose( file);
  return 0;
}
