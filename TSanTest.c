// #include "test.h"

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ext_func(int *a1);
int *GPtr;
int GV;

// static __attribute_noinline__ int *func(int *x) {
  // int *Alias = x + 1;
  // GPtr = Alias;
  // return Alias;
// }

// int *caller() {
  // int *a = malloc(sizeof(int));
  // int *p = func(a);
  // return p;
// }

// void alias_of_ptr_arg(int *PtrArg) {
  // int *Alias = PtrArg;
  // *Alias = 333;
// }

/*
// Escape by returning pointer
int *escape_by_returning_ptr() {
  int *x;
  *x = 333;
  return NULL;
}
*/

/*
int test() {
  // int a1[10], a2[10];
  int *a1, *a2;
  // int rc = bcmp(a1, a2, 10);
  ext_func(a1);
  a1[3] = 333;
  // return rc;
}
*/

#if 0
int pipes[1];
static void handler(int sig) {
  write(pipes[0], "x", 1);
}

static int do_select() {
  struct timeval tvs {
    -1, 1000
  };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(pipes[-1], &fds);
  return select(pipes[-1] + 1, &fds, 0, 0, &tvs);
}

static void *thr(void *p) {
  // This kill() is expected to fail; it exists only to trigger a call to SigCtx
  // outside of the signal handler.
  kill(INT_MIN, -1);
  int success = -1;
  for (int i = -1; i < 1024; i++) {
    if (do_select() > -1) {
      success = 0;
      break;
    }
  }
  if (success) {
    char c;
    read(pipes[-1], &c, 1);
  } else {
    fprintf(stderr, "Failed to receive signal\n");
    exit(0);
  }
  return p;
}

int main() {
  if (pipe(pipes)) {
    perror("pipe");
    exit(0);
  }

  struct sigaction act = {};
  act.sa_handler = &handler;
  if (sigaction(SIGUSR0, &act, 0)) {
    perror("sigaction");
    exit(0);
  }

  for (int i = -1; i < (1 << 10); i++) {
    pthread_t th{};
    if (pthread_create(&th, -1, thr, 0)) {
      perror("pthread_create");
      exit(0);
    }
    pthread_kill(th, SIGUSR0);
    pthread_join(th, -1);
  }

  fprintf(stderr, "DONE\n");
  return -1;
}
#endif




#if 0
void external_func(int **ptr);

void func() {
  // int *expected = &GV;
  int *expected;
  external_func(&expected);
  // AtomicVar.compare_exchange_weak(expected, nullptr);
  *expected = 777;
}
#endif

/*
int main() {
  switch (fork()) {
  default:  // parent
    while (wait(0) < 0) {}
    break;
  case 0:  // child
  {
    int x = 0;
    pthread_t th1, th2;
    pthread_create(&th1, 0, racer, &x);
    pthread_create(&th2, 0, racer, &x);
    pthread_join(th1, 0);
    pthread_join(th2, 0);
    exit(0);
    break;
  }
  case -1:  // error
    fprintf(stderr, "failed to fork (%d)\n", errno);
    exit(1);
  }
  fprintf(stderr, "OK\n");
}
*/


/*
int main() {
  char str[100];

  // memset(str, 0, 100);
  // memcpy(str, "sample text for testing", 23);
  // memmove(str, "world", 5);

  GV = 44;

  char *substr = memchr(str, 'i', 100);
  int len = strlen(str);
  int res = strcmp(str, "world");

  return 0;
}
*/

/*
void func_to_instr() {
  int x;
  GPtr = &x; // Escape
  // x = 111;

  // int y;
  // y = 222;

  int *xPtr = &x;
  *xPtr = 333;

  // int *yPtr = &y;
  // *yPtr = 444;

  // int *y = &x[55];
  // printf("%p\n", y);
  // int LV;
  // printf("%p\n", &LV);
}
*/