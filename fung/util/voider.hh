// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_VOIDER_HH
#define FUNG_UTIL_VOIDER_HH

namespace FunG
{
  /// @cond
  namespace Detail
  {
    /// helper to make gcc happy
    template <class...> struct voider { using type = void; };
  }
  /// @endcond


  /// Most fascinating type ever. Is void for all input types.
  template <class... Types>
  using void_t = typename Detail::voider<Types...>::type;
}

#endif // FUNG_UTIL_VOIDER_HH
