#include "Person.h"

#include <iostream>

Person::Person(const char* _name, int _age) : age(_age) {
  size_t len = sizeof(_name);
  name = (char*)calloc(len + 1, sizeof(char));
  strcpy(name, _name);
  std::cout << "construct Person::Person(const char*, int)\n";
}

Person::Person(const Person& that) : age(that.age) {
  size_t len = sizeof(that.name);
  name = (char*)calloc(len + 1, sizeof(char));
  strcpy(name, that.name);
  std::cout << "copy construct Person::Person(const Person&)\n";
}

Person::~Person() {
  if (name) {
    free(name);
    name = nullptr;
  }
  std::cout << "deconstruct Person::~Person()\n";
}

Person& Person::operator=(const Person& that) {
  free(name);
  size_t len = sizeof(that.name);
  name = (char*)calloc(len + 1, sizeof(char));
  strcpy(name, that.name);
  std::cout << "Person::operator=(const Person&)\n";
  return *this;
}

#ifdef R_REF
Person::Person(Person&& that) {
  name = that.name;
  age = that.age;
  that.name = nullptr;
  std::cout << "copy construct Person::Person(Person&&)\n";
}

Person& Person::operator=(Person&& that) {
  name = that.name;
  age = that.age;
  that.name = nullptr;
  std::cout << "Person::operator=(Person&&)\n";
  return *this;
}
#endif