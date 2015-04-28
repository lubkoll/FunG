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

#ifndef RFFGEN_LINEAR_ALGEBRA_TRANSPOSE_HH
#define RFFGEN_LINEAR_ALGEBRA_TRANSPOSE_HH

#include <type_traits>

#include "extractRowsAndCols.hh"
#include "../Util/zero.hh"
#include "../Util/at.hh"

namespace RFFGen
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute transpose of square matrix.
     */
    template <class Matrix, class TransposedMatrix = Matrix , class = std::enable_if_t<std::is_same<Matrix,TransposedMatrix>::value> >
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
    template <class Matrix, class TransposedMatrix , class = std::enable_if_t<!std::is_same<Matrix,TransposedMatrix>::value> >
    TransposedMatrix transpose(const Matrix& A)
    {
      TransposedMatrix B = zero<TransposedMatrix>();
      for(int i=0; i<numberOfRows<Matrix>(); ++i)
        for(int j=0; j<numberOfColumns<Matrix>(); ++j)
          at(B,j,i) = A(i,j);
      return B;
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_TRANSPOSE_HH
