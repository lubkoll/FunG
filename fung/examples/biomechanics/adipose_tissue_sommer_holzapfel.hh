// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH
#define FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH

#include "fung/finalize.hh"
#include "fung/generate.hh"
#include "fung/cmath/exp.hh"
#include "fung/linear_algebra/principal_invariants.hh"
#include "fung/linear_algebra/mixed_invariants.hh"
#include "fung/linear_algebra/strain_tensor.hh"
#include "fung/linear_algebra/tensor_product.hh"
#include "fung/examples/volumetric_penalty_functions.hh"

/**
 * \ingroup Biomechanics
 * \file adipose_tissue_sommer_holzapfel.hh
 * \brief Model for adipose tissue of \cite Sommer2013.
 */

namespace FunG
{
  /// @cond
  namespace Detail
  {
    template < class Matrix , int n = LinearAlgebra::dim<Matrix>() >
    auto generateIncompressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, const Matrix& M, const Matrix& F)
    {
      using namespace LinearAlgebra;
      auto S = LeftCauchyGreenStrainTensor<Matrix>(F);

      auto aniso = kappa*i1(F) + (1-3*kappa)*i4(F,M) - 1;
      auto f = cCells*( i1(F) - n ) + (k1/k2)*( exp( k2*(aniso^2) ) - 1);
      return f(S);
    }
  }
  /// @endcond

  /** @addtogroup Biomechanics
   *  @{ */

  /**
   * @brief Model for adipose tissue of \cite Sommer2013.
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
   * @tparam offset number of rows/columns of F, this is only required to adjust the offset of the energy functional such that \f$W(F)=0\f$ for \f$F=I\f$.
   */
  template < class Matrix , int offset = LinearAlgebra::dim<Matrix>()>
  auto incompressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, const Matrix& M, const Matrix& F)
  {
    return finalize( Detail::generateIncompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(cCells,k1,k2,kappa,M,F) );
  }

  /**
   * @brief Model for adipose tissue of \cite Sommer2013.
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
  template < class Matrix , int offset = LinearAlgebra::dim<Matrix>()>
  auto incompressibleAdiposeTissue_SommerHolzapfel(const Matrix& M, const Matrix& F)
  {
    return incompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(0.15,0.8,47.3,0.09,M,F);
  }


  /**
   * @brief Compressible version of the model for adipose tissue of \cite Sommer2013.
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
  template <class Inflation, class Compression, class Matrix , int offset = LinearAlgebra::dim<Matrix>()>
  auto compressibleAdiposeTissue_SommerHolzapfel(double cCells, double k1, double k2, double kappa, double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return finalize( Detail::generateIncompressibleAdiposeTissue_SommerHolzapfel<Matrix,offset>(cCells,k1,k2,kappa,M,F) + volumetricPenalty<Inflation,Compression>(d0,d1,F) );
  }

  /**
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
  template <class Inflation, class Compression, class Matrix , int offset = LinearAlgebra::dim<Matrix>()>
  auto compressibleAdiposeTissue_SommerHolzapfel(double d0, double d1, const Matrix& M, const Matrix& F)
  {
    return compressibleAdiposeTissue_SommerHolzapfel<Inflation,Compression,Matrix,offset>(0.15,0.8,47.3,0.09,d0,d1,M,F);
  }
  /** @} */
}

#endif // FUNG_ADIPOSE_TISSUE_SOMMER_HOLZAPFEL_HH
