#include "utils.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

char *pad_right(char *value, int length, char padding) {
  int value_length = strlen(value);

  // Don't do any processing if the desired length is below the values length
  if (length <= value_length)
    return value;

  char *padded_value = (char *)malloc(length + 1);
  for (int i = 0; i < length; i++) {
    if (i < value_length) {
      padded_value[i] = value[i];
    } else {
      padded_value[i] = padding;
    }
  }
  padded_value[length] = '\0';

  return padded_value;
}

char *pad_left(char *value, int length, char padding) {
  int value_length = strlen(value);

  // Don't do any processing if the desired length is below the values length
  if (length <= value_length)
    return value;

  char *padded_value = (char *)malloc(length + 1);
  for (int i = 0; i < length; i++) {
    if (i < length - value_length) {
      padded_value[i] = padding;
    } else {
      padded_value[i] = value[i - (length - value_length)];
    }
  }
  padded_value[length] = '\0';
  return padded_value;
}

// Theres probably some smart c way to do this by checking ascii codes but im
// tired
bool is_char_numeric(char value) {
  switch (value) {
  case '0':
    return true;
  case '1':
    return true;
  case '2':
    return true;
  case '3':
    return true;
  case '4':
    return true;
  case '5':
    return true;
  case '6':
    return true;
  case '7':
    return true;
  case '8':
    return true;
  case '9':
    return true;
  default:
    return false;
  }
}

bool is_string_numeric(char *value) {
  int length = strlen(value);

  for (int i = 0; i < length; i++) {
    if (!is_char_numeric(value[i]))
      return false;
  }
  return true;
}

char *get_string_by_index_within_delimeters(char *source, char delimeter,
                                            int index) {
  int length = strlen(source);
  int line = 1;

  char *result = NULL;

  for (int i = 0; i < length; i++) {
    if (source[i] == delimeter) {
      line++;
      continue;
    }

    if (line > index)
      break;

    if (line == index) {
      if (result == NULL) {
        result = malloc(2);
        result[0] = source[i];
        result[1] = '\0';
      } else {
        int result_length = strlen(result);
        result = realloc(result, result_length + 2);
        result[result_length] = source[i];
        result[result_length + 1] = '\0';
      }
    }
  }

  return result;
}