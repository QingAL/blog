#include <iostream>
#include <cstring>
#include "TEA.h"


int main() {
  std::string data = "1234";
  std::string key = "qwerqwerqwerqwer";
  std::string ret1 = TEA::encrypt(data, key);
  printf("ret1: %s, len: %ld\n", ret1.c_str(), ret1.length());
  std::string ret2 = TEA::decrypt(ret1, key);
  printf("ret2: %s, len: %ld\n", ret2.c_str(), ret2.length());
  return 0;
}

