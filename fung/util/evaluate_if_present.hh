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

#ifndef FUNG_UTIL_EVALUATE_IF_PRESENT_HH
#define FUNG_UTIL_EVALUATE_IF_PRESENT_HH

#include <type_traits>

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class X, bool Xpresent>
    struct EvaluateIfPresentImpl
    {
      static constexpr bool present = false;
      EvaluateIfPresentImpl(X const&){}
    };

    template <class X>
    struct EvaluateIfPresentImpl<X,true>
    {
      using ReturnType = std::add_const_t<decltype(std::declval<X>()())>;
      static constexpr bool present = true;

      EvaluateIfPresentImpl(X const& x) : value(x())
      {}

      ReturnType operator()() const
      {
        return value;
      }

      ReturnType value;
    };
  }


  /// Evaluate a functor object of type X if X::present=true.
  template <class X>
  struct EvaluateIfPresent : public Detail::EvaluateIfPresentImpl<X,X::present>
  {
    EvaluateIfPresent(X const& x) : Detail::EvaluateIfPresentImpl<X,X::present> (x)
    {}

    template <class... Args>
    EvaluateIfPresent(const Args&... args) : EvaluateIfPresent( X(args...) )
    {}
  };
  /**
   * \endcond
   */
}

#endif // FUNG_UTIL_EVALUATE_IF_PRESENT_HH
