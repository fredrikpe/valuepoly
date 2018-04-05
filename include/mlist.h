
template <typename ...Args>
struct signatures {};

template <typename Ret1, typename Arg>
struct signatures<Ret1(Arg)>
{
  using result_type = Ret1;
  using arg_type = Arg;
};

template <typename Ret1, typename Arg1, typename Ret2, typename Arg2, typename ...Args>
struct signatures<Ret1(Arg1), Ret2(Arg2), Args...>
{
};

template <typename taList, int taPos>
struct BListAt;

template <typename T>
struct BListAt<T, 0>
  : T {};

template <typename Head, typename... Tail>
struct BListAt<signatures<Head, Tail...>, 0>
  : signatures<Head> {};

template <typename Head, typename... Tail, int Pos>
struct BListAt<signatures<Head, Tail...>, Pos>
  : BListAt<signatures<Tail...>, Pos - 1> {};


