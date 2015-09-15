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

#ifndef FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
#define FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH

#include <type_traits>

#include "fung/util/at.hh"
#include "fung/util/extract_rows_and_cols.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/static_checks_nrows_ncols.hh"
#include "fung/util/zero.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute transpose of square matrix.
     */
    template <class Matrix, class TransposedMatrix = Matrix ,
              class = std::enable_if_t<std::is_same<Matrix,TransposedMatrix>::value> ,
              class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
    TransposedMatrix transpose(Matrix A)
    {
      auto a = at(A,0,0);
      for(int i=0; i<numberOfRows<Matrix>(); ++i)
        for(int j=i+1; j<numberOfColumns<Matrix>(); ++j)
        {
          a = at(A,i,j);
          at(A,i,j) = at(A,j,i);
          at(A,j,i) = a;
        }

      return A;
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute transpose of non-square matrix.
     */
    template <class TransposedMatrix, class Matrix ,
              class = std::enable_if_t<!std::is_same<Matrix,TransposedMatrix>::value> ,
              class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>() && Checks::isConstantSizeMatrix<TransposedMatrix>()> >
    TransposedMatrix transpose(const Matrix& A)
    {
      TransposedMatrix B = zero<TransposedMatrix>();
      for(int i=0; i<numberOfRows<Matrix>(); ++i)
        for(int j=0; j<numberOfColumns<Matrix>(); ++j)
          at(B,j,i) = A(i,j);
      return B;
    }


    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute transpose of square matrix.
     */
    template <class Matrix ,
              class = std::enable_if_t<!Checks::isConstantSizeMatrix<Matrix>()> ,
              class = std::enable_if_t<Checks::isDynamicMatrix<Matrix>()> >
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
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
