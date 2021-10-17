#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

typedef struct eca {
  char* cells;
  char* next_state;
  int cellcount;
  unsigned char rule;
  char rule_binary[9];
} eca;

eca* eca_init(int cellcount, char* init_pattern, unsigned char rule);
eca* eca_copy(eca* ca);
void eca_next(eca* ca);
void eca_free(eca* ca);
bool eca_equal(eca* ca1, eca* ca2);
