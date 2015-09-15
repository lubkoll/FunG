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
#include "fung/linear_algebra/invariants.hh"
#include "fung/linear_algebra/strain_tensor.hh"
#include "fung/linear_algebra/unit_matrix.hh"
#include "fung/examples/volumetric_penalty_functions.hh"

/**
 * \ingroup Rubber
 * \file neoHooke.hh
 * \brief Models based on the neo-Hookean material law. Input argument is the deformation gradient.
 */

namespace FunG
{
  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\iota_1(F^T F) \f$, where \f$\iota_1\f$ is the first principal matrix invariant .
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
  auto incompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    using Inv = typename InvariantTraits<Invariant::PRINCIPAL>::template ShiftedFirstInvariant<Matrix,offset>;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    auto f = c*Inv(S()) << S;
    return Finalize<decltype(f)>(f);
  }

  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F) \f$, where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
  auto modifiedIncompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    using Inv = typename InvariantTraits<Invariant::MODIFIED>::template ShiftedFirstInvariant<Matrix,offset>;
    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return c * Inv(S()) << S;
  }

  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace NeoHookeDetail
  {
    template <class InflationPenalty, class CompressionPenalty, class Matrix, LinearAlgebra::Invariant I = LinearAlgebra::Invariant::PRINCIPAL , int offset = LinearAlgebra::dimension<Matrix>() >
    auto compressibleNeoHookeImpl(double c, double d0, double d1, const Matrix& F)
    {
      using namespace LinearAlgebra;
      using Inv = typename InvariantTraits<I>::template ShiftedFirstInvariant<Matrix,offset>;
      auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
      return ( c*Inv(F) << S ) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F);
      //return Finalize<decltype(f)>(f);
    }
  }
  /**
   * \endcond
   */

  /**
   * \ingroup Rubber
   * \brief Generate a compressible neo-Hookean material law \f$ W(F)=c\iota_1(F^T F)+d_0\Gamma_\mathrm{In}(\det(F))+d_1\Gamma_\mathrm{Co}(\det(F)) \f$,
   * where \f$\iota_1\f$ is the first principal matrix invariant.
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
  auto compressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    return NeoHookeDetail::compressibleNeoHookeImpl<InflationPenalty,CompressionPenalty,Matrix,LinearAlgebra::Invariant::PRINCIPAL,offset>(c,d0,d1,F);
  }

  /**
   * \ingroup Rubber
   * \brief Generate a compressible neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F)+d_0\Gamma_\mathrm{In}(\det(F))+d_1\Gamma_\mathrm{Co}(\det(F)) \f$,
   * where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
  auto modifiedCompressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    return NeoHookeDetail::compressibleNeoHookeImpl<InflationPenalty,CompressionPenalty,Matrix,LinearAlgebra::Invariant::MODIFIED,offset>(c,d0,d1,F);
  }
}

#endif // FUNG_NEO_HOOKE_HH
