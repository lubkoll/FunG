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

#ifndef RFFGEN_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH
#define RFFGEN_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH

#include "extractRowsAndCols.hh"
#include "../Util/at.hh"
#include "../Util/zero.hh"

namespace RFFGen
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute tensor product \f$ M = v \otimes w \f$.
     */
    template <class Matrix, class Vector1, class Vector2>
    Matrix tensorProduct(const Vector1& v, const Vector2& w)
    {
      auto result = zero<Matrix>();

      for( int i=0; i<numberOfRows<Matrix>(); ++i )
        for( int j=0; j<numberOfColumns<Matrix>(); ++j )
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

#endif // RFFGEN_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH
