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

#ifndef RFFGEN_NEO_HOOKE_HH
#define RFFGEN_NEO_HOOKE_HH

#include "../../LinearAlgebra/invariants.hh"
#include "../../LinearAlgebra/strainTensor.hh"
#include "../../LinearAlgebra/unitMatrix.hh"
#include "../../finalize.hh"
#include "../volumetricPenaltyFunctions.hh"
#include "../../generate.hh"

/**
 * \ingroup Rubber
 * \file neoHooke.hh
 * \brief Models based on the neo-Hookean material law. Input argument is the deformation gradient.
 */

namespace RFFGen
{
  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\iota_1(F^T F) \f$, where \f$\iota_1\f$ is the first principal matrix invariant .
   */
  template <class Matrix>
  auto incompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    using Inv = typename InvariantTraits<Invariant::PRINCIPAL>::template ShiftedFirstInvariant<Matrix>;
    auto S = CauchyGreenStrainTensor<Matrix>(F);
    auto f = c*Inv() << S;
    return Finalize<decltype(f)>(f);
  }

  /**
   * \ingroup Rubber
   * \brief Generate an "incompressible" neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F) \f$, where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template <class Matrix>
  auto modifiedIncompressibleNeoHooke(double c, const Matrix& F)
  {
    using namespace LinearAlgebra;
    using Inv = typename InvariantTraits<Invariant::MODIFIED>::template ShiftedFirstInvariant<Matrix>;
    auto S = CauchyGreenStrainTensor<Matrix>(F);
    return c*Inv() << S;
  }

  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace NeoHookeDetail
  {
    template <class InflationPenalty, class CompressionPenalty, class Matrix, LinearAlgebra::Invariant I = LinearAlgebra::Invariant::PRINCIPAL>
    auto compressibleNeoHookeImpl(double c, double d0, double d1, const Matrix& F)
    {
      using namespace LinearAlgebra;
      using Inv = typename InvariantTraits<I>::template ShiftedFirstInvariant<Matrix>;
      auto S = CauchyGreenStrainTensor<Matrix>(F);
      return ( c*Inv() << S ) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F);
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
  template <class InflationPenalty, class CompressionPenalty, class Matrix>
  auto compressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    return NeoHookeDetail::compressibleNeoHookeImpl<InflationPenalty,CompressionPenalty,Matrix>(c,d0,d1,F);
  }

  /**
   * \ingroup Rubber
   * \brief Generate a compressible neo-Hookean material law \f$ W(F)=c\bar\iota_1(F^T F)+d_0\Gamma_\mathrm{In}(\det(F))+d_1\Gamma_\mathrm{Co}(\det(F)) \f$,
   * where \f$\bar\iota_1\f$ is the modified first principal matrix invariant.
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix>
  auto modifiedCompressibleNeoHooke(double c, double d0, double d1, const Matrix& F)
  {
    return NeoHookeDetail::compressibleNeoHookeImpl<InflationPenalty,CompressionPenalty,Matrix,LinearAlgebra::Invariant::MODIFIED>(c,d0,d1,F);
  }
}

#endif // RFFGEN_NEO_HOOKE_HH
