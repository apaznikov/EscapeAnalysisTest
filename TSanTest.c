// #include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *GPtr;

// int *mem;

void func() {
  int *mem = malloc(100);
  // GPtr = mem;

  mem[33] = 444;

  // int x;
  // GPtr = &x;
  // int *alias = &x;
  // *alias = 555;
}

/*
void func_to_instr() {
  int x;
  GPtr = &x; // Escape
  // x = 111;

  // int y;
  // y = 222;

  int *xPtr = &x;
  *xPtr = 333;

  // int *yPtr = &y;
  // *yPtr = 444;

  // int *y = &x[55];
  // printf("%p\n", y);
  // int LV;
  // printf("%p\n", &LV);
}
*/