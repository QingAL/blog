#include <string>

/*

            lvalue   const lvalue   rvalue   const rvalue
---------------------------------------------------------
X&          yes
const X&    yes      yes            yes      yes
X&&                                 yes
const X&&                           yes      yes

*/

class Person {
 public:
  char* name;
  int age;
  Person(const char* _name, int _age);
  Person(const Person& that);
  ~Person();
  Person& operator=(const Person& that);

#ifdef R_REF
  Person(Person&& that);
  Person& operator=(Person&& that);
#endif
};