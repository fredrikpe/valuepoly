
#include "../include/mlist.h"


#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include <type_traits>
#include <typeinfo>

//Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.
template<typename, typename T>
struct has_serialize
{
	static_assert(std::integral_constant<T, false>::value,
                "Second template parameter needs to be of function type.");
};

// specialization that does the checking
template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)>
{
private:
	template<typename T>
	static constexpr auto check(T*)
	-> typename
		std::is_same<
			decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
			Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		>::type;  // attempt to call it and see if the return type is correct

	template<typename>
	static constexpr std::false_type check(...);

	typedef decltype(check<C>(0)) type;

public:
	static constexpr bool value = type::value;
};

struct X
{
  void f(std::tuple<int, int, int> t) const
  { std::cout << std::get<0>(t)
              << std::get<1>(t)
              << std::get<2>(t); } 
};


struct Y : X {};

struct A {};

template <typename BList>
class object_t
{
public:
  template <typename T>
  object_t(T t)
    : m(std::make_shared<model<T>>(std::move(t)))
  {}

  template <int N>
  auto f(typename BListAt<BList, N>::type::arg_types args) const
    -> typename BListAt<BList, N>::type::result_type
  {
    return m->function_0(args);
  }

private:
  struct concept
  {
    virtual ~concept() = default;
//    virtual typename B::return_type function_0(typename B::arg_types args) const = 0;
    virtual auto function_0(typename BListAt<BList, 0>::type::arg_types args) const
      -> typename BListAt<BList, 0>::type::result_type = 0;
  };

  template <typename T>
  struct model final : concept
  {
    model(T t) : data(std::move(t))
    {}

    auto function_0(typename BListAt<BList, 0>::type::arg_types args) const
      -> typename BListAt<BList, 0>::type::result_type
    {
      return data.function<0>(args);
    }

    T data;
  };

  std::shared_ptr<const concept> m;
};

int main()
{

	std::cout << has_serialize<Y, void()>::value; // will print 1

  using b = BList<void(int, int, int), void(void)>;
  using MyObject = object_t<b>;

  std::vector<MyObject> v;
  v.push_back(X{});
  v.push_back(X{});

  for (auto e : v)
  {
    e.function<0>(std::tuple<int,int,int>(1, 2, 34));
  }
}












