#include <iostream>

extern void function();
extern void MyLib1();
extern void MyLib2();

int main() {
  // default symbol
  function();
  std::cout << "-----------------------------\n";
  MyLib1();
  std::cout << "-----------------------------\n";
  MyLib2();
}