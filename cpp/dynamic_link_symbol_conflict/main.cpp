#include <iostream>

extern void function();
extern void MyLib1();
extern void MyLib2();

int main() {
  // default symbol
  function();
}