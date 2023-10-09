#include "base64.h"
#include <iostream>

// int main() {
//   auto base64= base64::to_base64("Hello, World!");
//   std::cout << base64 << '\n'; // SGVsbG8sIFdvcmxkIQ==
//   auto s = base64::from_base64("SGVsbG8sIFdvcmxkIQ==");
//   std::cout << s << '\n'; // Hello, World!
// }

int main() {
  std::string original;
  for (int i = 0; i < 1024; i++) {
    original += char(std::rand());
  }

  std::cout << "char is "
            << (std::is_signed<char>::value ? "signed" : "unsigned")
            << std::endl;

  auto encoded = base64::to_base64(original);
  auto s = base64::from_base64(encoded);
  if (s == original) {
    std::cout << "PASS!!\n";
    return 0;
  } else {
    std::cout << "FAIL!!\n";
    return 1;
  }
}