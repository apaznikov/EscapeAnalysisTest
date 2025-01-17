#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// ================== DEFINITIONS ==================

int* GPtr;
int **GPtrPtr;

// struct StructTy { int x, y; };

//===----------------------------------------------------------------------===//
// Check pointer strategies
//===----------------------------------------------------------------------===//

/*
int **GPtrPtr;

void foo() {
  int x;
  // int *xa = &x;
  int *xa2 = &x;

  // int *xa2 = xa;
  // int **xaa = &xa;

  int **xx = &x;

  // GPtrPtr = (*xx) + 33;
  GPtr = xa2;
  // GPtrPtr = &xa2;

  // int *xAlias2 = xAlias;
  // GPtr = xAlias;
}
*/

//===----------------------------------------------------------------------===//
// New argument strategy for static functions
//===----------------------------------------------------------------------===//
/*
struct StructTy { int *x; };

static void level2_func1(int *x) {
  GPtr = x; // Escape through GPtr
}

static void level2_func2(int *x) { *x = 333; }

static void level1_func1(int *x) { level2_func1(x); }

static void level1_func2(int *x) {
  *x = 333;
  int *p = x;
  printf("%p\n", &p); // Escape through external func call
}

static void level1_func3(int *x, int *y, int *z) {
  *x = 42;
  level2_func2(x);  // x doesn't escape level2_func2
  level2_func2(y);  // p doesn't escape in level2_func2
  level2_func1(y);  // p escapes in level2_func1

  struct StructTy S1;
  // S1 doesn't escape, because x doesn't escape
  S1.x = x;

  struct StructTy S2;
  // S2 escapes because x escapes (bottom-top)
  S2.x = y;

  struct StructTy S3;
  // S3 escapes because z escapes (from top-bottom)
  S3.x = z;
}

void external_func(int *x);

void parent_func() {
  int p;
  level1_func1(&p);
  level1_func2(&p);

  int x, y, z;
  external_func(&z);

  level1_func3(&x, &y, &z);
}
*/

/*
static void level1_func3(int **x, int **y, int **z) {
  int xLocal, yLocal, zLocal;
  *x = &xLocal;
  *y = &yLocal;
  *z = &zLocal;

  struct StructTy S3;
  // S3 escapes because z escapes (from top-bottom)
  S3.x = *z;
}

void parent_func() {
  int *x, *y, *z;
  GPtrPtr = &z;
  level1_func3(&x, &y, &z);

  struct StructTy S3;
  // S3 escapes because z escapes (from top-bottom)
  S3.x = z;
}
*/

/*
void foo() {
  // int x[10];
  // GPtr = &x[3];
  // int *y = &x[5];

  int x;
  GPtr = &x;
  int *y = &x + 5;
  if (rand())
    int *z = y;
  int *p = y + 55;
}
*/

// void func(int *x) {
  // int y = *x;
  // printf("%d\n", y);
// }

// void func(int *x) {
	// int *Alias = x + 1;
	// *Alias = 333;
	// int y;
	// GPtr = &y;
// }

//===----------------------------------------------------------------------===//
// Returning value
//===----------------------------------------------------------------------===//

/*
int *ret_non_escaping_value() {
  int x;
  return &x;
}

int *ret_escaping_value() {
  int y;
  GPtr = &y;
  return &y;
}

void func() {
  int *x = ret_non_escaping_value();
  int *y = ret_escaping_value();
  int *z = malloc(sizeof(int));
  int *p = realloc(NULL, sizeof(int));
}
*/

//===----------------------------------------------------------------------===//
// Aliasing to function arguments
//===----------------------------------------------------------------------===//

/*
void use_esc_func(int *x, int *y) {
  // GPtr is escaping, but it's obvious, not print
  printf("%p", GPtr);
  // GPtr is escaping, but it's obvious, not print
  GPtr = GPtr;
  // x escapes here
  printf("%p", x);
}
*/

/*
static void func_with_ptr_arg(int *x, int *y) {
  int *Alias = x;
  if (rand())
    GPtr = Alias;
  GPtr = y;
}

void caller() {
  int x, y;
  func_with_ptr_arg(&x, &y);
}
*/

/*
// arg x should not escape here in BB entry
static void func_with_ptr_arg(int *x) {
  if (rand())
    GPtr = x;
}

void caller() {
  int x;
  func_with_ptr_arg(&x);
}
*/

