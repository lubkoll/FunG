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

#ifndef FUNG_LINEAR_ALGEBRA_UNIT_MATRIX_HH
#define FUNG_LINEAR_ALGEBRA_UNIT_MATRIX_HH

#include "fung/util/at.hh"
#include "fung/util/extract_rows_and_cols.hh"
#include "fung/util/voider.hh"
#include "fung/util/zero.hh"
#include "dimension.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute unit matrix for the specified constant size matrix type. This requires that a corresponding specialization of Zero is provided.
     */
    template <class Matrix, class = std::enable_if_t<Checks::isConstantSize<Matrix>()> >
    Matrix unitMatrix()
    {
      Matrix A = zero<Matrix>();
      for(int i=0; i<dim<Matrix>(); ++i) at(A,i,i) = 1;
      return A;
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute unit matrix for the specified dynamic size matrix type. This requires that a corresponding specialization of Zero is provided.
     */
    template <class Matrix, class = std::enable_if_t<!Checks::isConstantSize<Matrix>()> >
    Matrix unitMatrix(int rows)
    {
      Matrix A = zero<Matrix>(rows,rows);
      for(int i=0; i<rows; ++i) at(A,i,i) = 1;
      return A;
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_UNIT_MATRIX_HH
