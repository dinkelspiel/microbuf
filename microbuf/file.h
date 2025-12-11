#include <stdio.h>
#include <stdlib.h>

#ifndef FILE_H

typedef struct {
  char *contents;
  long length;
} FileContents;

FileContents *read_file(char *path);

#endif
#define FILE_H