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

#ifndef RFFGEN_UTIL_AT_HH
#define RFFGEN_UTIL_AT_HH

#include <type_traits>
#include <utility>
#include "static_checks.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace AtDetail
  {


    template <class Matrix, bool accessViaSquareBrackets, bool accessViaRoundBrackets>
    struct ValueTypeImpl;

    template <class Matrix, bool accessViaRoundBrackets>
    struct ValueTypeImpl<Matrix,true,accessViaRoundBrackets>
    {
      using type = decltype(std::declval<Matrix>()[0][0]);
    };

    template <class Matrix>
    struct ValueTypeImpl<Matrix,false,true>
    {
      using type = decltype(std::declval<Matrix>()(0,0));
    };

    template <class Matrix>
    using ValueType = ValueTypeImpl<Matrix,Checks::accessViaSquareBrackets<Matrix>(),Checks::accessViaRoundBrackets<Matrix>()>;

    template <class Matrix, bool whatever>
    __attribute__((always_inline)) auto& at(Matrix& A, int i, int j, std::true_type, std::integral_constant<bool,whatever>)
    {
      return A[i][j];
    }

    template <class Matrix, bool whatever>
    __attribute__((always_inline)) auto& at(Matrix& A, int i, int j, std::false_type, std::integral_constant<bool,whatever>)
    {
      return A(i,j);
    }
  }

  /// Access underlying value type of Matrix.
  template <class Matrix>
  using at_t = typename AtDetail::ValueType<Matrix>::type;


  template <class Matrix>
  __attribute__((always_inline)) auto& at(Matrix& A, int i, int j)
  {
    return AtDetail::at<Matrix>(A,i,j,std::integral_constant<bool,Checks::accessViaSquareBrackets<Matrix>()>(),
                                      std::integral_constant<bool,Checks::accessViaRoundBrackets<Matrix>()>());
  }
  /**
   * \endcond
   */
}

#endif // RFFGEN_UTIL_AT_HH
