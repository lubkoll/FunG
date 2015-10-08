// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_STATIC_CHECKS_HH
#define FUNG_STATIC_CHECKS_HH

#include <type_traits>
#include <utility>

#include "base.hh"
#include "extract_rows_and_cols.hh"
#include "voider.hh"

/*!
 * \file static_checks.hh
 * \brief Static checks for detection of presence of different operators and functions.
 */

namespace FunG
{
  namespace Checks
  {
    /**
     * \cond DOCUMENT_IMPLEMENTATIONS_DETAILS
     */
    // Multiplication with doubles
    template < class Arg , class ScalarArg > using TryMultiplicationWithArithmeticFromLeft  = decltype( std::declval<ScalarArg>() * std::declval<Arg>() );
    template < class Arg , class ScalarArg > using TryMultiplicationWithArithmeticFromRight = decltype( std::declval<Arg>() * std::declval<ScalarArg>() );
    template < class Arg , class ScalarArg > using TryInPlaceMultiplicationWithArithmetic   = decltype( std::declval<Arg>() *= std::declval<ScalarArg>() );

    template < class Arg , class ScalarArg , bool , class = void > struct MultiplicationWithArithmeticFromLeft                   : std::false_type {};
    template < class Arg , class ScalarArg , bool , class = void > struct MultiplicationWithArithmeticFromRight                  : std::false_type {};
    template < class Arg , class ScalarArg , bool , class = void > struct InPlaceMultiplicationWithArithmetic                    : std::false_type {};

    template < class Arg , class ScalarArg >
    struct MultiplicationWithArithmeticFromLeft< Arg , ScalarArg , true , void_t< TryMultiplicationWithArithmeticFromLeft<Arg,ScalarArg> > >
        : std::true_type
    {};

    template < class Arg , class ScalarArg >
    struct MultiplicationWithArithmeticFromRight< Arg , ScalarArg , true , void_t< TryMultiplicationWithArithmeticFromRight<Arg,ScalarArg> > >
        : std::true_type
    {};

    template < class Arg , class ScalarArg >
    struct InPlaceMultiplicationWithArithmetic< Arg , ScalarArg , true , void_t< TryInPlaceMultiplicationWithArithmetic<Arg,ScalarArg> > >
        : std::true_type
    {};


    // Multiplication
    template < class Arg1 , class Arg2 > using TryMultiplication = decltype( std::declval<Arg1>() * std::declval<Arg2>() );
    template < class Arg1 , class Arg2 > using TryInPlaceMultiplication = decltype( std::declval<Arg1>() *= std::declval<Arg2>() );
    template < class Arg1 , class Arg2 > using TryCallToRightMultiply = decltype( std::declval<Arg1>().rightmultiplyany(std::declval<Arg2>()) );

    template < class Arg1 , class Arg2 , bool , class = void > struct Multiplication                                  : std::false_type {};
    template < class Arg1 , class Arg2 , bool , class = void > struct InPlaceMultiplication                           : std::false_type {};
    template < class Arg1 , class Arg2 , bool , class = void > struct CallToRightMultiply                             : std::false_type {};

    template < class Arg1 , class Arg2 >
    struct Multiplication< Arg1 , Arg2 , true , void_t< TryMultiplication<Arg1,Arg2> > >                              : std::true_type {};

    template < class Arg1 , class Arg2 >
    struct InPlaceMultiplication< Arg1 , Arg2 , true , void_t< TryInPlaceMultiplication<Arg1,Arg2> > >                : std::true_type {};

    template < class Arg1 , class Arg2 >
    struct CallToRightMultiply< Arg1 , Arg2 , true , void_t< TryCallToRightMultiply<Arg1,Arg2> > >                    : std::true_type {};


    // Summation
    template < class Arg > using TrySummation = decltype( std::declval<Arg>() + std::declval<Arg>() );
    template < class Arg > using TryInPlaceSummation = decltype( std::declval<Arg>() += std::declval<Arg>() );

    template < class Arg ,  bool , class = void > struct Summation                                                    : std::false_type {};
    template < class Arg ,  bool , class = void > struct InPlaceSummation                                             : std::false_type {};

    template < class Arg > struct Summation< Arg , true , void_t< TrySummation<Arg> > >                               : std::true_type {};
    template < class Arg > struct InPlaceSummation< Arg , true , void_t< TryInPlaceSummation<Arg> > >                 : std::true_type {};


    // Access to vector or matrix entries
    template <class Matrix>
    using TryAccessViaSquareBracketsForMatrix = decltype(std::declval<Matrix>()[0][0]);

    template <class Vector>
    using TryAccessViaSquareBracketsForVector = decltype(std::declval<Vector>()[0]);

