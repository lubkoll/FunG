// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
#define FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH

#include <type_traits>

#include "fung/util/at.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/zero.hh"
#include "rows_and_cols.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /** @addtogroup LinearAlgebraGroup
     *  @{ */
    template <class Matrix, class TransposedMatrix = Matrix ,
              std::enable_if_t<std::is_same<Matrix,TransposedMatrix>::value && Checks::isConstantSize<Matrix>()>* = nullptr>
    TransposedMatrix transpose(Matrix A)
    {
      auto a = at(A,0,0);
      for(int i=0; i<rows<Matrix>(); ++i)
        for(int j=i+1; j<cols<Matrix>(); ++j)
        {
          a = at(A,i,j);
          at(A,i,j) = at(A,j,i);
          at(A,j,i) = a;
        }

      return A;
    }

    /// Compute transpose of non-square matrix.
    template <class TransposedMatrix, class Matrix ,
              std::enable_if_t<!std::is_same<Matrix,TransposedMatrix>::value && Checks::isConstantSize<Matrix>() && Checks::isConstantSize<TransposedMatrix>()>* = nullptr >
    TransposedMatrix transpose(const Matrix& A)
    {
      TransposedMatrix B = zero<TransposedMatrix>();
      for(int i=0; i<rows<Matrix>(); ++i)
        for(int j=0; j<cols<Matrix>(); ++j)
          at(B,j,i) = A(i,j);
      return B;
    }


    /// Compute transpose of square matrix.
    template <class Matrix ,
              std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
    Matrix transpose(Matrix A)
    {
      assert(rows(A) == cols(A));
      using Index = decltype(rows(std::declval<Matrix>()));
      auto a = std::remove_const_t<std::remove_reference_t<decltype(at(A,0,0))> >(0.);
      for(Index i=0; i<rows(A); ++i)
        for(Index j=i+1; j<cols(A); ++j)
        {
          a = at(A,i,j);
          at(A,i,j) = at(A,j,i);
          at(A,j,i) = a;
        }

      return A;
    }
    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
