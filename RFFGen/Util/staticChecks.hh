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

#include "../LinearAlgebra/extractRowsAndCols.hh"
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


    // Update and updateVariable functions.
    template < class F >
    using TryCallOfUpdate = decltype(std::declval<F>().update(std::declval<int>()));

    template < class F >
    using TryCallOfUpdateVariable = decltype(std::declval<F>().template updateVariable<0>(std::declval<int>()));

    template < class F , class = void > struct CallOfUpdate                                                           : std::false_type {};
    template < class F , class = void > struct CallOfUpdateVariable                                                   : std::false_type {};

    template < class F > struct CallOfUpdate< F , void_t< TryCallOfUpdate<F> > >                                      : std::true_type {};
    template < class F > struct CallOfUpdateVariable< F , void_t< TryCallOfUpdateVariable<F> > >                      : std::true_type {};
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

    /**
     * \ingroup Checks
     * \brief Check if object is a static matrix for some type satisfying Concepts::MatrixConcept.
     *
     * Checks if number of rows and number of columns are positive.
     */
    template < class Arg >
    constexpr bool isConstantSizeMatrix()
    {
      return ( LinearAlgebra::numberOfRows<Arg>() > 0 ) && ( LinearAlgebra::numberOfColumns<Arg>() > 0 );
    }

    /**
     * \ingroup Checks
     * \brief Check if object is a static vector for some type satisfying Concepts::VectorConcept.
     *
     * Checks if number of rows is positive.
     */
    template < class Arg >
    constexpr bool isStaticVector()
    {
      return ( LinearAlgebra::numberOfRows<Arg>() > 0 ) && ( LinearAlgebra::numberOfColumns<Arg>() == -1 );
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Arg support access to its entries via square brackets, i.e. via A[i][j] for matrices or v[i] for vectors.
     */
    template < class Arg >
    constexpr bool accessViaSquareBrackets()
    {
      return std::conditional_t< !isConstantSizeMatrix<Arg>() && isStaticVector<Arg>() ,
                                 AccessViaSquareBracketsForVector<Arg> ,
                                 AccessViaSquareBracketsForMatrix<Arg> >::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if objects of type Matrix support access to its entries via round brackets, i.e. via A(i,j).
     */
    template < class Arg >
    constexpr bool accessViaRoundBrackets()
    {
      return std::conditional_t< !isConstantSizeMatrix<Arg>() && isStaticVector<Arg>() ,
                                 AccessViaRoundBracketsForVector<Arg> ,
                                 AccessViaRoundBracketsForMatrix<Arg> >::value;
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
}

#endif // RFFGEN_STATIC_CHECKS_HH
