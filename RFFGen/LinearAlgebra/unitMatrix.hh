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

#ifndef RFFGEN_LINEAR_ALGEBRA_UNIT_MATRIX_HH
#define RFFGEN_LINEAR_ALGEBRA_UNIT_MATRIX_HH

#include "../Util/at.hh"
#include "../Util/voider.hh"
#include "../Util/zero.hh"
#include "dimension.hh"
#include "extractRowsAndCols.hh"

namespace RFFGen
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute unit matrix for the specified constant size matrix type. This requires that a corresponding specialization of Zero is provided.
     */
    template <class Matrix, class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
    Matrix unitMatrix()
    {
      Matrix A = zero<Matrix>();
      for(int i=0; i<dimension<Matrix>(); ++i) at(A,i,i) = 1;
      return A;
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute unit matrix for the specified dynamic size matrix type. This requires that a corresponding specialization of Zero is provided.
     */
    template <class Matrix, class = std::enable_if_t<!Checks::isConstantSizeMatrix<Matrix>()> >
    Matrix unitMatrix(int rows)
    {
      Matrix A = zero<Matrix>(rows,rows);
      for(int i=0; i<rows; ++i) at(A,i,i) = 1;
      return A;
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_UNIT_MATRIX_HH
