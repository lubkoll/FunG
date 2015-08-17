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

#ifndef FUNG_EXAMPLES_SKIN_HENDRIKS_HH
#define FUNG_EXAMPLES_SKIN_HENDRIKS_HH

#include "../../LinearAlgebra/strainTensor.hh"
#include "../../LinearAlgebra/unitMatrix.hh"
#include "../../LinearAlgebra/principalInvariants.hh"
#include "../../generate.hh"

/**
 * \ingroup Biomechanics
 * \file skinTissue_Hendriks.hh
 * \brief Versions of the skin model of Hendriks: Mechanical behavior of human epidermal and dermal layers in vivo. PhD thesis, Technische Universiteit Eindhoven, 2005.
 */

namespace FunG
{
  /**
   * \ingroup Biomechanics
   * \brief Model for skin tissue of Hendriks: Mechanical behavior of human epidermal and dermal layers in vivo. PhD thesis, Technische Universiteit Eindhoven, 2005.
   *
   * Implementation of the stored energy function
   * \f$W(F)=c_0(\iota_1-3) + c_1(\iota_1-3)(\iota_2-3)\f$, where \f$\iota_1,\iota_2\f$ are the first and second principal invariants of the strain tensor \f$F^T F\f$.
   *
   * @param c0 scaling of the shifted first principal invariant
   * @param c1 scaling of the product of shifted first and second principal invariant
   * @param F initial deformation gradient
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleSkin_Hendriks(double c0, double c1, const Matrix& F)
  {
    using namespace LinearAlgebra;
    auto i1 = ShiftedFirstPrincipalInvariant<Matrix,offset>();
    auto i2 = ShiftedSecondPrincipalInvariant<Matrix,offset>();

    auto S = LeftCauchyGreenStrainTensor<Matrix>(F);
    return ( c0*i1 + c1*i1*i2 ) << S;

  }

  /**
   * \ingroup Biomechanics
   * \brief Model for skin tissue of Hendriks: Mechanical behavior of human epidermal and dermal layers in vivo. PhD thesis, Technische Universiteit Eindhoven, 2005.
   *
   * Implementation of the stored energy function
   * \f$W(F)=c_0(\iota_1-3) + c_1(\iota_1-3)(\iota_2-3)\f$, where \f$\iota_1,\iota_2\f$ are the first and second principal invariants of the strain tensor \f$F^T F\f$.
   *
   * Material parameters are taken from Xu and Lu: Introduction to Skin Biothermomechanics and Thermal Pain, chapter Skin Biomechanics Modeling, pages 154-206, Springer and Science Press Beijing, 2011,
   * i.e \f$c_0=9.4 (\,\mathrm{kPa})\f$ and \f$ c_1 = 82 (\,\mathrm{kPa}) \f$.
   *
   * @param F initial deformation gradient
   */
  template <class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleSkin_Hendriks(const Matrix& F)
  {
    return incompressibleSkin_Hendriks<Matrix,offset>(9.4,82.,F);
  }


  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for skin tissue of Hendriks: Mechanical behavior of human epidermal and dermal layers in vivo. PhD thesis, Technische Universiteit Eindhoven, 2005.
   *
   * Implementation of the stored energy function
   * \f$W(F)=c_0(\iota_1-3) + c_1(\iota_1-3)(\iota_2-3) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression}\f$, where \f$\iota_1,\iota_2\f$ are the first and second principal invariants of the strain tensor \f$F^T F\f$.
   *
   * @param c0 scaling of the shifted first principal invariant
   * @param c1 scaling of the product of shifted first and second principal invariant
   * @param d0 scaling of the penalty function for inflation
   * @param d1 scaling of the penalty function for compression
   * @param F initial deformation gradient
   */
  template <class InflationPenalty, class CompressionPenalty, class Matrix, int offset = LinearAlgebra::dimension<Matrix>()>
  auto compressibleSkin_Hendriks(double c0, double c1, double d0, double d1, const Matrix& F)
  {
    return incompressibleSkin_Hendriks<Matrix,offset>(c0,c1,F) + volumetricPenalty<InflationPenalty,CompressionPenalty>(d0,d1,F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for skin tissue of Hendriks: Mechanical behavior of human epidermal and dermal layers in vivo. PhD thesis, Technische Universiteit Eindhoven, 2005.
   *
   * Implementation of the stored energy function
   * \f$W(F)=c_0(\iota_1-3) + c_1(\iota_1-3)(\iota_2-3) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression}\f$, where \f$\iota_1,\iota_2\f$ are the first and second principal invariants of the strain tensor \f$F^T F\f$.
   *
   * Material parameters are taken from Xu and Lu: Introduction to Skin Biothermomechanics and Thermal Pain, chapter Skin Biomechanics Modeling, pages 154-206, Springer and Science Press Beijing, 2011,
   * i.e \f$c_0=9.4 (\,\mathrm{kPa})\f$ and \f$ c_1 = 82 (\,\mathrm{kPa}) \f$.
   *
   * @param d0 scaling of the penalty function for inflation
   * @param d1 scaling of the penalty function for compression
   * @param F initial deformation gradient
   */
  template < class InflationPenalty , class CompressionPenalty , class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
  auto compressibleSkin_Hendriks(double d0, double d1, const Matrix& F)
  {
    return compressibleSkin_Hendriks<InflationPenalty,CompressionPenalty,Matrix,offset>(9.4,82.,d0,d1,F);
  }
}

#endif // FUNG_EXAMPLES_SKIN_HENDRIKS_HH
