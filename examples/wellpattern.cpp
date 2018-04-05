
#include "../include/mlist.h"

#include <string>
#include <iostream>

class BottomLayer
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
    using Top = typename AllLayers::template Impl<AllLayers>;

    Top&       top()       { return static_cast<Top&>(*this); }
    const Top& top() const { return static_cast<const Top&>(*this); }
  };
};

template <typename UpperLayer>
class LayerA : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Super = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(std::string s, int i, Args... args)
      : Super(args...),
      s(s),
      iLayerAData(i)
    { }

  public:
    void foo()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      this->top().bar();
    }

    void bar()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    std::string s;
    int iLayerAData;
  };
};

template <typename UpperLayer>
class LayerB : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Super = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(int i, Args... args) :
      Super(args...),
      iLayerBData(i)
    { }

    void bar()
    {
      std::cout << __PRETTY_FUNCTION__ << std::endl;
      Super::bar();
    }

    int iLayerBData;
  };
};

template <typename UpperLayer>
class LayerC : public UpperLayer
{
public:
  template <typename AllLayers>
  class Impl : public UpperLayer::template Impl<AllLayers>
  {
    using Super = typename UpperLayer::template Impl<AllLayers>;

  public:
    template <typename ...Args>
    explicit Impl(Args... args)
      : Super(args...)
    { }
  };
};



int main()
{
  using MyLayerA = LayerA<BottomLayer>;
  using MyLayerAB = LayerB<MyLayerA>;
  using MyLayerABC = LayerC<MyLayerAB>;

  using MySharedA = MyLayerA::Impl<MyLayerABC>;
  using MySharedAB = MyLayerAB::Impl<MyLayerABC>;
  using MySharedABC = MyLayerABC::Impl<MyLayerABC>;

  MySharedA sa(std::string("asdf"), 20);
  MySharedAB sb{10, sa};
  MySharedABC sc{sb};

  //MySharedAB s(10, std::string("asdf"), 20);

  sc.foo();
  std::cout << sc.iLayerBData << std::endl;
}


