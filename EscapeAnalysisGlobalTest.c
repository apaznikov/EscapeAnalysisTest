#include <stdio.h>
#include <stdlib.h>

// ================== DEFINITIONS ==================

int* GPtr;

//===----------------------------------------------------------------------===//
// First test
//===----------------------------------------------------------------------===//

void callee1_level2(int *x) {
	GPtr = x;
}

void callee1_level1(int *x) {
	callee1_level2(x);
}

void callee2_level1(int *x) {
	*x = 333;
}

void parent() {
	int x;
	callee1_level1(&x);
	callee2_level1(&x);
}