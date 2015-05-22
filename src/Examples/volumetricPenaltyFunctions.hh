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

#ifndef RFFGEN_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH
#define RFFGEN_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH

#include "../LinearAlgebra/determinant.hh"
#include "../CMath/log.hh"
#include "../CMath/pow.hh"
#include "../generate.hh"

namespace RFFGen
{
  /// Create volumetric penalty function composed of a penalty for inflation and one for compression.
  template <class Inflation, class Compression, class Matrix>
  auto volumetricPenalty(double d0, double d1, const Matrix& A)
  {
    auto det = LinearAlgebra::Determinant<Matrix>(A);
    auto f = ( d0*Inflation(det()) + d1*Compression(det()) ) << det;
    auto g = f - f.d0();
    return g;
  }

  /// Create the volumetric penalty function \f$ d_0 j^2 + d_1 \log(j),\ j=\det(A) \f$.
  template <class Matrix>
  auto volumetricQuadAndLog(double d0, double d1, const Matrix& A)
  {
    return volumetricPenalty< CMath::Pow<2> , CMath::LN >(d0,d1,A);
  }

  /// Create the volumetric penalty function \f$ d_0 j^5 + d_1 j^{-5},\ j=\det(A) \f$.
  template <class Matrix>
  auto volumetricHartmannNeff(double d0, double d1, const Matrix& A)
  {
    return volumetricPenalty< CMath::Pow<5> , CMath::Pow<-5> >(d0,d1,A);
  }
}

#endif // RFFGEN_EXAMPLES_VOLUMETRIC_PENALTY_FUNCTIONS_HH
