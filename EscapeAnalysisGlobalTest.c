#include <stdio.h>
#include <stdlib.h>

// ================== DEFINITIONS ==================

int* GPtr;

//===----------------------------------------------------------------------===//
// Recursive
//===----------------------------------------------------------------------===//

/*
// Simple recursive function
void rec_func(int *x) {
	if (*x) {
		(*x)--;
		rec_func(x);
	}
}

// SCC of 3 mutually recursive functions
void SCC_foo(int *x);
void SCC_bar(int *x);
void SCC_buz(int *x);

void SCC_foo(int *x) {
	SCC_bar(x);
}

void SCC_bar(int *x) {
	SCC_buz(x);
}

void SCC_buz(int *x) {
	SCC_foo(x);
}
*/

//===----------------------------------------------------------------------===//
// First test
//===----------------------------------------------------------------------===//

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