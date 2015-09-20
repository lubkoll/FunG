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

#ifndef FUNG_UTIL_COMPUTE_PRODUCT_HH
#define FUNG_UTIL_COMPUTE_PRODUCT_HH

#include <utility>

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
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

      auto operator()() const
      {
        return value;
      }

      decltype(std::declval<X>()() * std::declval<Y>()()) value;
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

  /**
   * \endcond
   */

}

#endif // FUNG_UTIL_COMPUTE_PRODUCT_HH
