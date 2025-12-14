#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Car {
  char *make;
  char *color;
} Car;

typedef struct Person {
  char *name;
  int id;
  char *email;
  Car *car;
} Person;

typedef struct {
  char *bytes;
  int pointer;
} ByteWriter;

ByteWriter *mb_new() {
  ByteWriter *writer = malloc(sizeof(ByteWriter));
  writer->bytes = malloc(128);
  writer->pointer = 0;
  return writer;
}

void mb_write_byte(ByteWriter *writer, char value) {
  writer->bytes[writer->pointer] = value;
  writer->pointer++;
}

void mb_write_int(ByteWriter *writer, int value) {
  writer->bytes[writer->pointer + 0] = (value >> 24) & 0xFF;
  writer->bytes[writer->pointer + 1] = (value >> 16) & 0xFF;
  writer->bytes[writer->pointer + 2] = (value >> 8) & 0xFF;
  writer->bytes[writer->pointer + 3] = (value) & 0xFF;

  writer->pointer += 4;
}

void mb_write_bytearray(ByteWriter *writer, char *value, int length) {
  mb_write_int(writer, length);
  for (int i = 0; i < length; i++) {
    mb_write_byte(writer, value[i]);
  }
}

void mb_write_string(ByteWriter *writer, char *value) {
  int length = strlen(value);
  mb_write_int(writer, length);
  for (int i = 0; i < length; i++) {
    mb_write_byte(writer, value[i]);
  }
}

ByteWriter *car_encode(Car car) {
  ByteWriter *writer = mb_new();
  mb_write_byte(writer, 1);
  mb_write_string(writer, car.make);
  mb_write_string(writer, car.color);

  return writer;
}

ByteWriter *person_encode(Person person) {
  ByteWriter *writer = mb_new();
  mb_write_byte(writer, 1);

  mb_write_string(writer, person.name);
  mb_write_int(writer, person.id);
  mb_write_string(writer, person.email);

  if (person.car == NULL) {
    mb_write_byte(writer, 0);
  } else {
    mb_write_byte(writer, 1);
    ByteWriter *writer_car = car_encode(*person.car);
    mb_write_bytearray(writer, writer_car->bytes, writer_car->pointer - 1);
  }

  return writer;
}

Person person_decode(char bytes[]) {
  size_t bytes_size = sizeof(bytes);

  char version = bytes[0];
  if (version != 1) {
    printf("version mismatch between decoder and buffer, errors might occur\n");
  }
}