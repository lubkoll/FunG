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

#ifndef RFFGEN_STATIC_CHECKS_HH
#define RFFGEN_STATIC_CHECKS_HH

#include <type_traits>
#include <utility>

#include "base.hh"
#include "voider.hh"

/*!
 * \file static_checks.hh
 * \brief Static checks for detection of presence of different operators and functions.
 */

namespace RFFGen
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
        : std::true_type//std::integral_constant<bool,std::is_convertible<decltype( std::declval<ScalarArg>() * std::declval<Arg>() ),Arg>::value>
    {};

    template < class Arg , class ScalarArg >
    struct MultiplicationWithArithmeticFromRight< Arg , ScalarArg , true , void_t< TryMultiplicationWithArithmeticFromRight<Arg,ScalarArg> > >
        : std::true_type//std::integral_constant<bool,std::is_convertible< decltype( std::declval<Arg>() * std::declval<ScalarArg>() ) , Arg >::value>
    {};

    template < class Arg , class ScalarArg >
    struct InPlaceMultiplicationWithArithmetic< Arg , ScalarArg , true , void_t< TryInPlaceMultiplicationWithArithmetic<Arg,ScalarArg> > >
        : std::true_type//std::integral_constant<bool,std::is_convertible< decltype( std::declval<Arg>() *= std::declval<ScalarArg>() ) , Arg& >::value>
    {};


    // Multiplication
    template < class Arg1 , class Arg2 > using TryMultiplication = decltype( std::declval<Arg1>() * std::declval<Arg2>() );
    template < class Arg1 , class Arg2 > using TryInPlaceMultiplication = decltype( std::declval<Arg1>() *= std::declval<Arg2>() );
    template < class Arg1 , class Arg2 > using TryCallToRightMultiply = decltype( std::declval<Arg1>().rightmultiplyany(std::declval<Arg2>()) );

    template < class Arg1 , class Arg2 , bool , class = void > struct Multiplication                        : std::false_type {};
    template < class Arg1 , class Arg2 , bool , class = void > struct InPlaceMultiplication                 : std::false_type {};
    template < class Arg1 , class Arg2 , bool , class = void > struct CallToRightMultiply                   : std::false_type {};

    template < class Arg1 , class Arg2 >
    struct Multiplication< Arg1 , Arg2 , true , void_t< TryMultiplication<Arg1,Arg2> > >
        : std::true_type
    {};

    template < class Arg1 , class Arg2 >
    struct InPlaceMultiplication< Arg1 , Arg2 , true , void_t< TryInPlaceMultiplication<Arg1,Arg2> > >
        : std::true_type
    {};

    template < class Arg1 , class Arg2 >
    struct CallToRightMultiply< Arg1 , Arg2 , true , void_t< TryCallToRightMultiply<Arg1,Arg2> > >
        : std::true_type
    {};


    // Summation
    template < class Arg > using TrySummation = decltype( std::declval<Arg>() + std::declval<Arg>() );
    template < class Arg > using TryInPlaceSummation = decltype( std::declval<Arg>() += std::declval<Arg>() );

    template < class Arg ,  bool , class = void > struct Summation                                          : std::false_type {};
    template < class Arg ,  bool , class = void > struct InPlaceSummation                                   : std::false_type {};

    template < class Arg >
    struct Summation< Arg , true , void_t< TrySummation<Arg> > >
        : std::true_type//std::integral_constant<bool,std::is_convertible< decltype( std::declval<Arg>() + std::declval<Arg>() ) , Arg >::value>
    {};

    template < class Arg >
    struct InPlaceSummation< Arg , true , void_t< TryInPlaceSummation<Arg> > >
        : std::true_type//std::integral_constant<bool,std::is_convertible< decltype( std::declval<Arg>() += std::declval<Arg>() ) , Arg& >::value>
    {};


    // Access to matrix entries
    template <class Matrix>
    using TryAccessViaSquareBrackets = decltype(std::declval<Matrix>()[0][0]);

    template <class Matrix>
    using TryAccessViaRoundBrackets = decltype(std::declval<Matrix>()(0,0));


    template < class Matrix, class = void > struct AccessViaSquareBrackets : std::false_type {};

    template <class Matrix>
    struct AccessViaSquareBrackets< Matrix , void_t<TryAccessViaSquareBrackets<Matrix> > > : std::true_type {};


    template < class Matrix, class = void > struct AccessViaRoundBrackets  : std::false_type {};

    template <class Matrix>
    struct AccessViaRoundBrackets< Matrix , void_t<TryAccessViaRoundBrackets<Matrix> > > : std::true_type {};


    // Update and updateVariable functions.
    template < class F >
    using TryCallOfUpdate = decltype(std::declval<F>().update(std::declval<int>()));

    template < class F >
    using TryCallOfUpdateVariable = decltype(std::declval<F>().template updateVariable<0>(std::declval<int>()));

    template < class F , class = void > struct CallOfUpdate                 : std::false_type {};
    template < class F , class = void > struct CallOfUpdateVariable         : std::false_type {};

    template < class F >
    struct CallOfUpdate< F , void_t< TryCallOfUpdate<F> > >                 : std::true_type {};

    template < class F >
    struct CallOfUpdateVariable< F , void_t< TryCallOfUpdateVariable<F> > > : std::true_type {};
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
      return MultiplicationWithArithmeticFromLeft<Arg,ScalarArg,!std::is_base_of<Base,Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support multiplication with objects of type ScalarArg from the right.
     */
    template < class Arg , class ScalarArg >
    constexpr bool multiplicationWithArithmeticFromRight()
    {
      return MultiplicationWithArithmeticFromRight<Arg,ScalarArg,!std::is_base_of<Base,Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support in-place multiplication with objects of type ScalarArg.
     */
    template < class Arg , class ScalarArg >
    constexpr bool inPlaceMultiplicationWithArithmetic()
    {
      return InPlaceMultiplicationWithArithmetic<Arg,ScalarArg,!std::is_base_of<Base,Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of typed Arg1 and Arg2 support multiplication (free operator*).
     */
    template < class Arg1 , class Arg2 >
    constexpr bool multiplication()
    {
      return Multiplication<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg1 support in-place multiplication with objects of type Arg2.
     */
    template < class Arg1 , class Arg2 >
    constexpr bool inPlaceMultiplication()
    {
      return InPlaceMultiplication<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg1 support multiplication with objects of type Arg2 via call to rightmultiplyany(Arg2).
     */
    template < class Arg1 , class Arg2 >
    constexpr bool callToRightMultiply()
    {
      return CallToRightMultiply<Arg1,Arg2,!std::is_base_of<Base,Arg1>() && !std::is_base_of<Base,Arg2>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support summation.
     */
    template < class Arg >
    constexpr bool summation()
    {
      return Summation<Arg,!std::is_base_of<Base,Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support in-place summation.
     */
    template < class Arg >
    constexpr bool inPlaceSummation()
    {
      return InPlaceSummation<Arg,!std::is_base_of<Base,Arg>()>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Matrix support access to its entries via square brackets, i.e. via A[i][j].
     */
    template < class Matrix >
    constexpr bool accessViaSquareBrackets()
    {
      return AccessViaSquareBrackets<Matrix>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Matrix support access to its entries via round brackets, i.e. via A(i,j).
     */
    template < class Matrix >
    constexpr bool accessViaRoundBrackets()
    {
      return AccessViaRoundBrackets<Matrix>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if object of type F has a member function update().
     */
    template < class F >
    constexpr bool hasUpdateFunction()
    {
      return CallOfUpdate<F>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if object of type F has a member function updateVariable().
     */
    template < class F >
    constexpr bool hasUpdateVariableFunction()
    {
      return CallOfUpdateVariable<F>::value;
    }
  }
  /**
   * \defgroup Checks Checks
   * \brief Static checks for the presence of different operators and functions.
   */
}

#endif // RFFGEN_STATIC_CHECKS_HH
