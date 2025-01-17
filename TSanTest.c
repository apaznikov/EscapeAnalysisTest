// #include "test.h"

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


/*
int *GPtr;

// int *mem;

void func() {
  int *mem = malloc(100);
  // GPtr = mem;
  memset(mem, 0, 100);
  // int *mem2 = malloc(100);
  memcpy(mem, "hello", 5);
  memmove(mem, "world", 5);

  // mem[33] = 444;

  // int x;
  // GPtr = &x;
  // int *alias = &x;
  // *alias = 555;
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