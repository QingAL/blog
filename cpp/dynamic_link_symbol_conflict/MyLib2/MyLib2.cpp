#include <iostream>
#include "MyLib2.h"

void function() {
  std::cout<< "in MyLib2 function\n";
}

void MyLib2() {
    std::cout<< "in MyLib2\n";
    function();
}