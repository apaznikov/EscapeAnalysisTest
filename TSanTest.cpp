// #include "test.h"
#include <memory>
#include <string.h>
#include <stdio.h>

void func(int *x) {
  int *y = &x[55];
  printf("%p\n", y);
  // int LV;
  // printf("%p\n", &LV);
}

// char *data = new char[10];
// char *data1 = new char[10];
// char *data2 = new char[10];

// void *Thread1(void *x) {
// 	static volatile int size = 1;
// 	memcpy(data+5, data1, size);
// 	barrier_wait(&barrier);
// 	return NULL;
// }
//
// void *Thread2(void *x) {
// 	static volatile int size = 4;
// 	barrier_wait(&barrier);
// 	memcpy(data+3, data2, size);
// 	return NULL;
// }

// int main() {
	// barrier_init(&barrier, 2);
	// int *x = new int;
	// printf("%p\n", x);
	// print_address("addr1=", 1, &data[3]);
	// print_address("addr2=", 1, &data[5]);
	// pthread_t t[2];
	// pthread_create(&t[0], NULL, Thread1, NULL);
	// pthread_create(&t[1], NULL, Thread2, NULL);
	// pthread_join(t[0], NULL);
	// pthread_join(t[1], NULL);
	// return 0;
// }

/*
void *thread_func(void *x) {
  barrier_wait(&barrier);
  *static_cast<int *>(x) = 222;
  return nullptr;
}

static void race() {
  int data = 0;
  pthread_t t;
  pthread_create(&t, nullptr, thread_func, &data);
  data = 111;
  barrier_wait(&barrier);
  pthread_join(t, nullptr);
}

struct X {
	__attribute__((noinline))
	X() { atexit(race); }
} x;

int main() {
	barrier_init(&barrier, 2);
	fprintf(stderr, "DONE\n");
}
*/

/*
std::unique_ptr<long> global(new long(42));

void *thread(void *x) {
	*global = 43;
	barrier_wait(&barrier);
	return nullptr;
}

int main() {
	barrier_init(&barrier, 2);
	pthread_t th;
	pthread_create(&th, nullptr, thread, nullptr);
	pthread_detach(th);
	barrier_wait(&barrier);
	return 0;
}
*/

/*
_Atomic(int*) p;

void *thread_func(void *a) {
	barrier_wait(&barrier);
	int *pp = __c11_atomic_load(&p, __ATOMIC_RELAXED);
	*pp = 42;
	return 0;
}

int main() {
	barrier_init(&barrier, 2);
	pthread_t th;
	pthread_create(&th, 0, thread_func, p);
	__c11_atomic_store(&p, new int, __ATOMIC_RELAXED);
	barrier_wait(&barrier);
	pthread_join(th, 0);
}
*/