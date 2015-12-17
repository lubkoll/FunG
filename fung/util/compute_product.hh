// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_COMPUTE_PRODUCT_HH
#define FUNG_UTIL_COMPUTE_PRODUCT_HH

#include <utility>
#include "type_traits.hh"

namespace FunG
{
  /// @cond
  namespace Detail
  {
    template <class X, class Y, bool bothPresent>
    struct ComputeProductImpl
    {
      static constexpr bool present = false;
      ComputeProductImpl(X const&, Y const&) {}
    };

    template <class X, class Y>
    struct ComputeProductImpl<X,Y,true>
    {
      static constexpr bool present = true;

      ComputeProductImpl(X const& x, Y const& y) : value( x() * y() )
      {}

      decltype(auto) operator()() const
      {
        return value;
      }

      decay_t<decltype(std::declval<X>()() * std::declval<Y>()() )> value;
    };
  }
  template <class X, class Y>
  struct ComputeProduct : public Detail::ComputeProductImpl<X,Y,X::present && Y::present>
  {
    ComputeProduct(X const& x, Y const& y)
      : Detail::ComputeProductImpl<X,Y,X::present && Y::present> (x,y)
    {}
  };

  template <class F, class G>
  auto product(const F& f, const G& g)
  {
    return ComputeProduct<F,G>(f,g);
  }
  /// @endcond
}

#endif // FUNG_UTIL_COMPUTE_PRODUCT_HH
