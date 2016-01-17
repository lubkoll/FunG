// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

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
    /** @addtogroup LinearAlgebraGroup
     *  @{ */

    /// Compute tensor product \f$ M = v \otimes w \f$.
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

    /// Compute tensor product \f$ M = v \otimes v \f$.
    template <class Matrix, class Vector>
    Matrix tensorProduct(const Vector& v)
    {
      return tensorProduct<Matrix>(v,v);
    }

    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TENSOR_PRODUCT_HH
