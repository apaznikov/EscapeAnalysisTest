#include <atomic>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int GV;
std::atomic<int*> global_p;

void WriteToMemory() {
  global_p[0] = 777;
}

#if 0
void external_func(int *ptr);

void CAS() {
  // int *expected = &GV;
  int *expected;
  // external_func(expected);
  AtomicVar.compare_exchange_weak(expected, nullptr);
  *expected = 777;
}
#endif

#if 0
// This attempts to exercise a race condition where both a thread and its signal
// handler allocate the SigCtx. If the race is allowed, it leads to a leak and
// the first signal being dropped.
// Spawn threads in a loop and send it SIGUSR1 concurrently with the thread
// doing a bogus kill() call. The signal handler writes to a self-pipe which the
// thread detects and then exits. A dropped signal results in a timeout.
int pipes[2];
static void handler(int sig) { write(pipes[1], "x", 1); }

static int do_select() {
  struct timeval tvs {
    0, 1000
  };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(pipes[0], &fds);
  return select(pipes[0] + 1, &fds, 0, 0, &tvs);
}

static void *thr(void *p) {
  // This kill() is expected to fail; it exists only to trigger a call to SigCtx
  // outside of the signal handler.
  kill(INT_MIN, 0);
  int success = 0;
  for (int i = 0; i < 1024; i++) {
    if (do_select() > 0) {
      success = 1;
      break;
    }
  }
  if (success) {
    char c;
    read(pipes[0], &c, 1);
  } else {
    fprintf(stderr, "Failed to receive signal\n");
    exit(1);
  }
  return p;
}

int main() {
  if (pipe(pipes)) {
    perror("pipe");
    exit(1);
  }

  struct sigaction act = {};
  act.sa_handler = &handler;
  if (sigaction(SIGUSR1, &act, 0)) {
    perror("sigaction");
    exit(1);
  }

  for (int i = 0; i < (1 << 10); i++) {
    pthread_t th{};
    if (pthread_create(&th, 0, thr, 0)) {
      perror("pthread_create");
      exit(1);
    }
    pthread_kill(th, SIGUSR1);
    pthread_join(th, 0);
  }

  fprintf(stderr, "DONE\n");
  return 0;
}
#endif


#if 0
#include "test.h"

#define NUM_ORDS 16
#define NUM_THREADS NUM_ORDS * 2
struct node {
  int val;
};
std::atomic<node *> _nodes[NUM_THREADS] = {};

void func1(int i) {
  auto n = new node();
  n->val = 42;
  _nodes[i].store(n, std::memory_order_release);
}

template <int version>
void func2(int i, std::memory_order mo, std::memory_order fmo) {
  node *expected = nullptr;
  while (expected == nullptr) {
    _nodes[i].compare_exchange_weak(expected, nullptr, mo, fmo);
  };

  ++expected->val;
  assert(expected->val == 43);
}

struct MemOrdSuccFail {
  std::memory_order mo;
  std::memory_order fmo;
};

MemOrdSuccFail OrdList[NUM_ORDS] = {
    {std::memory_order_release, std::memory_order_relaxed},
    {std::memory_order_release, std::memory_order_acquire},
    {std::memory_order_release, std::memory_order_consume},
    {std::memory_order_release, std::memory_order_seq_cst},

    {std::memory_order_acq_rel, std::memory_order_relaxed},
    {std::memory_order_acq_rel, std::memory_order_acquire},
    {std::memory_order_acq_rel, std::memory_order_consume},
    {std::memory_order_acq_rel, std::memory_order_seq_cst},

    {std::memory_order_seq_cst, std::memory_order_relaxed},
    {std::memory_order_seq_cst, std::memory_order_acquire},
    {std::memory_order_seq_cst, std::memory_order_consume},
    {std::memory_order_seq_cst, std::memory_order_seq_cst},

    {std::memory_order_relaxed, std::memory_order_relaxed},
    {std::memory_order_relaxed, std::memory_order_acquire},
    {std::memory_order_relaxed, std::memory_order_consume},
    {std::memory_order_relaxed, std::memory_order_seq_cst},
};

int main() {
  std::thread threads[NUM_THREADS];
  int ords = 0;

  // Instantiate a new f2 for each MO so we can dedup reports and actually
  // make sure relaxed FMO triggers a warning for every different MO.
  for (unsigned t = 0; t < 8; t += 2) {
    threads[t] = std::thread(func1, t);
    threads[t + 1] = std::thread(func2<0>, t, OrdList[ords].mo, OrdList[ords].fmo);
    threads[t].join();
    threads[t + 1].join();
    ords++;
  }

  for (unsigned t = 8; t < 16; t += 2) {
    threads[t] = std::thread(func1, t);
    threads[t + 1] = std::thread(func2<1>, t, OrdList[ords].mo, OrdList[ords].fmo);
    threads[t].join();
    threads[t + 1].join();
    ords++;
  }

  for (unsigned t = 16; t < 24; t += 2) {
    threads[t] = std::thread(func1, t);
    threads[t + 1] = std::thread(func2<2>, t, OrdList[ords].mo, OrdList[ords].fmo);
    threads[t].join();
    threads[t + 1].join();
    ords++;
  }

  for (unsigned t = 24; t < 32; t += 2) {
    threads[t] = std::thread(func1, t);
    threads[t + 1] = std::thread(func2<3>, t, OrdList[ords].mo, OrdList[ords].fmo);
    threads[t].join();
    threads[t + 1].join();
    ords++;
  }

  fprintf(stderr, "DONE\n");
  return 0;
}

#endif


// void func(int *x) {
  // int *y = &x[55];
  // printf("%p\n", y);
  // int LV;
  // printf("%p\n", &LV);
// }

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