#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ================== DEFINITIONS ==================

int* GPtr;

void *thread(void *x) {
	*(int *)(x) = 222;
	return NULL;
}

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

__attribute_noinline__ void SCC_foo(int *x) {
	*x = 111;
	SCC_bar(x);
}

__attribute_noinline__ void SCC_bar(int *x) {
	*x = 222;
	SCC_buz(x);
}

__attribute_noinline__ void SCC_buz(int *x) {
	*x = 333;
	SCC_foo(x);
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