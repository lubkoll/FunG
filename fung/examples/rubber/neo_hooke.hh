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

#ifndef FUNG_NEO_HOOKE_HH
#define FUNG_NEO_HOOKE_HH

#include "fung/finalize.hh"
#include "fung/generate.hh"
#include "fung/linear_algebra/principal_invariants.hh"
#include "fung/linear_algebra/strain_tensor.hh"
#include "fung/linear_algebra/unit_matrix.hh"
#include "fung/examples/volumetric_penalty_functions.hh"

/**
 * \ingroup Rubber
 * \file neo_hooke.hh
 * \brief Models based on the neo-Hookean material law. Input argument is the deformation gradient.
 */

namespace FunG
{
  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\iota_1(F^T F) \f$, where \f$\iota_1\f$ is the first principal matrix invariant .
   */
  template < class Matrix , int n = LinearAlgebra::dim<Matrix>() >
  auto incompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return finalize( c*(i1(S) - n) );
  }

  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F) \f$, where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template < class Matrix , int n = LinearAlgebra::dim<Matrix>() >
  auto modifiedIncompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return c*(mi1<decltype(S),n>(S) - n);
  }

  /**
   * \ingroup Rubber
   * \brief Generate a compressible neo-Hookean material law \f$ W(F)=c\iota_1(F^T F)+d_0\Gamma_\mathrm{In}(\det(F))+d_1\Gamma_\mathrm{Co}(\det(F)) \f$,
   * where \f$\iota_1\f$ is the first principal matrix invariant.
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix , int n = LinearAlgebra::dim<Matrix>() >
  auto compressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    using namespace LinearAlgebra;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return c*(i1(S) - n) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F);
  }

  /**
   * \ingroup Rubber
   * \brief Generate a compressible neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F)+d_0\Gamma_\mathrm{In}(\det(F))+d_1\Gamma_\mathrm{Co}(\det(F)) \f$,
   * where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix , int n = LinearAlgebra::dim<Matrix>() >
  auto modifiedCompressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    using namespace LinearAlgebra;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return c*(mi1<decltype(S),n>(S) - n) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F);
  }
}

#endif // FUNG_NEO_HOOKE_HH
