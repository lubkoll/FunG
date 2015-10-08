// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_ADDMISSINGOPERATORS_HH
#define FUNG_UTIL_ADDMISSINGOPERATORS_HH

#include <type_traits>
#include "base.hh"
#include "static_checks.hh"

namespace FunG
{
  /// Defines operator+ if not yet defined and in-place summation (operator+=()) is supported.
  template < class Arg ,
             class = std::enable_if_t< !std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>() > ,
             class = std::enable_if_t< !Checks::summation<Arg>() &&
                                       Checks::inPlaceSummation<Arg>() > >
  auto operator+(Arg x, const Arg& y)
  {
    x += y;
    return x;
  }

  /// Defines operator* for multiplication with built-in arithmetic types from the left if undefined and in-place multiplication (operator*=()) is supported.
  template < class Arg , class ScalarArg ,
             class = std::enable_if_t< std::is_arithmetic<ScalarArg>::value > ,
             class = std::enable_if_t< !std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>() > ,
             class = std::enable_if_t< !Checks::multiplicationWithArithmeticFromLeft<Arg,ScalarArg>() &&
                                       Checks::inPlaceMultiplicationWithArithmetic<Arg,ScalarArg>() > >
  auto operator*( ScalarArg a , Arg x )
  {
    x *= a;
    return x;
  }

  /// Defines operator* for multiplication with built-in arithmetic types from the right if undefined and in-place multiplication (operator*=()) is supported.
  template < class Arg , class ScalarArg ,
             class = std::enable_if_t< std::is_arithmetic<ScalarArg>::value > ,
             class = std::enable_if_t< !std::is_base_of<Base,Arg>() && !std::is_arithmetic<Arg>::value> ,
             class = std::enable_if_t< !Checks::multiplicationWithArithmeticFromRight<Arg,ScalarArg>() &&
                                       Checks::inPlaceMultiplicationWithArithmetic<Arg,ScalarArg>() > >
  auto operator*( Arg x , ScalarArg a )
  {
    x *= a;
    return x;
  }

  /// Defines operator* for multiplication of non-arithmetic types if undefined and in-place multiplication (operator*=()) is supported.
  template < class Arg1 , class Arg2 ,
             class = std::enable_if_t< !std::is_base_of<Arg1,Base>() && !std::is_base_of<Arg2,Base>() > ,
             class = std::enable_if_t< !std::is_arithmetic<Arg1>() && !std::is_arithmetic<Arg2>() > ,
             class = std::enable_if_t< !Checks::multiplication<Arg1,Arg2>() &&
                                       Checks::inPlaceMultiplication<Arg1,Arg2>() > >
  auto operator*( Arg1 x, const Arg2& y)
  {
    x *= y;
    return x;
  }

  /// Defines operator* for multiplication of non-arithmetic types if undefined and in-place multiplication is provided in terms of the member function rightmultiplyany() (such as for Dune::FieldMatrix).
  template < class Arg1 , class Arg2 ,
             class = std::enable_if_t< !std::is_base_of<Arg1,Base>() && !std::is_base_of<Arg2,Base>() > ,
             class = std::enable_if_t< !std::is_arithmetic<Arg1>() && !std::is_arithmetic<Arg2>() > ,
             class = std::enable_if_t< !Checks::multiplication<Arg1,Arg2>() &&
                                       !Checks::inPlaceMultiplication<Arg1,Arg2>() > ,
             class = std::enable_if_t< Checks::callToRightMultiply<Arg1,Arg2>() > >
  auto operator*( Arg1 x , const Arg2& y)
  {
    return x.rightmultiplyany(y);
  }
}

#endif // FUNG_UTIL_ADDMISSINGOPERATORS_HH
