#include <iostream>

#include "Person.h"

void AddLine() { std::cout << "------------------------------------------\n"; }

Person GetPerson() {
  Person temp("temp", 10);
  return temp;
}

int main() {
  {
    Person a("temp", 10);  // 构造函数
    Person b = a;          // 拷贝构造函数
    b = a;                 // =运算符
  }
  AddLine();
  {
    Person a =
        GetPerson();  // 没有开启优化 rvo 优化，return 变量会执行拷贝构造2次
  }
  AddLine();
  {
    Person("rvalue",
           1);  // 右值，当分号结束后就析构,C03标准
    AddLine();
  }

  // std::move 是将左值转为右值
  {
    Person a("10", 10);
    Person b(std::move(a));  // 调用移动构造函数
  }
  AddLine();
}