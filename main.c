#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// ======= DEFINITIONS =======

// Global variable to demonstrate escape
int* GPtr;

int GV;

// Structure to demonstrate field escape
typedef struct {
  int *field;
} StructTy;

typedef struct { int Field1, Field2; } StructWithoutPointers;
void *Ptr;

int GArr[10];

int **DoubleGPtr;

void escape_through_argument_aliasing(int *k) {
	int *p = k;
}

void escape_through_gptr_aliasing() {
	int *GPtrAlias = GPtr;
}

/*
void bar(int **p) {
	**p = 222;
}
*/

/*
void escape_address_of_pointer() {
	int x = 111;
	int *p = &x;
	bar(&p);
	printf("%d\n", x);
}
*/

/*
int main() {
	escape_address_of_pointer();
	return 0;
}
*/

/*
void mutual_aliases_cond() {
	int x;
	int *a1, *a2;
	if (rand()) {
		a1 = a2;
		GPtr = a2;
	} else {
		a1 = &x;
		x = 999;
	}
}
*/

/*
void mutual_aliases_loop() {
	int x;
	int *a1, *a2;
	while (rand()) {
		if (rand()) {
			a1 = a2;
			GPtr = a2;
		} else {
			a1 = &x;
			x = 999;
		}
	}
}
*/

/*
void mutual_aliases() {
	int x;
	int *a1, *a2;
	a1 = a2;
	a1 = &x;
	GPtr = a2;
}
*/

// void func(int v) {}

/*
int *ret_ptr() {
	int x = 0;
	return &x;
}

int ret_val() {
	int x = 0;
	return x;
}
*/

/*
int passing_value_is_not_aliasing() {
	int x;
	int *k = &x;
	int y = x;
	GPtr = &y;
	return y;
}
*/

/*
int passing_value_of_aliases_is_not_escape() {
	int x;
	int *Alias = &x;
	GV = *Alias;
	func(*Alias);
	return *Alias;
}
*/

/*
int passing_value_is_not_escape() {
	int x;
	GV = x;
	func(x);
	return x;
}
*/

/*
void passing_by_value_is_not_escaping() {
	int x;
	GV = x;

	int *p;
	GPtr = p;

	int y;
	GPtr = &y;
}
*/

/*
// TODO
void escape_to_global_array_element() {
	int x;
	GArr[5] = x;
}
*/

/*
void external_func(int* ptr) {
}

void escaping_func_arguments(int x, int y, int *z) {
	GPtr = &x;
	external_func(&y);
}
*/

/*
typedef struct S { char x; char *p; } STy;
STy GS;

int foo() {
	int x = 0;
	x++;
	STy S1, S2;
	S1 = S2;
	return x;
}
*/

/*
void escape_pointee_object() {
	int x[10];
	int *p = &x[5];
	GPtr = &x[2];
}
*/

/*
void escape_in_the_middle_of_alias_chain() {
	int x[10];
	int *y = &x[3];
	int *z = &y[4];
	GPtr = y;
}
*/

/*
void global_variable_as_alias_in_GEP() {
	__atomic_store_n(&GPtr[0], 333, __ATOMIC_RELEASE);
}

__attribute__((no_sanitize("thread")))
void *func() { return NULL; }

void foo() {
	void *p = func();
}
*/

// ====================================

/*
void *VoidGPtr = NULL;

void print_address(const char *str, int n, ...) {
	va_list ap;
	void *p;
	while (n--)
		p = va_arg(ap, void *);
	VoidGPtr = p;
}
*/

//===----------------------------------------------------------------------===//
// Structures
//===----------------------------------------------------------------------===//

/*

void test() {
	StructWithoutPointers SWP;
	int FieldVal = SWP.Field1;
	int *FieldPtr = &SWP.Field1;
	Ptr = FieldPtr;
}
*/

/*
// Escape of a structure field
StructTy* escape_struct_field() {
  StructTy* s;
  int x;
  s->field = &x;
  return s;
}
*/

/*
typedef struct { int *p1, *p2, *p3; } StructWithManyPtrs;

void func(StructWithManyPtrs S) {}

void multiple_aliases_via_struct() {
	int x, y, z;
	StructWithManyPtrs S;
	S.p1 = &x;
	S.p2 = &y;
	S.p3 = &z;
	func(S);
}
*/

//===----------------------------------------------------------------------===//
// Escape via memcpy intrinsic
//===----------------------------------------------------------------------===//

/*
void escape_memcpy() {
	StructTy S1, S2;
	S2 = S1;
	GPtr = S2.field;
}
*/

/*
// --------- Escape Cases --------

void escape_global_variable() {
	int x;
	GV = x;
}

// Returning the address of a local variable
int* escape_local() {
  int x;
  return &x;
}

// Assigning the address of a local variable to a global variable
void escape_global() {
  int x = 20;
  global_ptr = &x;
}


// Passing the address of a local variable to a function
int* escape_func() {
  int x = 30;
  external_func(&x);
  return &x;
}

// Escape by returning pointer
int *escape_by_returning_ptr() {
	int *x;
	return x;
}
*/


/*
// Escape through aliasing
int* escape_aliasing() {
  int x = 50;
  int *Alias = &x;
  GPtr = Alias;  // Now global_ptr points to x through alias
  return &x;
}
*/

/*

// --- Non-Escape Cases ---

// Local variable without passing its address
void no_escape_local() {
  int x = 40;
}

// Passing a copy of a variable
void no_escape_arg(int x) { }


void no_escape_memcpy() {
	StructWithoutPointers S1, S2;
	S2 = S1;
	GPtr = &S2.Field1;
}
*/

//===----------------------------------------------------------------------===//
// Escape of nested structures
//===----------------------------------------------------------------------===//
/*

typedef struct {
	int *Ptr;
} InnerStructTy;

typedef struct {
	InnerStructTy Inner;
} OuterStructTy;

OuterStructTy *GS;

void escape_nested_struct() {
	int x = 333;
	InnerStructTy InnerS = { &x };
	OuterStructTy OuterS = { InnerS };
	GS = &OuterS; // Escape due to nested structure
}
*/

/*

void multiple_aliases() {
	int x, y, z;
	int *Ptr;
	Ptr = &x;
	Ptr = &y;
	Ptr = &z;
	GPtr = Ptr;
}
*/

/*
struct Point {
	int x;
	int y;
};

int getSum(struct Point p) {
	return p.x + p.y;
}

void func() {
	struct Point p = {0, 0};
	getSum(p);
}
*/

/*
void func(int flag) {
	int a = 0, b = 0, c = 0;
	if (flag) {
		GV = a;
	} else {
		GV = b;
	}
	c = b;
}
*/

/*
int test(int x) { return GV; }

void func_loop() {
	int a = 0, b = 0, c = 0;
	a = b = c = 0;

	if (test(c)) {
		do {
			if (rand() > 100) {
				GV = b;
				return;
			}
			a++;
		} while (test(a) < 10);
	}
}
*/

/*
#define N 10

void escape_in_loop() {
	int i;
	for (i = 0; i < N; i++) {
		int x;
		if (i == 5)
			GPtr = &x; // Escape within loop
		else
			GPtr = &i;
	}
}
*/

/*
void escape_in_conditional() {
	int x;
	if (GPtr == NULL)
		GPtr = &x; // Escape within conditional
}
*/
