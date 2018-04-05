
#include "../include/mlist.h"

#include <string>
#include <iostream>

class BaseLayer
{
public:
  template <typename AllLayers>
  class Impl
  {
  protected:
    template <typename ...Args>
    explicit Impl(const Args&...)
    {}

  public:
    using MostDerived = typename AllLayers::template Impl<AllLayers>;

    MostDerived&       most_derived()       { return static_cast<MostDerived&>(*this); }
    const MostDerived& most_derived() const { return static_cast<const MostDerived&>(*this); }
  };
};

template <typename UpperLayer>
class LayerA : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Parent = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(std::string s, int i, Args... args)
      : Parent(args...),
      s(s),
      layerA_int(i)
    { }

  public:
    void foo()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      this->most_derived().bar();
    }

    void bar()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    std::string s;
    int layerA_int;
  };
};

template <typename UpperLayer>
class LayerB : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Parent = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(int i, Args... args) :
      Parent(args...),
      layerB_int(i)
    { }

    void bar()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      Parent::bar();
    }

    int layerB_int;
  };
};

template <typename UpperLayer>
class LayerC : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Parent = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(Args... args)
      : Parent(args...)
    { }
  };
};



int main()
{
  using MyLayerA = LayerA<BaseLayer>;
  using MyLayerAB = LayerB<MyLayerA>;
  using MyLayerABC = LayerC<MyLayerAB>;
  using AllLayers = MyLayerABC;

  using ImplA = MyLayerA::Impl<AllLayers>;
  using MyImplAB = MyLayerAB::Impl<AllLayers>;
  using MyImplABC = MyLayerABC::Impl<AllLayers>;

  //MyImplA sa(std::string("asdf"), 20);
  MyImplAB sb{1, "asdf", 20};
  MyImplABC sc{sb};


  sc.foo();
  std::cout << sc.layerA_int << std::endl;
  std::cout << sc.layerB_int << std::endl;
}


