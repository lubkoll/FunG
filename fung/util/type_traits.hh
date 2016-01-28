// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_TYPE_TRAITS_HH
#define FUNG_UTIL_TYPE_TRAITS_HH

#include <type_traits>

#include "voider.hh"

namespace FunG
{
  /// @cond
  namespace Checks
  {
    // access underlying type of the expression templates of the Eigen library
    template <class EigenArg>
    using TryNestedType_PlainObject                  =     typename EigenArg::PlainObject;
    /// @endcond
  }

  /// Identity, i.e. Decay<F>::type == F
  template <class F, class = void>
  struct Decay
  {
    using type = F;
  };

  /// Underlying type for expression templates of the Eigen library.
  template <class F>
  struct Decay< F , void_t< Checks::TryNestedType_PlainObject<F> > >
  {
    using type = typename F::PlainObject;
  };

  /// Specialize this template class to register arithmetic types that are not built-in.
  template <class F>
  struct is_arithmetic : std::is_arithmetic<F>
  {};

  /// Access underlying type (if it is hidden by expression templates).
  template <class F>
  using decay_t = typename Decay< std::decay_t<F> >::type;
}

#endif // FUNG_UTIL_TYPE_TRAITS_HH
