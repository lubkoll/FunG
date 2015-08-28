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

#ifndef FUNG_MUSCLE_TISSUE_MARTINS_HH
#define FUNG_MUSCLE_TISSUE_MARTINS_HH

#include "../../LinearAlgebra/modifiedPrincipalInvariants.hh"
#include "../../LinearAlgebra/modifiedMixedInvariants.hh"
#include "../../LinearAlgebra/strainTensor.hh"
#include "../../LinearAlgebra/tensorProduct.hh"
#include "../../CMath/exp.hh"
#include "../volumetricPenaltyFunctions.hh"
#include "../../generate.hh"

/**
 * \ingroup Biomechanics
 * \file muscleTissue_Martins.hh
 * \brief Versions of the muscle model of Martins1998.
 */

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace MuscleTissueDetail
  {
    template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
    auto generateIncompressibleMuscleTissue_Martins(double c, double b, double A, double a, const Matrix& M, const Matrix& F )
    {
      using CMath::exp;
      using namespace LinearAlgebra;
      auto i1 = ShiftedFirstModifiedPrincipalInvariant<Matrix,offset>();
      auto i6 = ShiftedThirdModifiedMixedInvariant<Matrix>(F,M);

      return ( c * ( exp( b * i1 ) - 1 ) +
               A * ( exp( a * ( i6^2 ) ) - 1 )
             ) << LeftCauchyGreenStrainTensor<Matrix>(F);
    }
  }
  /**
   * \endcond
   */

  /**
   * \ingroup Biomechanics
   * \brief Incompressible version of the model for muscle tissue of Martins1998.
   *
   * Implementation of the stored energy function
   * \f$ W(F)=c(\exp(b(\bar\iota_1-3))-1) + A(\exp(a(\bar\iota_6-1)^2)-1) \f$, where \f$\bar\iota_1,\bar\iota_6=\bar\iota_4\f$ are the first modified principal and the third modified mixed
   * invariant of the strain tensor \f$F^T F\f$.
   *
   * \param c first material parameter for the isotropic part
   * \param b second material parameter for the isotropic part
   * \param A first material parameter for the anisotropic part
   * \param a second material parameter for the anisotropic part
   * \param M structural (rank-one) tensor describing the initial orientation of muscle fibers for \f$F=I\f$, where \f$I\f$ is the unit matrix.
   * \param F deformation gradient
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleMuscleTissue_Martins(double c, double b, double A, double a, const Matrix& M, const Matrix& F)
  {
    return MuscleTissueDetail::generateIncompressibleMuscleTissue_Martins<Matrix,offset>(c,b,A,a,M,F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Incompressible version of the model for muscle tissue of Martins1998.
   *
   * Implementation of the stored energy function
   * \f$ W(F)=c(\exp(b(\bar\iota_1-3))-1) + A(\exp(a(\bar\iota_6-1)^2)-1) \f$, where \f$\bar\iota_1,\bar\iota_6=\bar\iota_4\f$ are the first modified principal and the third modified mixed
   * invariant of the strain tensor \f$F^T F\f$.
   *
   * Material parameters taken from the same above mentioned publication, i.e. \f$a=0.387 (\,\mathrm{kPa})\f$, \f$ b = 23.46 \f$, \f$ A = 0.584 (\,\mathrm{kPa}) \f$
   * and \f$ a = 12.43\f$.
   *
   * \param M structural (rank-one) tensor describing the initial orientation of muscle fibers for \f$F=I\f$, where \f$I\f$ is the unit matrix.
   * \param F deformation gradient
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleMuscleTissue_Martins(const Matrix& M, const Matrix& F)
  {
    return incompressibleMuscleTissue_Martins<Matrix,offset>(0.387, 23.46, 0.584, 12.43, M, F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for muscle tissue of Martins1998.
   *
   * Implementation of the stored energy function
   * \f$ W(F)=c(\exp(b(\bar\iota_1-3))-1) + A(\exp(a(\bar\iota_6-1)^2)-1) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression} \f$, where \f$\bar\iota_1,\bar\iota_6=\bar\iota_4\f$ are the first modified principal and the third modified mixed
   * invariant of the strain tensor \f$F^T F\f$.
   *
   * \param c first material parameter for the isotropic part
   * \param b second material parameter for the isotropic part
   * \param A first material parameter for the anisotropic part
   * \param a second material parameter for the anisotropic part
   * \param d0 material parameter for the penalty for inflation
   * \param d1 material parameter for the penalty for compression
   * \param M structural (rank-one) tensor describing the initial orientation of muscle fibers for \f$F=I\f$, where \f$I\f$ is the unit matrix.
   * \param F deformation gradient
   */
  template < class Inflation , class Compression , class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto compressibleMuscleTissue_Martins(double c, double b, double A, double a, double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return MuscleTissueDetail::generateIncompressibleMuscleTissue_Martins<Matrix,offset>(c,b,A,a,M,F) + volumetricPenalty<Inflation,Compression>(d0,d1,F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for muscle tissue of Martins1998.
   *
   * Implementation of the stored energy function
   * \f$ W(F)=c(\exp(b(\bar\iota_1-3))-1) + A(\exp(a(\bar\iota_6-1)^2)-1) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression}(\det(F))\f$, where \f$\bar\iota_1,\bar\iota_6=\bar\iota_4\f$ are the first modified principal and the third modified mixed
   * invariant of the strain tensor \f$F^T F\f$.
   *
   * Material parameters taken from the above mentioned publication, i.e. \f$a=0.387 (\,\mathrm{kPa})\f$, \f$ b = 23.46 \f$, \f$ A = 0.584 (\,\mathrm{kPa}) \f$
   * and \f$ a = 12.43\f$.
   *
   * \param d0 material parameter for the penalty for inflation
   * \param d1 material parameter for the penalty for compression
   * \param M structural (rank-one) tensor describing the initial orientation of muscle fibers for \f$F=I\f$, where \f$I\f$ is the unit matrix.
   * \param F deformation gradient
   */
  template < class Inflation , class Compression , class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto compressibleMuscleTissue_Martins(double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return compressibleMuscleTissue_Martins<Inflation,Compression,Matrix,offset>(0.387, 23.46, 0.584, 12.43, d0, d1, M, F);
  }
}

#endif // FUNG_MUSCLE_TISSUE_MARTINS_HH
