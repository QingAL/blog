#include "Singleton.h"
#include <stdio.h>

class Test : public Singleton<Test> {};

#ifdef DEMO_DEBUG
void *func(void *waitTime) {
  int *waitPtr = (int *)waitTime;
  Test *ptr = Test::debugGetInstance(*waitPtr);
  printf("addr: %p\n", ptr);
}
#endif

int main() {

  Test *ptr = Test::getInstance();
  printf("addr: %p\n", ptr);

#ifdef DEMO_DEBUG
  int wait = 5;
  int go = 0;

  pthread_t threadA;
  pthread_t threadB;

  pthread_create(&threadA, nullptr, func, (void *)&wait);
  pthread_create(&threadB, nullptr, func, (void *)&go);

  pthread_join(threadA, nullptr);
  pthread_join(threadB, nullptr);
#endif

  return 0;
}