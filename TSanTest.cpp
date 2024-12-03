#include "test.h"

#include <memory>



/*
void *thread(void *x) {
	barrier_wait(&barrier);
	*static_cast<int *>(x) = 222;
	return nullptr;
}

static void race() {
	int data = 0;
	pthread_t t;
	pthread_create(&t, nullptr, thread, &data);
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