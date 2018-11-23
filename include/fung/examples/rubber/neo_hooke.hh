// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

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
    return finalize( c*(i1(strainTensor(F)) - n) );
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
    return finalize( c*(mi1<decltype(S),n>(S) - n) );
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
    return finalize( c*(i1(strainTensor(F)) - n) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F) );
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
    return finalize( c*(mi1<decltype(S),n>(S) - n) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F) );
  }
}

#endif // FUNG_NEO_HOOKE_HH
