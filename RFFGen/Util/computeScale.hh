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

#ifndef RFFGEN_UTIL_COMPUTE_SCALE_HH
#define RFFGEN_UTIL_COMPUTE_SCALE_HH

#include <utility>

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class X, bool Xpresent>
    struct ComputeScaleImpl
    {
      static constexpr bool present = false;
      ComputeScaleImpl(double, X const&){}
    };

    template <class X>
    struct ComputeScaleImpl<X,true>
    {
      static constexpr bool present = true;

      ComputeScaleImpl(double a, X const& x) : value(a * x())
      {}

      auto operator()() const
      {
        return value;
      }

       decltype(std::declval<double>() * std::declval<X>()()) value;
    };
  }

  template <class X>
  struct ComputeScale : public Detail::ComputeScaleImpl<X,X::present>
  {
    ComputeScale(double a, X const& x)
      : Detail::ComputeScaleImpl<X,X::present> (a,x)
    {}

    template <class F, class... Args>
    ComputeScale(double a, F const& f, const Args&... args) : ComputeScale( a, X(f,args...) )
    {}
  };
  /**
   * \endcond
   */
}

#endif // RFFGEN_UTIL_COMPUTE_SCALE_HH
