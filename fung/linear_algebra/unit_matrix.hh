// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

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
    /** @addtogroup LinearAlgebraGroup
     *  @{ */

    /// Compute unit matrix for the specified constant size matrix type. This requires that a corresponding specialization of Zero is provided.
    template <class Matrix, class = std::enable_if_t<Checks::isConstantSize<Matrix>()> >
    Matrix unitMatrix()
    {
      Matrix A = zero<Matrix>();
      for(int i=0; i<dim<Matrix>(); ++i) at(A,i,i) = 1;
      return A;
    }

    /// Compute unit matrix for the specified dynamic size matrix type. This requires that a corresponding specialization of Zero is provided.
    template <class Matrix, class = std::enable_if_t<!Checks::isConstantSize<Matrix>()> >
    Matrix unitMatrix(int rows)
    {
      Matrix A = zero<Matrix>(rows,rows);
      for(int i=0; i<rows; ++i) at(A,i,i) = 1;
      return A;
    }

    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_UNIT_MATRIX_HH
