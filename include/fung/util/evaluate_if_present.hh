// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_EVALUATE_IF_PRESENT_HH
#define FUNG_UTIL_EVALUATE_IF_PRESENT_HH

#include <type_traits>

#include "voider.hh"

namespace FunG
{
  /// @cond
  namespace Detail
  {
    template <class F, class Arg>
    using TryCallOfUpdate = decltype( std::declval<F>().update(std::declval<Arg>()) );

    template <class F, class Arg, int id>
    using TryCallOfUpdateWithIndex = decltype( std::declval<F>().template update<id>(std::declval<Arg>()) );


    template <class F, class Arg, class = void>
    struct HasUpdateWithoutIndex : std::false_type {};

    template <class F, class Arg>
    struct HasUpdateWithoutIndex<F,Arg,void_t< TryCallOfUpdate<F,Arg> > > : std::true_type {};


    template <class F, class Arg, int id, class = void>
    struct HasUpdateWithIndex : std::false_type {};

    template <class F, class Arg, int id>
    struct HasUpdateWithIndex<F,Arg,id,void_t< TryCallOfUpdateWithIndex<F,Arg,id> > > : std::true_type {};
  }
  /// @endcond


  template <class F, class Arg,
            std::enable_if_t<!Detail::HasUpdateWithoutIndex<F,Arg>::value>* = nullptr>
  void update_if_present(F&&, Arg&&)
  {}

  template <class F, class Arg,
            std::enable_if_t<Detail::HasUpdateWithoutIndex<F,Arg>::value>* = nullptr>
  void update_if_present(F&& f, Arg&& x)
  {
    f.update(std::forward<Arg>(x));
  }


  template <int id, class F, class Arg,
            std::enable_if_t<!Detail::HasUpdateWithIndex<F,Arg,id>::value>* = nullptr>
  void update_if_present(F&&, Arg&&)
  {}

  template <int id, class F, class Arg,
            std::enable_if_t<Detail::HasUpdateWithIndex<F,Arg,id>::value>* = nullptr>
  void update_if_present(F&& f, Arg&& x)
  {
    f.template update<id>(std::forward<Arg>(x));
  }
}

#endif // FUNG_UTIL_EVALUATE_IF_PRESENT_HH
