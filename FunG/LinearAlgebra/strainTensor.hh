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

#ifndef FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH
#define FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH

#include "transpose.hh"
#include "FunG/MathematicalOperations/sum.hh"
#include "FunG/Util/base.hh"
#include "FunG/Util/chainer.hh"
#include "FunG/Util/addTransposedMatrix.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts { template <class> struct SymmetricMatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Left Cauchy-Green strain tensor \f$ F^T F \f$ for a symmetric matrix \f$ F \f$.
     *
     * This class is used for nonlinear material models based on the deformation gradient \f$\nabla\varphi\f$, which takes the role of \f$F\f$.
     * Caches both \f$ F^T \f$ and \f$ F^T F \f$.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    class LeftCauchyGreenStrainTensor :
        public Base ,
        public Chainer< LeftCauchyGreenStrainTensor<Matrix , Concepts::SymmetricMatrixConceptCheck<Matrix> > >
    {
    public:
      LeftCauchyGreenStrainTensor() = default;
      /**
       * @brief Constructor.
       * @param F point of evaluation.
       */
      explicit LeftCauchyGreenStrainTensor(Matrix const& F) { update(F); }

      /// Reset point of evaluation.
      void update(Matrix const& F)
      {
        FT = transpose(F);
        FTF = FT * F;
      }

      /// Function value \f$ F^T * F \f$.
      Matrix const& d0() const noexcept
      {
        return FTF;
      }

      /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
      template <int>
      Matrix d1(Matrix const& dF1) const
      {
        Matrix FTdF1 = FT * dF1;
        return addTransposed(FTdF1);
      }

      /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
      template <int,int>
      Matrix d2(Matrix const& dF1, Matrix const& dF2) const
      {
        Matrix dF2TdF1 = transpose(dF2) * dF1;
        return addTransposed(dF2TdF1);
      }

    private:
      Matrix FT, FTF;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Linearized strain tensor \f$ \frac{1}{2}\left(F^T+F\right) \f$.
     *
     * This class is used for linear material models based on the displacement gradient \f$\nabla u\f$, which takes the role of \f$F\f$.
     * Caches the function value \f$ \frac{1}{2}\left(F^T+F\right) \f$.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    class LinearizedStrainTensor :
        public Base ,
        public Chainer< LinearizedStrainTensor< Matrix , Concepts::SymmetricMatrixConceptCheck<Matrix> > >
    {
    public:
      /**
       * @brief Constructor.
       * @param F point of evaluation
       */
      explicit LinearizedStrainTensor(const Matrix& F) { update(F); }

      /// Reset point of evaluation.
      void update(Matrix const& F)
      {
        d0Result = F + transpose(F);
        d0Result *= 0.5;
      }

      /// Function value \f$ \frac{1}{2}\left(F^T+F\right) \f$.
      Matrix const& d0() const
      {
        return d0Result;
      }

      /// First directional derivative \f$ \frac{1}{2}\left(dF^T+dF\right) \f$.
      template <int>
      Matrix d1(const Matrix& dF) const
      {
        return 0.5*(dF + transpose(dF));
      }

    private:
      Matrix d0Result = Matrix{0.};
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Model of the geometric nonlinearity in elasticity theory.
     * Implemented as template-alias to CauchyGreenStrainTensor.
     */
    template <class Matrix> using GeometricNonlinearity = LeftCauchyGreenStrainTensor<Matrix>;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Strain tensor \f$ \frac{1}{2}\left(F^T+F + F^T F\right) \f$.
     *
     * This class is used for nonlinear material models based on the displacement gradient \f$\nabla u\f$, which takes the role of \f$F\f$.
     * Implemented as Sum<LinearizedStrainTensor,GeometricNonlinearity>.
     */
    template <class Matrix>
    class StrainTensor :
        public MathematicalOperations::Sum< LinearizedStrainTensor<Matrix> , GeometricNonlinearity<Matrix> > ,
        public Chainer< StrainTensor<Matrix> >
    {
      using Base = MathematicalOperations::Sum< LinearizedStrainTensor<Matrix> , GeometricNonlinearity<Matrix> >;
    public:
      /**
       * @brief Constructor.
       * @param F point of evaluation
       */
      explicit StrainTensor(const Matrix& F) : Base(F) {}
    };
  }
}

#endif // FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH
