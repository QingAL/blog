#include <iostream>
#include "MyDll.h"

int main () {
  std::cout << "hello form main\n";
  std::cout << __TIME__ << std::endl;
  std::cout << __DATE__ << std::endl;
  MyDll::hello();
}