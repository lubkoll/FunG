// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_STATIC_CHECKS_HH
#define FUNG_STATIC_CHECKS_HH

#include <type_traits>
#include <utility>

#include "extract_rows_and_cols.hh"
#include "type_traits.hh"
#include "voider.hh"

namespace FunG
{
  namespace Checks
  {
    /** @addtogroup ConceptGroup
     *  @{ */

    /// @cond
    template <class F>
    using TryMemOp_callable                         =      decltype(std::declval<F>()());

    template <class F>
    using TryMemFn_d0                               =      decltype(std::declval<F>().d0());

    template < class F , class IndexedArg ,
               class Arg = typename IndexedArg::type ,
               int id = IndexedArg::index >
    using TryMemFn_d1                               =      decltype(std::declval<F>().template d1<id>(std::declval<Arg>()));

    template < class F , class IndexedArg ,
               class Arg = typename IndexedArg::type >
    using TryMemFn_d1_without_index                 =      decltype(std::declval<F>().d1(std::declval<Arg>()));

    template < class F , class IndexedArgX , class IndexedArgY ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type ,
               int idx = IndexedArgX::index ,
               int idy = IndexedArgY::index >
    using TryMemFn_d2                               =      decltype(std::declval<F>().template d2<idx,idy>(std::declval<ArgX>(),std::declval<ArgY>()));

    template < class F , class IndexedArgX , class IndexedArgY ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type >
    using TryMemFn_d2_without_index                 =      decltype(std::declval<F>().d2(std::declval<ArgX>(),std::declval<ArgY>()));

    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type ,
               class ArgZ = typename IndexedArgZ::type ,
               int idx = IndexedArgX::index ,
               int idy = IndexedArgY::index ,
               int idz = IndexedArgZ::index >
    using TryMemFn_d3                                =     decltype(std::declval<F>().template d3<idx,idy,idz>(std::declval<ArgX>(),std::declval<ArgY>(),std::declval<ArgZ>()));

    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
               class ArgX = typename IndexedArgX::type ,
               class ArgY = typename IndexedArgY::type ,
               class ArgZ = typename IndexedArgZ::type >
    using TryMemFn_d3_without_index                  =     decltype( std::declval<F>().d3(std::declval<ArgX>(),std::declval<ArgY>(),std::declval<ArgZ>()) );


    template < class Arg1 , class Arg2 >
    using TryFree_Multiplication                     =     decltype( std::declval<Arg1>() * std::declval<Arg2>() );
    template < class Arg1 , class Arg2 >
    using TryMemFn_InPlaceMultiplication             =     decltype( std::declval<Arg1>() *= std::declval<Arg2>() );
    template < class Arg1 , class Arg2 >
    using TryMemFn_rightmultiplyany                  =     decltype( std::declval<Arg1>().rightmultiplyany(std::declval<Arg2>()) );

    template < class Arg >
    using TryFree_Summation                          =     decltype( std::declval<Arg>() + std::declval<Arg>() );
    template < class Arg >
    using TryMemFn_InPlaceSummation                  =     decltype( std::declval<Arg>() += std::declval<Arg>() );

    template <class Matrix>
    using TryMemFn_SquareBracketAccessForMatrix      =     decltype( std::declval<Matrix>()[0][0] );
    template <class Matrix>
    using TryMemFn_RoundBracketAccessForMatrix       =     decltype( std::declval<Matrix>()(0,0) );
    template <class Vector>
    using TryMemFn_SquareBracketAccessForVector      =     decltype( std::declval<Vector>()[0] );
    template <class Vector>
    using TryMemFn_RoundBracketAccessForVector       =     decltype( std::declval<Vector>()(0) );


    template < class F , class = void >
    struct HasMemOp_callable
        : std::false_type {};

    template <class F>
    struct HasMemOp_callable< F , void_t< TryMemOp_callable<F> > >
        : std::true_type
    {};

    template < class F , class = void >
    struct HasMemFn_d0
        : std::false_type {};

    template <class F>
    struct HasMemFn_d0< F , void_t< TryMemFn_d0<F> > >
        : std::true_type
    {};

    template < class F , class IndexedArg , class = void>
    struct HasMemFn_d1
        : std::false_type {};

    template < class F , class IndexedArg >
    struct HasMemFn_d1< F , IndexedArg , void_t< TryMemFn_d1<F,IndexedArg> > >
        : std::true_type
    {};

    template < class F , class IndexedArg >
    struct HasMemFn_d1< F , IndexedArg , void_t< TryMemFn_d1_without_index<F,IndexedArg> > >
        : std::true_type
    {};


