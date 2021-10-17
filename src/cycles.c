#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"eca.h"

typedef struct CycleRes {
  unsigned int transient;
  unsigned int period;
  double time;
} CycleRes;

CycleRes* find_cycle_floyd(eca* ca) {
  clock_t start, end;
  double time;
  unsigned int t, p;
  eca *u, *v, *tmp;
  CycleRes* res = malloc(sizeof(CycleRes));
  start = clock();

  // Find Floyd time
  u = eca_copy(ca);
  eca_next(u);
  v = eca_copy(u);
  eca_next(v);
  while (!eca_equal(u, v)) {
    eca_next(u);
    eca_next(v);
    eca_next(v);
  }
  eca_free(u);

  // Find transient
  t = 0;
  u = eca_copy(ca);
  while (!eca_equal(u, v)) {
    eca_next(u);
    eca_next(v);
    t += 1;
  }

  // Find period
  p = 1;
  eca_next(v);
  while (!eca_equal(u, v)) {
    eca_next(v);
    p += 1;
  }

  // Format result
  end = clock();
  time = ((double) (end - start)) / CLOCKS_PER_SEC;
  res->time = time;
  res->transient = t;
  res->period = p;

  // Free memory
  eca_free(u);
  eca_free(v);
  return res;
}

CycleRes* find_cycle_brent(eca* ca) {
  clock_t start, end;
  double time;
  unsigned int t, p, power, i;
  eca *u, *v, *tmp;
  CycleRes* res = malloc(sizeof(CycleRes));
  start = clock();

  // Find period
  power = 1;
  p = 1;
  u = eca_copy(ca);
  v = eca_copy(ca);
  eca_next(v);
  while (!eca_equal(u, v)) {
    if (power == p) {
      eca_free(u);
      u = eca_copy(v);
      power *= 2;
      p = 0;
    }
    eca_next(v);
    p += 1;
  }
  eca_free(u);
  eca_free(v);

  // Find transient
  u = eca_copy(ca);
  v = eca_copy(ca);
  for (i = 0; i < p; i++) {
    eca_next(v);
  }
  t = 0;
  while (!eca_equal(u, v)) {
    eca_next(u);
    eca_next(v);
    t += 1;
  }

  // Format result
  end = clock();
  time = ((double) (end - start)) / CLOCKS_PER_SEC;
  res->time = time;
  res->transient = t;
  res->period = p;

  // Free memory
  eca_free(u);
  eca_free(v);
  return res;
}

int main() {
  CycleRes *floyd_res, *brent_res;
  int cellcount = 31;
  char init[] = "1110110110101010101010010101111";
  int rule = 30;
  eca* ca = eca_init(cellcount, init, rule);

  floyd_res = find_cycle_floyd(ca);
  brent_res = find_cycle_brent(ca);

  printf("----------------------------------------\n");
  printf("Initial vector %s\n", init);
  printf("Length %d\n", cellcount);
  printf("ECA %d\n", rule);
  printf("----------------------------------------\n");
  printf("Floyd's Algorithm:\n");
  printf("Transient: %u Period: %u Time: %f sec\n", floyd_res->transient, floyd_res->period, floyd_res->time);
  printf("Brent's Algorithm:\n");
  printf("Transient: %u Period: %u Time: %f sec\n", brent_res->transient, brent_res->period, brent_res->time);
}
