
#include <type_traits>
#include <tuple>

template <typename... Args>
struct mlist
{};

template <typename T>
struct mlist_unwrap
{
  using type = T;
};

template <>
struct mlist_unwrap<mlist<>>
{
  using type = void;
};

template <typename T>
struct mlist_unwrap<mlist<T>>
{
  using type = T;
};

template <typename taFirst, typename taSecond, typename... taTail>
struct mlist_unwrap<mlist<taFirst, taSecond, taTail...>>
{
  using type = mlist<taFirst, taSecond, taTail...>;
};


template <typename taList, int taPos>
struct mlist_at;

template <typename taAny>
struct mlist_at<taAny, 0>
  : mlist_unwrap<taAny> {};

template <typename taHead, typename... taTail>
struct mlist_at<mlist<taHead, taTail...>, 0>
{
  using type = taHead;
};

template <typename taHead, typename... taTail, int taPos>
struct mlist_at<mlist<taHead, taTail...>, taPos>
  : mlist_at<mlist<taTail...>, taPos - 1> {};



template <typename ...Args>
struct BList {};

template <typename Ret1, typename ...Args1>
struct BList<Ret1(Args1...)>
{
  using result_type = Ret1;
  using arg_types = std::tuple<Args1...>;
};

template <typename Ret1, typename Ret2, typename ...Args1, typename ...Args2, typename ...Args3>
struct BList<Ret1(Args1...), Ret2(Args2...), Args3...>
{
  using head = BList<Ret1(Args1...)>;
  using tail = BList<Ret2(Args2...), Args3...>;

};

template <typename T>
struct BListUnwrap
{
  using type = T;
};

template <>
struct BListUnwrap<BList<>>
{
  using type = void;
};

template <typename T>
struct BListUnwrap<BList<T>>
{
  using type = T;
};

template <typename taFirst, typename taSecond, typename... taTail>
struct BListUnwrap<BList<taFirst, taSecond, taTail...>>
{
  using type = BList<taFirst, taSecond, taTail...>;
};



template <typename taList, int taPos>
struct BListAt;

template <typename taAny>
struct BListAt<taAny, 0>
  : BListUnwrap<taAny> {};

template <typename taHead, typename... taTail>
struct BListAt<BList<taHead, taTail...>, 0>
{
  using type = BList<taHead>;
};

template <typename taHead, typename... taTail, int taPos>
struct BListAt<BList<taHead, taTail...>, taPos>
  : BListAt<BList<taTail...>, taPos - 1> {};


