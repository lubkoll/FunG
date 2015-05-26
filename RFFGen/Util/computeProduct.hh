/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library RFFGen.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   RFFGen is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   RFFGen is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with RFFGen.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef RFFGEN_UTIL_COMPUTE_PRODUCT_HH
#define RFFGEN_UTIL_COMPUTE_PRODUCT_HH

#include <utility>

namespace RFFGen
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

//    template <class F, class G, class... Args>
//    ComputeProduct(F const& f, G const& g, const Args&... args) : ComputeProduct( X(f,args...), Y(g,args...) )
//    {}
  };
  /**
   * \endcond
   */

}

#endif // RFFGEN_UTIL_COMPUTE_PRODUCT_HH
