#include <iostream>

typedef void (*Func)(int a, int b, float c, float d);

void Func1(int a, float b, int c, float d) {
  printf("a = %d, b = %f, c = %d, d = %f\n", a, b, c, d);
}

int main() {
  Func funcPtr = (Func)Func1;
  funcPtr(1, 2, 3.01f, 4.01f);
}