#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#ifndef UTILS_H

char *pad_right(char *value, int length, char padding);
char *pad_left(char *value, int length, char padding);

bool is_char_numeric(char value);

bool is_string_numeric(char *value);

char *get_string_by_index_within_delimeters(char *source, char delimeter,
                                            int index);

#endif
#define UTILS_H