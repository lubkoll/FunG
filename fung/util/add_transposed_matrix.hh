// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_ADDTRANSPOSEDMATRIX_HH
#define FUNG_UTIL_ADDTRANSPOSEDMATRIX_HH

#include <cassert>
#include <type_traits>
#include "fung/linear_algebra/dimension.hh"
#include "static_checks.hh"

namespace FunG
{
  /**
   * @brief Overwrites \f$A\f$ with \f$A+A^T\f$.
   * @return \f$A+A^T\f$
   */
  template <class Matrix ,
            std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
  Matrix addTransposed(Matrix& A)
  {
    using LinearAlgebra::dim;
    using Index = decltype(dim<Matrix>());
    for(Index i=0; i<dim<Matrix>(); ++i)
      for(Index j=i+1; j<dim<Matrix>(); ++j)
        at(A,j,i) = at(A,i,j) = at(A,i,j) + at(A,j,i);
    for(Index i=0; i<dim<Matrix>(); ++i) at(A,i,i) *= 2;
    return A;
  }

  /**
   * @brief Overwrites \f$A\f$ with \f$A+A^T\f$.
   * @return \f$A+A^T\f$
   */
  template <class Matrix ,
            std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
  Matrix addTransposed(Matrix& A)
  {
    using LinearAlgebra::rows;
    using LinearAlgebra::cols;
    assert( rows(A) == cols(A) );
    using Index = decltype(rows(std::declval<Matrix>()));
    for(Index i=0; i<rows(A); ++i)
      for(Index j=i+1; j<cols(A); ++j)
        at(A,j,i) = at(A,i,j) = at(A,i,j) + at(A,j,i);
    for(Index i=0; i<rows(A); ++i) at(A,i,i) *= 2;
    return A;
  }
}

#endif // FUNG_UTIL_ADDTRANSPOSEDMATRIX_HH
