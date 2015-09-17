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

#ifndef FUNG_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH
#define FUNG_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH

#include "fung/util/at.hh"
#include "rows_and_cols.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/zero.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute tensor product \f$ M = v \otimes w \f$.
     */
    template <class Matrix, class Vector1, class Vector2,
              std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr>
    Matrix tensorProduct(const Vector1& v, const Vector2& w)
    {
      auto result = zero<Matrix>();

      for( int i=0; i<rows<Matrix>(); ++i )
        for( int j=0; j<cols<Matrix>(); ++j )
          at(result,i,j) = v[i] * w[j];

      return result;
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute tensor product \f$ M = v \otimes v \f$.
     */
    template <class Matrix, class Vector>
    Matrix tensorProduct(const Vector& v)
    {
      return tensorProduct<Matrix>(v,v);
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH
