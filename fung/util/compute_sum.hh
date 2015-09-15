/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_UTIL_COMPUTE_SUM_HH
#define FUNG_UTIL_COMPUTE_SUM_HH

#include <utility>

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class X, class Y, bool Xpresent, bool Ypresent>
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

      auto operator()() const
      {
        return value;
      }

      decltype(std::declval<X>()() + std::declval<Y>()()) value;
    };

    template <class X, class Y>
    struct ComputeSumImpl<X,Y,true,false>
    {
      static constexpr bool present = true;

      ComputeSumImpl(const X& x, const Y&) : value(x())
      {}

      auto operator()() const
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

      auto operator()() const
      {
        return value;
      }

      decltype(std::declval<Y>()()) value;
    };
  }

  template <class...> struct ComputeSum;

  template <class X, class Y>
  struct ComputeSum<X,Y> : public Detail::ComputeSumImpl<X,Y,X::present,Y::present>
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
  /**
   * \endcond
   */
}

#endif // FUNG_UTIL_COMPUTE_SUM_HH