    template < class F , class IndexedArgX , class IndexedArgY , class = void >
    struct HasMemFn_d2
        : std::false_type {};

    template < class F , class IndexedArgX , class IndexedArgY >
    struct HasMemFn_d2< F , IndexedArgX , IndexedArgY , void_t< TryMemFn_d2<F,IndexedArgX,IndexedArgY> > >
        : std::true_type
    {};

    template < class F , class IndexedArgX , class IndexedArgY >
    struct HasMemFn_d2< F , IndexedArgX , IndexedArgY , void_t< TryMemFn_d2_without_index<F,IndexedArgX,IndexedArgY> > >
        : std::true_type
    {};


    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ , class = void>
    struct HasMemFn_d3
        : std::false_type {};

    template <class F, class IndexedArgX, class IndexedArgY, class IndexedArgZ>
    struct HasMemFn_d3< F , IndexedArgX , IndexedArgY , IndexedArgZ , void_t< TryMemFn_d3<F,IndexedArgX,IndexedArgY,IndexedArgZ> > >
        : std::true_type
    {};

    template <class F, class IndexedArgX, class IndexedArgY, class IndexedArgZ>
    struct HasMemFn_d3< F , IndexedArgX , IndexedArgY , IndexedArgZ , void_t< TryMemFn_d3_without_index<F,IndexedArgX,IndexedArgY,IndexedArgZ> > >
        : std::true_type
    {};


    template < class F , class IndexedArg , class = void >
    struct HasMemFn_d1_with_index
        : std::false_type {};

    template < class F , class IndexedArg >
    struct HasMemFn_d1_with_index< F , IndexedArg , void_t< TryMemFn_d1<F,IndexedArg> > >
        : std::true_type {};


    template < class F , class IndexedArgX , class IndexedArgY , class = void >
    struct HasMemFn_d2_with_index
        : std::false_type {};

    template < class F , class IndexedArgX , class IndexedArgY >
    struct HasMemFn_d2_with_index< F , IndexedArgX , IndexedArgY , void_t< TryMemFn_d2<F,IndexedArgX,IndexedArgY> > >
        : std::true_type {};


    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ , class = void >
    struct HasMemFn_d3_with_index : std::false_type {};

    template < class F , class IndexedArgX , class IndexedArgY , class IndexedArgZ >
    struct HasMemFn_d3_with_index< F , IndexedArgX , IndexedArgY , IndexedArgZ ,
        void_t< TryMemFn_d3<F,IndexedArgX,IndexedArgY,IndexedArgZ> > >
        : std::true_type {};


    template < class Arg1 , class Arg2 ,
               bool = !is_arithmetic<Arg1>::value && !is_arithmetic<Arg2>::value &&
                      !HasMemOp_callable<Arg1>::value && !HasMemOp_callable<Arg2>::value ,
               class = void >
    struct HasFree_Multiplication : std::false_type {};

    template < class Arg1 , class Arg2 >
    struct HasFree_Multiplication< Arg1 , Arg2 , false , void > : std::true_type {};

    template < class Arg1 , class Arg2 >
    struct HasFree_Multiplication< Arg1 , Arg2 , true , void_t< TryFree_Multiplication<Arg1,Arg2> > > : std::true_type {};


    template < class Arg1 , class Arg2 , bool , class = void >
    struct HasMemFn_InPlaceMultiplication
        : std::false_type {};

    template < class Arg1 , class Arg2 >
    struct HasMemFn_InPlaceMultiplication< Arg1 , Arg2 , true , void_t< TryMemFn_InPlaceMultiplication<Arg1,Arg2> > >
        : std::true_type {};


    template < class Arg1 , class Arg2 , bool , class = void >
    struct HasMemFn_rightmultiplany
        : std::false_type {};

    template < class Arg1 , class Arg2 >
    struct HasMemFn_rightmultiplany< Arg1 , Arg2 , true , void_t< TryMemFn_rightmultiplyany<Arg1,Arg2> > >
        : std::true_type {};


    template < class Arg ,  bool , class = void >
    struct HasFree_Summation
        : std::false_type {};

    template < class Arg >
    struct HasFree_Summation< Arg , true , void_t< TryFree_Summation<Arg> > >
        : std::true_type {};

    template < class Arg ,  bool , class = void >
    struct HasMemFn_InPlaceSummation
        : std::false_type {};

    template < class Arg >
    struct HasMemFn_InPlaceSummation< Arg , true , void_t< TryMemFn_InPlaceSummation<Arg> > >
        : std::true_type {};


