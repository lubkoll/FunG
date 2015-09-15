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

#ifndef FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH
#define FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH

#include "fung/generate.hh"
#include "fung/cmath/exp.hh"
#include "fung/linear_algebra/principal_invariants.hh"
#include "fung/linear_algebra/mixed_invariants.hh"
#include "fung/linear_algebra/strain_tensor.hh"
#include "fung/linear_algebra/tensor_product.hh"

/**
 * \ingroup Biomechanics
 * \file adiposeTissue_SommerHolzapfel.hh
 * \brief Model for adipose tissue of \cite Sommer2013.
 */

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
    auto generateIncompressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, const Matrix& M, const Matrix& F)
    {
      using CMath::exp;
      using namespace LinearAlgebra;
      auto i1 = FirstPrincipalInvariant<Matrix>(F);
      auto si1 = ShiftedFirstPrincipalInvariant<Matrix,offset>(F);
      auto i4 = FirstMixedInvariant<Matrix>(F,M);

      auto aniso = kappa*i1 + (1-3*kappa)*i4 - 1;
      return ( cCells*si1 +
               (k1/k2)*( exp( k2*(aniso^2) ) - 1)
             ) << LeftCauchyGreenStrainTensor<Matrix>(F);
    }
  }
  /**
   * \endcond
   */

  /**
   * \ingroup Biomechanics
   * \brief Model for adipose tissue of \cite Sommer2013.
   *
   * Implementation of the stored energy function
   * \f$ W(F)= c_\mathrm{Cells}(\iota_1-3) + \frac{k_1}{k_2}\exp(k_2(\kappa\iota_1+(1-3\kappa)*\iota_4)^2-1) \f$, where \f$ \iota_1,\iota_4 \f$ are the first and first mixed invariant of the
   * strain tensor \f$F^T F\f$.
   *
   * @param cCells scaling of the neo-Hookean model for the description of the adipocytes as cell foam.
   * @param k1 stress-like parameter of the model for the interlobular septa
   * @param k2 dimensionless parameter of the model for the interlobular septa
   * @param kappa fiber dispersion parameter \f$(0\le\kappa\le\frac{1}{3})\f$.
   * @param M structural tensor describing the fiber direction of the interlobular septa, i.e. \f$M=v\otimesv\f$ for a fiber direction \f$v\f$
   * @param F initial deformation gradient
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, const Matrix& M, const Matrix& F)
  {
    return Detail::generateIncompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(cCells,k1,k2,kappa,M,F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Model for adipose tissue of \cite Sommer2013.
   *
   * Implementation of the stored energy function
   * \f$ W(F)= c_\mathrm{Cells}(\iota_1-3) + \frac{k_1}{k_2}\exp(k_2(\kappa\iota_1+(1-3\kappa)*\iota_4)^2-1) \f$, where \f$ \iota_1,\iota_4 \f$ are the first and first mixed invariant of the
   * strain tensor \f$F^T F\f$.
   *
   * Material parameters are taken from the above mentioned publication, Table 2, i.e. \f$c_\mathrm{Cells}=0.15 (\,\mathrm{kPa})\f$, \f$k_1=0.8 (\,\mathrm{kPa})\f$, \f$k_2=47.3\f$ and \f$\kappa=0.09\f$.
   *
   * @param M structural tensor describing the fiber direction of the interlobular septa, i.e. \f$M=v\otimesv\f$ for a fiber direction \f$v\f$
   * @param F initial deformation gradient
   */
  template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto incompressibleAdiposeTissue_SommerHolzapfel(const Matrix& M, const Matrix& F)
  {
    return incompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(0.15,0.8,47.3,0.09,M,F);
  }


  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for adipose tissue of \cite Sommer2013.
   *
   * Implementation of the stored energy function
   * \f$ W(F)= c_\mathrm{Cells}(\iota_1-3) + \frac{k_1}{k_2}\exp(k_2(\kappa\iota_1+(1-3\kappa)*\iota_4)^2-1) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression}  \f$, where \f$ \iota_1,\iota_4 \f$ are the first and first mixed invariant of the
   * strain tensor \f$F^T F\f$.
   *
   * @param cCells scaling of the neo-Hookean model for the description of the adipocytes as cell foam.
   * @param k1 stress-like parameter of the model for the interlobular septa
   * @param k2 dimensionless parameter of the model for the interlobular septa
   * @param kappa fiber dispersion parameter \f$(0\le\kappa\le\frac{1}{3})\f$.
   * @param M structural tensor describing the fiber direction of the interlobular septa, i.e. \f$M=v\otimesv\f$ for a fiber direction \f$v\f$
   * @param d0 scaling of the penalty function for inflation
   * @param d1 scaling of the penalty function for compression
   * @param F initial deformation gradient
   */
  template <class Inflation, class Compression, class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto compressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return Detail::generateIncompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(cCells,k1,k2,kappa,M,F) + volumetricPenalty<Inflation,Compression>(d0,d1,F);
  }

  /**
   * \ingroup Biomechanics
   * \brief Compressible version of the model for adipose tissue of \cite Sommer2013.
   * Material parameters are taken from the same publication, Table 2, i.e. \f$c_\mathrm{Cells}=0.15 (\,\mathrm{kPa})\f$, \f$k_1=0.8 (\,\mathrm{kPa})\f$, \f$k_2=47.3\f$ and \f$\kappa=0.09\f$.
   *
   * Implementation of the stored energy function
   * \f$ W(F)= c_\mathrm{Cells}(\iota_1-3) + \frac{k_1}{k_2}\exp(k_2(\kappa\iota_1+(1-3\kappa)*\iota_4)^2-1) + d_0\Gamma_\mathrm{Inflation}(\det(F)) + d_1\Gamma_\mathrm{Compression}  \f$, where \f$ \iota_1,\iota_4 \f$ are the first and first mixed invariant of the
   * strain tensor \f$F^T F\f$.
   *
   * @param d0 scaling of the penalty function for inflation
   * @param d1 scaling of the penalty function for compression
   * @param M structural tensor describing the fiber direction of the interlobular septa, i.e. \f$M=v\otimesv\f$ for a fiber direction \f$v\f$
   * @param F initial deformation gradient
   */
  template <class Inflation, class Compression, class Matrix , int offset = LinearAlgebra::dimension<Matrix>()>
  auto compressibleAdiposeTissue_SommerHolzapfel(double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return compressibleAdiposeTissue_SommerHolzapfel<Inflation,Compression,Matrix,offset>(0.15,0.8,47.3,0.09,d0,d1,M,F);
  }
}

#endif // FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH
