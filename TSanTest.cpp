#include "test.h"

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