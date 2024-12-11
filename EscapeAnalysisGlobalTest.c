#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// ================== DEFINITIONS ==================

int* GPtr;

//===----------------------------------------------------------------------===//
// Aliasing to function arguments
//===----------------------------------------------------------------------===//

/*
void *func_with_ptr_arg(int *x, int *y) {
	int *Alias = x;
	*Alias = 333;
	if (rand())
		GPtr = x;
	GPtr = y;
	return NULL;
}
*/

//===----------------------------------------------------------------------===//
// Function returning address
//===----------------------------------------------------------------------===//

// void race() {
	// int data = 0;
	// pthread_t t;
	// pthread_create(&t, NULL, thread, &data);
	// data = 111;
	// pthread_join(t, NULL);
// }

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

// Simple recursive function
void rec_func(int *x, int *y, int *z) {
	if (rand()) {
		int *Alias = y;
		*Alias = 333;
	} else {
		rec_func(x, y, z);
	}
	int *Alias2 = x;
	GPtr = Alias2;
	GPtr = y;
}

/*
// SCC of 3 mutually functions
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
	GPtr = x;											// Escape through GPtr
}

void level2_func2(int *x) {
	*x = 333;
}

void level1_func1(int *x) {
	level2_func1(x);
}

void level1_func2(int *x) {
	*x = 333;
	int *p = x;
	printf("%p\n", &p);			// Escape through external func call
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