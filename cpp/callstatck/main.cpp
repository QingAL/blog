#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define STATCK_MAX_DEEP 16

void crashTest() {
  int *intVal = nullptr;
  *intVal = 5;
}

void crashHandle(int signo) {
  printf("get signal : %d\n", signo);
  void *statck[STATCK_MAX_DEEP];
  size_t statckDeep = backtrace(statck, STATCK_MAX_DEEP);
  char **statckStr = backtrace_symbols(statck, statckDeep);
  for (int i = 0; i < statckDeep; ++i) {
    printf("%s\n", statckStr[i]);
  }
  exit(0);
}

void Func1() { crashTest(); }
void Func() { Func1(); }

int main() {
  signal(SIGSEGV, crashHandle);
  Func();
  return 0;
}