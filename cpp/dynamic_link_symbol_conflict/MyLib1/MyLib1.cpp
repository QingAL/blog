#include "MyLib1.h"
#include <iostream>

void function() { std::cout << "in MyLib1 function\n"; }

void MyLib1() {
  std::cout << "in MyLib1\n";
  function();
}