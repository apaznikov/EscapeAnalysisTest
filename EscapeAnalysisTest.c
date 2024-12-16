#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// ================== DEFINITIONS ==================

// Global pointer to demonstrate escape through assignment to global pointer
int* GPtr;

// Structure to demonstrate field escape
typedef struct {
  int *field;
} StructTy;

typedef struct { int Field1, Field2; } StructWithoutPointers;
void *Ptr;

#define N 10

int GV;
int GArr[N];
int **DoubleGPtr;

void test() {
	int x;
	GPtr = &x;
}

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
	printf("%p\n", p);

	// GPtr = ret_ptr();

	// int *q = GPtr;
	// *q = 666;
}
*/

//===----------------------------------------------------------------------===//
// Atomic
//===----------------------------------------------------------------------===//

// _Atomic(int*) p;
// void atomic() {
	// int *pp = __c11_atomic_load(&p, __ATOMIC_RELAXED);
	// *pp = 444;
// }

//===----------------------------------------------------------------------===//
// Casts
//===----------------------------------------------------------------------===//

//void ptrtoint_and_inttoptr() {
//	int y;
//	int *x = (int *)(long int)&y;
//	GPtr = x;
//}

//void func2() {
//	float *y;
//	int *x = (int*) y;
//	GPtr = x;
//}

//struct S {
//	int *x;
//};

//void function_pointer_cast() {
//	struct S S1;
//	int x;
//	S1.x = &x;
//	int *p = S1.x;
//	GPtr = p;
//}

//void cast_to_array_type() {
//	int x;
//	int arr[5] = (int)&x;  // Cast to array type
//	GPtr = (int *)arr; // Escape, pointer arithmetic on arr could access x
//}

//void truncate() {
//	long long x;
//	int *y = (int *)x;   // Truncation, like trunc
//	GPtr = y;  // Potential escape, especially if *y is modified
//}

//void cast_with_arithmetic() {
//	int x;
//	long long *y = (long long *)&x;
//	y++; // Pointer arithmetic after cast can be tricky for escape analysis
//	GPtr = (int *)y; // Likely escapes
//}

//void inttoptr_cast() {
//	double *x;
//	GPtr = (int *)x;
//}

//void inttoptr_cast() {
//	long int x;
//	GPtr = (int *)x;
//}


//===----------------------------------------------------------------------===//
// PHIs
//===----------------------------------------------------------------------===//

/*
void func(int **ptr);

void nested_phi() {
	int x, y, z;
	int *p = malloc(sizeof(int)), *q = &z;
	func(&q);
	int *pqAlias = NULL;

	if (rand()) {
		if (rand()) {
			p = &x;
		} else {
			p = &y;
		}

		pqAlias = p;
	} else {
		pqAlias = q;
	}
	int *pqAliasAlias = pqAlias;
	func(&pqAliasAlias);		// to supress pAlias propagation
}
*/

/*
void aliasing_phi() {
	int x, y;
	int *p;
	int *pAlias = NULL;

	if (GV) {
		p = &x;
	} else {
		p = &y;
	}

	// p = phi(x, y)

	// Phi seems to be a pointee in this aliasing
	// but real pointees are x and y
	pAlias = p;

	func(&pAlias);		// to supress pAlias propagation
	GPtr = pAlias;
}
*/

/*
void escaping_phi() {
	int x, y;
	int *p;

	if (rand()) {
		p = &x;
	} else {
		p = &y;
	}

	GPtr = p;
}
*/

//===----------------------------------------------------------------------===//
// Using global arrays, ConstExpr GEPs
//===----------------------------------------------------------------------===//

// static char GStr[] = "abracadabra";
// static void *PtrArr[N];
// static void *StaticPtr;

// void escape_through_const_expr_GEP() {
// 	GStr[3] = 'z';		// no escape
// 	int x = rand();
// 	StaticPtr = &x;		// escape
// 	int y = rand();
// 	PtrArr[5] = &y;		// escape
// }

/*
void escape_through_const_expr_GEP() {
	GStr[3] = 'z';		// no escape
	int x;
	PtrArr[5] = &x;		// escape
}
*/

//===----------------------------------------------------------------------===//
// Escape through external "already escaping" objects
//===----------------------------------------------------------------------===//

/*
void escape_through_ptr_argument_aliasing(int *k) {
	int *p = k;
}
*/

/*

void escape_through_gptr_aliasing() {
	int *GPtrAlias = GPtr;
}

void bar(int **p) {
	**p = 222;
}

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

//===----------------------------------------------------------------------===//
// Aliases
//===----------------------------------------------------------------------===//

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
