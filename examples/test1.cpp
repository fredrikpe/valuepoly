
#include "../include/vp.h"

#include <memory>
#include <vector>
#include <string>
#include <iostream>

struct X
{
  void f(int n) const
  {
    std::cout << n;
  }
};

struct Y : X {};

struct A
{
  int f() {return 42;}
};

int main()
{

  using sig = void(int);
	std::cout << tmp::has_signature<Y, sig>::value; // will print 1

  using MyPoly = tmp::poly<sig>;

  std::vector<MyPoly> v;
  v.push_back(X{});
  //v.push_back(A{});

  for (auto e : v)
  {
    e.f(1);
  }
}












