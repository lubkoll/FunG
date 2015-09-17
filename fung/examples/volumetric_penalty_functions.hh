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

#ifndef FUNG_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH
#define FUNG_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH

#include "fung/generate.hh"
#include "fung/cmath/log.hh"
#include "fung/cmath/pow.hh"
#include "fung/linear_algebra/determinant.hh"

namespace FunG
{
  /// Create volumetric penalty function composed of a penalty for inflation and one for compression.
  template <class Inflation, class Compression, class Matrix>
  auto volumetricPenalty(double d0, double d1, const Matrix& A)
  {
    using LinearAlgebra::det;
    auto f = ( d0*Inflation() + d1*Compression() ) ( det(A) );
    return f - f.d0();
  }

  /// Create the volumetric penalty function \f$ d_0 j^2 + d_1 \log(j),\ j=\det(A) \f$.
  template <class Matrix>
  auto volumetricQuadAndLog(double d0, double d1, const Matrix& A)
  {
    return volumetricPenalty< Pow<2> , LN >(d0,d1,A);
  }

  /// Create the volumetric penalty function \f$ d_0 j^5 + d_1 j^{-5},\ j=\det(A) \f$.
  template <class Matrix>
  auto volumetricHartmannNeff(double d0, double d1, const Matrix& A)
  {
    return volumetricPenalty< Pow<5> , Pow<-5> >(d0,d1,A);
  }
}

#endif // FUNG_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH
