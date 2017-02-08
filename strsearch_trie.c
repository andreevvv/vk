#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

// const for file validation
static const long MAX_FILE_SIZE = 128L * 1024L * 1024L;
static const long INVALID_FILE_SIZE = -1L;

// const for string validation
static const char MIN_CHAR = 32;
static const char MAX_CHAR = 127;
static const char CHAR_NEW_LINE = '\n';

// string validation
bool is_valid_str (char *str) {
  char *ptr;

  for (ptr = str; *ptr; ++ptr) {
    if (*ptr >= MAX_CHAR || *ptr <= MIN_CHAR && *ptr != CHAR_NEW_LINE) {
      return false;
    }
  }
  return true;
}

// TODO: add trie implementaion here

// main function impemenation
int main (int argc, char **argv) {
  FILE *file = NULL;
  long fsize = 0L;
  char *str = NULL;
  size_t str_len = 0;
  ssize_t str_read = 0;
  unsigned long line_num = 0L;

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

  // TODO: add create trie hare

  while ((str_read = getline( &str, &str_len, file)) != -1) {
    if (is_valid_str( str)) {
      // TODO: add find and insert in trie here
      //printf("YES\n");
      //printf("NO\n");
      /*printf("Line: %lu hash: %" PRIu32 " %s\n", line_num, hash, str);*/
      ++line_num;
    } else {
      printf("Line: %lu the string contains invalid data: %s", line_num, str);
      fclose( file);
      return 1;
    }
  }

  fclose( file);
  return 0;
}
