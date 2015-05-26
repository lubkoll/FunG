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

#ifndef RFFGEN_UTIL_CONSISTENCY_CHECK_HH
#define RFFGEN_UTIL_CONSISTENCY_CHECK_HH

#include <type_traits>

#include "voider.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  /// Tells us if F has a member function d0().
  template < class F , class = void >
  struct HasD0MemberFunction                  : std::false_type {};

  /// Tells us if T has a member function d1(Arg).
  template < class F , class IndexedArg , class = void>
  struct HasD1MemberFunction                  : std::false_type {};

  /// Tells us if F has a member function d2(Arg,Arg)
  template < class F , class IndexedArgX , class IndexedArgY , class = void >
  struct HasD2MemberFunction                  : std::false_type {};

  /// Tells us if F has a member function d3(Arg,Arg,Arg)
  template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ , class = void>
  struct HasD3MemberFunction                  : std::false_type {};


  namespace Detail
  {
    /* Unevaluated function calls: */
    /// Try an unevaluated call to d0 member function.
    template <class F> using check_d0_member_presence_t = decltype(std::declval<F>().d0());

    /// Try an unevaluated call to d1 member function.
    template < class F , class IndexedArg ,
               class Arg = typename IndexedArg::type ,
               int id = IndexedArg::index >
    using check_d1_member_presence_t = decltype(std::declval<F>().template d1<id>(std::declval<Arg>()));

    /// Try an unevaluated call to d2 member function.
    template < class F , class IndexedArgX , class IndexedArgY ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type ,
               int idx = IndexedArgX::index ,
               int idy = IndexedArgY::index >
    using check_d2_member_presence_t = decltype(std::declval<F>().template d2<idx,idy>(std::declval<ArgX>(),std::declval<ArgY>()));

    /// Try an unevaluated call to d3 member function.
    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type ,
               class ArgZ = typename IndexedArgZ::type ,
               int idx = IndexedArgX::index ,
               int idy = IndexedArgY::index ,
               int idz = IndexedArgZ::index >
    using check_d3_member_presence_t = decltype(std::declval<F>().template d3<idx,idy,idz>(std::declval<ArgX>(),std::declval<ArgY>(),std::declval<ArgZ>()));


    /// Check consistency of derivatives. This checks only if undefined and defined functions are defined consistently, i.e. if d2 is not present (meaning that
    /// is is identically zero), there can not be an implementation of d3.
    template <bool presence_of_d0 = false, bool presence_of_d1 = false, bool presence_of_d2 = false, bool presence_of_d3 = false>
    struct ConsistentDerivatives : std::false_type {};

    /// Specialiation for functions that provide d0,d1,d2 and d3.
    template <> struct ConsistentDerivatives<true,true,true,true>    : std::true_type {};

    /// Specialiation for functions that provide d0,d1 and d2 (quadratic functions).
    template <> struct ConsistentDerivatives<true,true,true,false>   : std::true_type {};

    /// Specialiation for functions that provide d0 and d1 (linear functions).
    template <> struct ConsistentDerivatives<true,true,false,false>  : std::true_type {};

    /// Specialiation for functions that provide d0 (constant functions).
    template <> struct ConsistentDerivatives<true,false,false,false> : std::true_type {};


    /// Statically compute the number of defined derivatives, i.e. the minimal order of a function.
    template <bool presence_of_d0 = false, bool presence_of_d1 = false, bool presence_of_d2 = false, bool presence_of_d3 = false>
    struct NumberOfDerivatives : std::integral_constant<int,-1> {};

    /// Specialiation for functions that provide d0,d1,d2 and d3.
    template <> struct NumberOfDerivatives<true,true,true,true>    : std::integral_constant<int,3> {};

    /// Specialiation for functions that provide d0,d1 and d2 (quadratic functions).
    template <> struct NumberOfDerivatives<true,true,true,false>   : std::integral_constant<int,2> {};

    /// Specialiation for functions that provide d0 and d1 (linear functions).
    template <> struct NumberOfDerivatives<true,true,false,false>  : std::integral_constant<int,1> {};

    /// Specialiation for functions that provide d0 (constant functions).
    template <> struct NumberOfDerivatives<true,false,false,false> : std::integral_constant<int,0> {};


//    /// Convenient alias that calls all member function checks for a function.
//    template <int id, template <bool,bool,bool,bool> class ConsistencyCheck, class F, class ArgX, class ArgY>
//    using FMemberCheck = ConsistencyCheck<HasD0MemberFunction<F>::value,
//                                          HasD1MemberFunction<id,F,Arg>::value,
//                                          HasD2MemberFunction<F,Arg>::value,
//                                          HasD3MemberFunction<F,Arg>::value>;
  }

  /// Specialization for the case that d0() can be called.
  template <class F>
  struct HasD0MemberFunction<F,void_t<Detail::check_d0_member_presence_t<F> > >
      : std::true_type
  {};

  /// Specialization for the case that d1(Arg) can be called.
  template < class F , class IndexedArg >
  struct HasD1MemberFunction<F,IndexedArg,void_t<Detail::check_d1_member_presence_t<F,IndexedArg> > >
      : std::true_type
  {};

  /// Specialization for the case that d2(Arg,Arg) can be called.
  template < class F , class IndexedArgX , class IndexedArgY >
  struct HasD2MemberFunction<F,IndexedArgX,IndexedArgY,void_t<Detail::check_d2_member_presence_t<F,IndexedArgX,IndexedArgY> > >
      : std::true_type
  {};

  /// Specialization for the case that d3(Arg,Arg,Arg) can be called.
  template <class F, class IndexedArgX, class IndexedArgY, class IndexedArgZ>
  struct HasD3MemberFunction<F,IndexedArgX,IndexedArgY,IndexedArgZ,void_t<Detail::check_d3_member_presence_t<F,IndexedArgX,IndexedArgY,IndexedArgZ> > >
      : std::true_type
  {};

  template <class F>
  constexpr bool hasD0MemberFunction() { return HasD0MemberFunction<F>::value; }

  template < class F , class IndexedArgX >
  constexpr bool hasD1MemberFunction() { return HasD1MemberFunction<F,IndexedArgX>::value; }

  template < class F , class IndexedArgX , class IndexedArgY >
  constexpr bool hasD2MemberFunction() { return HasD2MemberFunction<F,IndexedArgX,IndexedArgY>::value; }

  template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ >
  constexpr bool hasD3MemberFunction() { return HasD3MemberFunction<F,IndexedArgX,IndexedArgY,IndexedArgZ>::value; }

  template < class F >
  constexpr bool hasConsistentFirstDerivative()
  {
    return hasD0MemberFunction<F>();
  }

  template < class F , class IndexedArgX , class IndexedArgY >
  constexpr bool hasConsistentSecondDerivative()
  {
    return hasConsistentFirstDerivative<F>() &&
         ( hasD2MemberFunction<F,IndexedArgX,IndexedArgY>() ?  hasD1MemberFunction<F,IndexedArgX>() : true );
  }

  template < class F, class IndexedArgX , class IndexedArgY , class IndexedArgZ >
  constexpr bool hasConsistentThirdDerivative()
  {
    return hasConsistentSecondDerivative<F,IndexedArgX,IndexedArgY>() &&
         ( hasD3MemberFunction<F,IndexedArgX,IndexedArgY,IndexedArgZ>() ? hasD2MemberFunction<F,IndexedArgX,IndexedArgY>() : true );
   }


//  /// Checks if the derivatives in F are defined consistently.
//  template <int id, class F, class Arg>
//  constexpr bool hasConsistentDerivatives()
//  {
//    return Detail::FMemberCheck<id,Detail::ConsistentDerivatives,F,Arg>::value;
//  }


//  /// Statically compute the number of non-vanishing derivatives defined in F (maximum is 3).
//  template <int id, class F, class Arg>
//  constexpr int numberOfDerivatives()
//  {
//    return Detail::FMemberCheck<id, Detail::NumberOfDerivatives,F,Arg>::value;
//  }
  /**
   * \endcond
   */
}

#endif // RFFGEN_UTIL_CONSISTENCY_CHECK_HH
