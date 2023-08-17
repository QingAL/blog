#include "MyLib2.h"
#include <iostream>

void function() { std::cout << "in MyLib2 function\n"; }

void MyLib2() {
  std::cout << "in MyLib2\n";
  function();
}