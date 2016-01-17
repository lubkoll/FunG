// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_TYPE_TRAITS_HH
#define FUNG_UTIL_TYPE_TRAITS_HH

#include <type_traits>
#include "static_checks.hh"

namespace FunG
{
  /// Identity, i.e. Decay<F>::type == F
  template <class F, class = void>
  struct Decay
  {
    using type = F;
  };

  /// Underlying type for expression templates of the Eigen library.
  template <class F>
  struct Decay<F,void_t<Checks::TryNestedType_PlainObject<F> > >
  {
    using type = typename F::PlainObject;
  };

  /// Specialize this template class to register arithmetic types that are not built-in.
  template <class F>
  struct IsArithmetic : std::false_type
  {};

  /// Access underlying type (if it is hidden by expression templates).
  template <class F>
  using decay_t = typename Decay< std::decay_t<F> >::type;

  template <class F>
  using is_arithmetic = std::integral_constant< bool , std::is_arithmetic<F>::value || IsArithmetic<F>::value >;
}

#endif // FUNG_UTIL_TYPE_TRAITS_HH