/*
void args_are_aliases(int *ArgX, int *ArgY) {
	int *k;
	// int *x = k;
	k = ArgX;
	if (rand()) {
		k = ArgX;
	}
	GPtr = ArgX;
}

void caller() {
	int x, y;
	args_are_aliases(&x, &y);
}
*/

//===----------------------------------------------------------------------===//
// SCC 2
//===----------------------------------------------------------------------===//

static void func2(int *x, int *y, int *z);

static void func1(int *x, int *y, int *z, int *NoEsc) {
  if (rand())
    GPtr = x;
  printf("%p", y);
  func2(x, y, z);
}

static void func2(int *x, int *y, int *z) {
  int *NoEsc;
  if (rand())
    func1(x, y, z, NoEsc);
  printf("%p\n", z);
}

void caller() {
  int x, y, z, NoEsc;
  func1(&x, &y, &z, &NoEsc);
}

/*
void alias_of_ptr_arg(int *PtrArg) {
        int *Alias = PtrArg;
        printf("%p\n", PtrArg);
}
*/

/*
void escaping_arg(int *Arg1, int *Arg2) {
  int *x = Arg1;
  if (rand())
    GPtr = x;
}

void escaping_arg_caller() {
  int x, y;
  escaping_arg(&x, &y);
}
*/

//===----------------------------------------------------------------------===//
// Function returning address
//===----------------------------------------------------------------------===//

/*
void *thread_func(void *data) {
  int *x = (int *)data;
  *x = 111;
}

void race() {
  int data = 0;
  pthread_t t;
  pthread_create(&t, NULL, thread_func, &data);
  data = 111;
  pthread_join(t, NULL);
}
*/

//===----------------------------------------------------------------------===//
// Function returning address
//===----------------------------------------------------------------------===//

/*
int *ret_ptr() {
	return GPtr;
}

void use_ret_ptr() {
	int *p = ret_ptr();
	*p = 555;
}
*/

//===----------------------------------------------------------------------===//
// Recursive
//===----------------------------------------------------------------------===//

/*
// Simple recursive function
static void rec_func(int **x, int **y, int **z) {
  int xLocal;
  *x = &xLocal;

  if (rand()) {
    rec_func(x, y, z);
  }
  GPtrPtr = x;
}

void caller() {
  int *x, *y, *z;
  rec_func(&x, &y, &z);
}
*/

// SCC of 3 mutually functions
/*
void bar(int *x);

void foo(int *x) {
	int *xAlias = x;
	if (rand())
		bar(x);
}

void bar(int *x) {
	GPtr = x;
	foo(x);
}
*/

/*
void SCC_foo(int *x, int *y, int *z, int *p, int *NoEsc);
void SCC_bar(int *x, int *y, int *z, int *p, int *NoEsc);
void SCC_buz(int *x, int *y, int *z, int *p, int *NoEsc);

__attribute_noinline__ void SCC_foo(int *x, int *y, int *z, int *p, int *NoEsc) {
	GPtr = x;
	SCC_bar(x, y, z, p, NoEsc);
}

__attribute_noinline__ void SCC_bar(int *x, int *y, int *z, int *p, int *NoEsc) {
	GPtr = y;
	int *xAlias = x;
	int *yAlias = y;
	int *zAlias = z;
	if (rand())
		GPtr = p;
	SCC_buz(x, y, z, p, NoEsc);
}

__attribute_noinline__ void SCC_buz(int *x, int *y, int *z, int *p, int *NoEsc) {
	GPtr = z;
	SCC_foo(x, y, z, p, NoEsc);
}
*/

//===----------------------------------------------------------------------===//
// First test
//===----------------------------------------------------------------------===//

/*
void level2_func1(int *x) {
  GPtr = x; // Escape through GPtr
}

void level2_func2(int *x) { *x = 333; }

void level1_func1(int *x) { level2_func1(x); }

void level1_func2(int *x) {
  *x = 333;
  int *p = x;
  printf("%p\n", &p); // Escape through external func call
}

void level1_func3(int *x, int *p) {
  *x = 42;
  level2_func2(x);
  level2_func1(p);
}

void external_func(int *x);

void parent_func1() {
  int x;
  level1_func1(&x);
  level1_func2(&x);
  int y;
  external_func(&y);
  int z, p;
  level1_func3(&z, &p);
}
*/