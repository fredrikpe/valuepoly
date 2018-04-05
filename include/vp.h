#pragma once

#include <memory>
#include <type_traits>


namespace tmp {

template<typename, typename T>
struct has_signature
{
	static_assert(std::integral_constant<T, false>::value,
                "Second template parameter needs to be of function type.");
};

template<typename C, typename Ret, typename... Args>
struct has_signature<C, Ret(Args...)>
{
private:
	template<typename T>
	static constexpr auto check(T*)
	-> typename
		std::is_same<
			decltype( std::declval<T>().f( std::declval<Args>()... ) ),
			Ret
		>::type;

	template<typename>
	static constexpr std::false_type check(...);

	using type = decltype(check<C>(0));

public:
	static constexpr bool value = type::value;
	static constexpr bool nvalue = !type::value;
};


template <typename ...Args>
class poly {};

template <typename Ret, typename ...Arg>
class poly<Ret(Arg...)>
{
public:
  template <typename T, typename =
    typename std::enable_if<has_signature<T, Ret(Arg...)>::value, T>::type>
  poly(T t)
    : _model(std::make_shared<model<T>>(std::move(t)))
  {}

  Ret f(Arg... arg)
  {
    return _model->f(arg...);
  }

private:
  struct concept
  {
    virtual ~concept() = default;
    virtual Ret f(Arg... arg) const = 0;
  };

  template <typename T>
  struct model final : concept
  {
    model(T t) : data(std::move(t))
    {}

    Ret f(Arg... arg) const
    {
      return data.f(arg...);
    }

    T data;
  };

  std::shared_ptr<const concept> _model;
};

} // tmp