    template <class Matrix>
    using TryAccessViaRoundBracketsForMatrix = decltype(std::declval<Matrix>()(0,0));

    template <class Vector>
    using TryAccessViaRoundBracketsForVector = decltype(std::declval<Vector>()(0));

    template < class Matrix, class = void > struct AccessViaSquareBracketsForMatrix                                                     : std::false_type {};
    template <class Matrix> struct AccessViaSquareBracketsForMatrix< Matrix , void_t<TryAccessViaSquareBracketsForMatrix<Matrix> > >    : std::true_type {};

    template < class Matrix, class = void > struct AccessViaRoundBracketsForMatrix                                                      : std::false_type {};
    template <class Matrix> struct AccessViaRoundBracketsForMatrix< Matrix , void_t<TryAccessViaRoundBracketsForMatrix<Matrix> > >      : std::true_type {};

    template < class Vector, class = void > struct AccessViaSquareBracketsForVector                                                     : std::false_type {};
    template <class Vector> struct AccessViaSquareBracketsForVector< Vector , void_t<TryAccessViaSquareBracketsForVector<Vector> > >    : std::true_type {};

    template < class Vector, class = void > struct AccessViaRoundBracketsForVector                                                      : std::false_type {};
    template <class Vector> struct AccessViaRoundBracketsForVector< Vector , void_t<TryAccessViaRoundBracketsForVector<Vector> > >      : std::true_type {};

    template <class EigenArg>
    using TryAccessToPlainObject = typename EigenArg::PlainObject;

    template <class EigenArg, class = void>
    struct HasTypePlainObject : std::false_type {};

    template <class EigenArg>
    struct HasTypePlainObject<EigenArg,void_t<TryAccessToPlainObject<EigenArg> > > : std::true_type{};
    /**
     * \endcond
     */


    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support multiplication with objects of type ScalarArg from the left.
     */
    template < class Arg , class ScalarArg >
    constexpr bool multiplicationWithArithmeticFromLeft()
    {
      return MultiplicationWithArithmeticFromLeft<Arg,ScalarArg,!std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support multiplication with objects of type ScalarArg from the right.
     */
    template < class Arg , class ScalarArg >
    constexpr bool multiplicationWithArithmeticFromRight()
    {
      return MultiplicationWithArithmeticFromRight<Arg,ScalarArg,!std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support in-place multiplication with objects of type ScalarArg.
     */
    template < class Arg , class ScalarArg >
    constexpr bool inPlaceMultiplicationWithArithmetic()
    {
      return InPlaceMultiplicationWithArithmetic<Arg,ScalarArg,!std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of typed Arg1 and Arg2 support multiplication (free operator*).
     */
    template < class Arg1 , class Arg2 >
    constexpr bool multiplication()
    {
      return Multiplication<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>() && !std::is_arithmetic<Arg1>() && !std::is_arithmetic<Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg1 support in-place multiplication with objects of type Arg2.
     */
    template < class Arg1 , class Arg2 >
    constexpr bool inPlaceMultiplication()
    {
      return InPlaceMultiplication<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>() && !std::is_arithmetic<Arg1>() && !std::is_arithmetic<Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg1 support multiplication with objects of type Arg2 via call to rightmultiplyany(Arg2).
     */
    template < class Arg1 , class Arg2 >
    constexpr bool callToRightMultiply()
    {
      return CallToRightMultiply<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>() && !std::is_arithmetic<Arg1>() && !std::is_arithmetic<Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support summation.
     */
    template < class Arg >
    constexpr bool summation()
    {
      return Summation<Arg,!std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support in-place summation.
     */
    template < class Arg >
    constexpr bool inPlaceSummation()
    {
      return InPlaceSummation<Arg,!std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>()>::value;
    }

//    /**
//     * \ingroup Checks
//     * \brief Check if object is a static matrix for some type satisfying Concepts::MatrixConcept.
//     *
//     * Checks if number of rows and number of columns are positive.
//     */
//    template < class Arg >
//    constexpr bool isConstantSizeMatrix()
//    {
//      return ( LinearAlgebra::numberOfRows<Arg>() > 0 ) && ( LinearAlgebra::numberOfColumns<Arg>() > 0 );
//    }

    /**
     * \ingroup Checks
     * \brief Check if object is a static vector for some type satisfying Concepts::VectorConcept.
     *
     * Checks if number of rows is positive.
     */
    template < class Arg >
    constexpr bool isConstantSize()
    {
      return ( LinearAlgebra::numberOfRows<Arg>() > 0 );
    }
  }
}

#endif // FUNG_STATIC_CHECKS_HH
