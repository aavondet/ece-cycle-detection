#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#include"eca.h"

static void set_rule_binary(eca* ca);

eca* eca_init(int cellcount, char* init_pattern, unsigned char rule) {
  eca* ca = malloc(sizeof(eca));
  int i;

  if(ca != NULL) {
    *ca = (eca){.cells = malloc(cellcount * sizeof(char)),
                 .next_state = malloc(cellcount * sizeof(char)),
                 .cellcount = cellcount,
                 .rule_binary = "",
                 .rule = rule};
    if (ca->cells != NULL && ca->next_state != NULL) {
      set_rule_binary(ca);
      for (i = 0; i < cellcount; i++) {
        if (init_pattern[i] == '0') {
          ca->cells[i] = '0';
        } else if(init_pattern[i] == '1') {
          ca->cells[i] = '1';
        }
      }
      return ca;
    } else {
      eca_free(ca);
      return NULL;
    }
  } else {
    return NULL;
  }
}

eca* eca_copy(eca* old_ca) {
  eca* ca = malloc(sizeof(eca));
  int i;

  if(ca != NULL) {
    *ca = (eca){.cells = malloc((old_ca->cellcount) * sizeof(char)),
                 .next_state = malloc((old_ca->cellcount) * sizeof(char)),
                 .cellcount = old_ca->cellcount,
                 .rule_binary = "",
                 .rule = old_ca->rule};
    if (ca->cells != NULL && ca->next_state != NULL) {
      set_rule_binary(ca);
      for (i = 0; i < old_ca->cellcount; i++) {
        ca->cells[i] = old_ca->cells[i];
      }
      return ca;
    } else {
      eca_free(ca);
      return NULL;
    }
  } else {
    return NULL;
  }
}

bool eca_equal(eca* ca1, eca* ca2) {
  if (strcmp(ca1->cells, ca2->cells) == 0) {
    return true;
  } else {
    return false;
  }
}

static void set_rule_binary(eca* ca) {
  int p;

  for (p = 0; p <= 7; p++) {
    if((int)(pow(2, p)) & ca->rule) {
      ca->rule_binary[abs(p - 7)] = '1';
    } else {
      ca->rule_binary[abs(p - 7)] = '0';
    }
  }
  ca->rule_binary[8] = '\0';
}

void eca_next(eca* ca) {
  int prev_index;
  int next_index;
  char window[4];
  int i;

  for (i = 0; i < ca->cellcount; i++) {
    if (i == 0) {
      prev_index = ca->cellcount - 1;
    } else {
      prev_index = i - 1;
    }
    if (i == (ca->cellcount - 1)) {
      next_index = 0;
    } else {
      next_index = i + 1;
    }

    window[0] = ca->cells[prev_index];
    window[1] = ca->cells[i];
    window[2] = ca->cells[next_index];
    window[3] = '\0';

    if(strcmp(window, "111") == 0) {
      ca->next_state[i] = ca->rule_binary[0];
    } else if (strcmp(window, "110") == 0) {
      ca->next_state[i] = ca->rule_binary[1];
    } else if (strcmp(window, "101") == 0) {
      ca->next_state[i] = ca->rule_binary[2];
    } else if (strcmp(window, "100") == 0) {
      ca->next_state[i] = ca->rule_binary[3];
    } else if (strcmp(window, "011") == 0) {
      ca->next_state[i] = ca->rule_binary[4];
    } else if (strcmp(window, "010") == 0) {
      ca->next_state[i] = ca->rule_binary[5];
    } else if (strcmp(window, "001") == 0) {
      ca->next_state[i] = ca->rule_binary[6];
    } else if (strcmp(window, "000") == 0) {
      ca->next_state[i] = ca->rule_binary[7];
    }
  }
  for (i = 0; i < ca->cellcount; i++) {
    ca->cells[i] = ca->next_state[i];
  }
}

void eca_free(eca* ca) {
  if (ca != NULL) {
    if (ca->cells != NULL) {
      free(ca->cells);
    }
    if (ca->next_state != NULL) {
      free(ca->next_state);
    }
    free(ca);
  }
}

