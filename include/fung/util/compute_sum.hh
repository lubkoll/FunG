// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_COMPUTE_SUM_HH
#define FUNG_UTIL_COMPUTE_SUM_HH

#include <utility>
#include "type_traits.hh"

namespace FunG
{
  /// @cond
  namespace Detail
  {
    template <class X, class Y, bool = X::present, bool = Y::present>
    struct ComputeSumImpl
    {
      static constexpr bool present = false;
      ComputeSumImpl(const X&, const Y&) {}
    };

    template <class X, class Y>
    struct ComputeSumImpl<X,Y,true,true>
    {
      static constexpr bool present = true;

      ComputeSumImpl(const X& x, const Y& y) : value(x() + y())
      {}

      decltype(auto) operator()() const
      {
        return value;
      }

      decay_t<decltype(std::declval<X>()()+std::declval<Y>()())> value;
    };

    template <class X, class Y>
    struct ComputeSumImpl<X,Y,true,false>
    {
      static constexpr bool present = true;

      ComputeSumImpl(const X& x, const Y&) : value(x())
      {}

      decltype(auto) operator()() const
      {
        return value;
      }

      decltype(std::declval<X>()()) value;
    };

    template <class X, class Y>
    struct ComputeSumImpl<X,Y,false,true>
    {
      static constexpr bool present = true;

      ComputeSumImpl(const X&, const Y& y) : value(y())
      {}

      decltype(auto) operator()() const
      {
        return value;
      }

      decltype(std::declval<Y>()()) value;
    };
  }

  template <class...> struct ComputeSum;

  template <class X, class Y>
  struct ComputeSum<X,Y> : public Detail::ComputeSumImpl<X,Y>
  {
    ComputeSum(const X& x, const Y& y)
      : Detail::ComputeSumImpl<X,Y,X::present,Y::present>(x,y)
    {}

    template <class F, class G, class... Args>
    ComputeSum(F const& f, G const& g, const Args&... args) : ComputeSum( X(f,args...), Y(g,args...) )
    {}
  };

  template <class X, class... Y>
  struct ComputeSum<X,Y...> : public ComputeSum<X,ComputeSum<Y...> >
  {
    ComputeSum(const X& x, const Y&... y) : ComputeSum<X,ComputeSum<Y...> >(x,ComputeSum<Y...>(y...))
    {}
  };

  template <class F, class... G>
  auto sum(F&& f, G&&... g)
  {
    return ComputeSum<std::decay_t<F>,std::decay_t<G>...>(std::forward<F>(f),std::forward<G>(g)...);
  }
  /// @endcond
}

#endif // FUNG_UTIL_COMPUTE_SUM_HH
