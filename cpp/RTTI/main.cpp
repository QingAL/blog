#include <iostream>
#include <typeinfo>
using namespace std;

class A
{
public:
     void Print() { cout<<"This is class A."<<endl; }
};

class B : public A
{
public:
     void Print() { cout<<"This is class B."<<endl; }
};

struct C
{
     void Print() { cout<<"This is struct C."<<endl; }
};

int main()
{
     A *pA1 = new A();
     A a2;

     cout<<typeid(pA1).name()<<endl; // class A *
     cout<<typeid(a2).name()<<endl; // class A

     B *pB1 = new B();
     cout<<typeid(pB1).name()<<endl; // class B *

     C *pC1 = new C();
     C c2;

     cout<<typeid(pC1).name()<<endl; // struct C *
     cout<<typeid(c2).name()<<endl; // struct C

     return 0;
}