#ifndef FUNCTION_GENERATION_UTIL_MAKE_FUNCTION_HH
#define FUNCTION_GENERATION_UTIL_MAKE_FUNCTION_HH

#include <functional>
#include <utility>

namespace FunctionGeneration
{
  namespace Detail
  {
    template <class T, class Arg>
    using return_t = decltype( std::declval<T>()( std::declval<const Arg&>() ) );

    template <class T, class Arg>
    using function_t = std::function< return_t<T,Arg>(const Arg&) >;
  }

  /**
   * Pack a functor in a suitable std::function object.
   */
  template <class Arg, class T>
  Detail::function_t<T,Arg> make_function(const T& t)
  {
    return Detail::function_t<T,Arg>(t);
  }
}

#endif // FUNCTION_GENERATION_UTIL_MAKE_FUNCTION_HH
