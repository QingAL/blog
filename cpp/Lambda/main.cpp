#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  // 使用 Lambda 表达式作为函数对象
  int count = std::count_if(v.begin(), v.end(),
                            [](int x) -> bool { return x % 2 == 0; });
  std::cout << "There are " << count << " even numbers.\n";
  // 使用 Lambda 表达式作为函数指针
  auto cmp = [](int x, int y) -> bool { return x > y; };
  std::sort(v.begin(), v.end(), cmp);
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << "\n";

  printf("----------------------\n");
  // 变量捕获
  int a = 1, b = 1, c = 1;
  printf("a addr: %p\n", &a);
  printf("b addr: %p\n", &b);
  printf("c addr: %p\n", &c);

  auto m1 = [a, &b, &c](int param1, int *param2) -> int {
    printf("----------------------\n");
    printf("params1: %p\n", &param1);
    printf("a: %d, b: %d, c: %d\n", a, b, c);
    printf("a addr: %p\n", &a);
    printf("b addr: %p\n", &b);
    printf("c addr: %p\n", &c);
    int ans = a + b + c;
    printf("value is: %d, addr: %p\n", ans, &ans);
    return ans;
  };

  int ans = m1(a, &b);
  printf("value is: %d, addr: %p\n", ans, &ans);

  return 0;
}