    template < class Matrix, class = void >
    struct HasMemFn_SquareBracketAccessForMatrix
        : std::false_type {};
    template <class Matrix>
    struct HasMemFn_SquareBracketAccessForMatrix< Matrix , void_t<TryMemFn_SquareBracketAccessForMatrix<Matrix> > >
        : std::true_type  {};

    template < class Matrix, class = void >
    struct HasMemFn_RoundBracketAccessForMatrix
        : std::false_type {};
    template <class Matrix>
    struct HasMemFn_RoundBracketAccessForMatrix< Matrix , void_t<TryMemFn_RoundBracketAccessForMatrix<Matrix> > >
        : std::true_type  {};

    template < class Vector, class = void >
    struct HasMemFn_SquareBracketAccessForVector
        : std::false_type {};
    template <class Vector>
    struct HasMemFn_SquareBracketAccessForVector< Vector , void_t<TryMemFn_SquareBracketAccessForVector<Vector> > >
        : std::true_type  {};

    template < class Vector, class = void >
    struct HasMemFn_RoundBracketAccessForVector
        : std::false_type {};
    template <class Vector>
    struct HasMemFn_RoundBracketAccessForVector< Vector , void_t<TryMemFn_RoundBracketAccessForVector<Vector> > >
        : std::true_type  {};


    template <class EigenArg, class = void>
    struct HasNestedType_PlainObject
        : std::false_type {};
    template <class EigenArg>
    struct HasNestedType_PlainObject< EigenArg , void_t< TryNestedType_PlainObject<EigenArg> > >
        : std::true_type  {};
    /// @endcond


    template < class F >
    constexpr bool isFunction()
    {
      return HasMemOp_callable<F>::value;
    }

    template < class F >
    constexpr bool hasConsistentFirstDerivative()
    {
      return HasMemOp_callable<F>::value;
    }

    /// Check if objects of typed Arg1 and Arg2 support multiplication (free operator*).
    template < class Arg1 , class Arg2 >
    constexpr bool hasFree_Multiplication()
    {
      return HasFree_Multiplication<Arg1,Arg2>::value;
    }

    /// Check if objects of type Arg1 support in-place multiplication with objects of type Arg2.
    template < class Arg1 , class Arg2 >
    constexpr bool hasMemFn_InPlaceMultiplication()
    {
      return HasMemFn_InPlaceMultiplication<Arg1,Arg2,!isFunction<Arg1>() && !isFunction<Arg2>() && !is_arithmetic<Arg1>() >::value;
    }

    /// Check if objects of type Arg1 support multiplication with objects of type Arg2 via call to rightmultiplyany(Arg2).
    template < class Arg1 , class Arg2 >
    constexpr bool hasMemFn_rightmultiplyany()
    {
      return HasMemFn_rightmultiplany<Arg1,Arg2,!isFunction<Arg1>() && !isFunction<Arg2>() && !is_arithmetic<Arg1>() && !is_arithmetic<Arg2>()>::value;
    }

    /// Check if objects of type Arg support summation.
    template < class Arg >
    constexpr bool hasFree_Summation()
    {
      return HasFree_Summation<Arg,!isFunction<Arg>() && !is_arithmetic<Arg>()>::value;
    }

    /// Check if objects of type Arg support in-place summation.
    template < class Arg >
    constexpr bool hasMemFn_InPlaceSummation()
    {
      return HasMemFn_InPlaceSummation<Arg,!isFunction<Arg>() && !is_arithmetic<Arg>()>::value;
    }

    /**
     * @brief Check if object is a static vector for some type satisfying Concepts::VectorConcept.
     *
     * Checks if number of rows is positive.
     */
    template < class Arg >
    constexpr bool isConstantSize()
    {
      return LinearAlgebra::NumberOfRows<Arg>::value > 0;
    }


    template < class F , class IndexedArgX , class IndexedArgY >
    constexpr bool hasConsistentSecondDerivative()
    {
      return hasConsistentFirstDerivative<F>() &&
          ( HasMemFn_d2<F,IndexedArgX,IndexedArgY>::value ?  HasMemFn_d1<F,IndexedArgX>::value : true );
    }

    template < class F, class IndexedArgX , class IndexedArgY , class IndexedArgZ >
    constexpr bool hasConsistentThirdDerivative()
    {
      return hasConsistentSecondDerivative<F,IndexedArgX,IndexedArgY>() &&
          ( HasMemFn_d3<F,IndexedArgX,IndexedArgY,IndexedArgZ>::value ? HasMemFn_d2<F,IndexedArgX,IndexedArgY>::value : true );
    }

    /** @} */
  }
}

#endif // FUNG_STATIC_CHECKS_HH
