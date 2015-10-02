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

#include "voider.hh"

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
      using ReturnType = std::decay_t<decltype(std::declval<X>()())>;
      static constexpr bool present = true;

      EvaluateIfPresentImpl(X const& x) : value(x())
      {}

      ReturnType operator()() const
      {
        return value;
      }

      ReturnType value;
    };


    template <class F, class Arg>
    using TryCallOfUpdate = decltype( std::declval<F>().update(std::declval<Arg>()) );

    template <class F, class Arg, int id>
    using TryCallOfUpdateWithIndex = decltype( std::declval<F>().template update<id>(std::declval<Arg>()) );

    template <class F, class Arg, class = void>
    struct HasUpdateWithoutIndex : std::false_type {};
    template <class F, class Arg>
    struct HasUpdateWithoutIndex<F,Arg,void_t< TryCallOfUpdate<F,Arg> > > : std::true_type {};

    template <class F, class Arg, int id, class = void>
    struct HasUpdateWithIndex : std::false_type {};
    template <class F, class Arg, int id>
    struct HasUpdateWithIndex<F,Arg,id,void_t< TryCallOfUpdateWithIndex<F,Arg,id> > > : std::true_type {};
  }

  template <class F, class Arg,
            std::enable_if_t<!Detail::HasUpdateWithoutIndex<F,Arg>::value>* = nullptr>
  inline void update_if_present(F&&, Arg&&)
  {}

  template <class F, class Arg,
            std::enable_if_t<Detail::HasUpdateWithoutIndex<F,Arg>::value>* = nullptr>
  inline void update_if_present(F&& f, Arg&& x)
  {
    f.update(x);
  }

  template <int id, class F, class Arg,
            std::enable_if_t<!Detail::HasUpdateWithIndex<F,Arg,id>::value>* = nullptr>
  inline void update_if_present(F&&, Arg&&)
  {}

  template <int id, class F, class Arg,
            std::enable_if_t<Detail::HasUpdateWithIndex<F,Arg,id>::value>* = nullptr>
  inline void update_if_present(F&& f, Arg&& x)
  {
    f.template update<id>(x);
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
