// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_TYPE_TRAITS_HH
#define FUNG_UTIL_TYPE_TRAITS_HH

#include "static_checks.hh"

namespace FunG
{
  /*!
    \brief Identity, i.e. Decay<F>::type == F
   */
  template <class F, class = void>
  struct Decay
  {
    using type = F;
  };

  /*!
    \brief Underlying type for expression templates of the Eigen library.
   */
  template <class F>
  struct Decay<F,void_t<Checks::TryAccessToPlainObject<F> > >
  {
    using type = typename F::PlainObject;
  };

  /*!
    \brief Access underlying type (if it is hidden by expression templates).
   */
  template <class F>
  using decay_t = typename Decay<F>::type;
}

#endif // FUNG_UTIL_TYPE_TRAITS_HH
