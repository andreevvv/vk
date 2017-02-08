#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

static const long MAX_FILE_SIZE = 128L * 1024L * 1024L;
static const long INVALID_FILE_SIZE = -1L;

static const char MIN_CHAR = 32;
static const char MAX_CHAR = 127;
static const char CHAR_NEW_LINE = '\n';

bool is_valid_str (char *str) {
  char *ptr;

  for (ptr = str; *ptr; ++ptr) {
    if (*ptr >= MAX_CHAR || *ptr <= MIN_CHAR && *ptr != CHAR_NEW_LINE) {
      return false;
    }
  }
  return true;
}


uint32_t jenkins_one_at_a_time_hash (char *key, size_t len) {
  uint32_t hash, i;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
}

struct avl_node {
  struct avl_node *left;
  struct avl_node *right;
  uint32_t        hash;
};
typedef struct avl_node avl_node_t;

struct avl_tree {
  struct avl_node *root;
};
typedef struct avl_tree avl_tree_t;

avl_tree_t *avl_create_tree() {
  avl_tree_t *tree = malloc( sizeof(avl_tree_t));

  if (!tree) {
    return NULL;
  }
  tree->root = NULL;
  return tree;
}

avl_node_t *avl_create_node() {
  avl_node_t *node =  malloc( sizeof( avl_node_t));

  if (!node) {
    return NULL;
  }
  node->left = NULL;
  node->right = NULL;
  node->hash = 0;
  return node;
}

void avl_destroy_node(avl_node_t *node) {
  if (!node) {
    return;
  }
  avl_destroy_node( node->left);
  avl_destroy_node( node->right);
  free( node);
}

void avl_destroy_tree (avl_tree_t *tree) {
    avl_node_t *cur = tree->root;
    if (cur) {
      avl_destroy_node( cur);
    }
}

avl_node_t *avl_balance_node (avl_node_t *node) {
  avl_node_t *root = NULL;
  if (node->left) {
    node->left  = avl_balance_node( node->left );
  }
  if (node->right) {
    node->right = avl_balance_node( node->right );
  }
  return root;
}

void avl_balance_tree (avl_tree_t *tree) {
  avl_node_t *root  = avl_balance_node( tree->root);
  if (root != tree->root) {
    tree->root = root;
  }
}

bool avl_insert_node (avl_tree_t *tree, uint32_t hash) {
  avl_node_t *next = NULL;
  avl_node_t *last = NULL;
  avl_node_t *node = avl_create_node();
  if (!node) {
    return false;
  }

  if (!tree->root) {
    node->hash = hash;
    tree->root = node;
  } else {
    next = tree->root;
    while (next) {
      last = next;
      if (hash < next->hash ) {
        next = next->left;
      }
      else if (hash > next->hash ) {
             next = next->right;
      }
    }
    node->hash = hash;
    if (hash < last->hash) {
      last->left = node;
    }
    if (hash > last->hash) {
      last->right = node;
    }
  }
  avl_balance_tree( tree);
  return true;
}

avl_node_t *avl_find_node (avl_tree_t *tree, uint32_t hash) {
  avl_node_t *cur = tree->root;

  while (cur && cur->hash != hash) {
    if (hash > cur->hash) {
      cur = cur->right;
    }
    else {
      cur = cur->left;
    }
  }
  return cur;
}

int main (int argc, char** argv) {
    FILE *file = NULL;
    long fsize = 0L;
    char *str = NULL;
    size_t str_len = 0;
    ssize_t str_read = 0;
    unsigned long line_num = 0L;
    uint32_t hash = 0;
    avl_tree_t *tree = NULL;

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

    tree = avl_create_tree();
    if(!tree) {
      printf("Error memory allocation\n");
      return 1;
    }

    while ((str_read = getline( &str, &str_len, file)) != -1) {
      if (is_valid_str( str)) {
        hash = jenkins_one_at_a_time_hash( str, str_len);
        if (avl_find_node( tree, hash) ) {
          printf("YES\n");
        } else {
          printf("NO\n");
          avl_insert_node( tree, hash);
        }
        /*printf("Line: %lu hash: %" PRIu32 "\n", line_num, hash);*/
        /*++line_num;*/
      } else {
        printf("Line: %lu the string contains invalid data: %s", line_num, str);
        fclose( file);
        return 0;
      }
    }

    avl_destroy_tree( tree);
    fclose( file);
    return 0;
}
