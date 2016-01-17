// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH
#define FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH

#include "transpose.hh"
#include "fung/mathematical_operations/sum.hh"
#include "fung/util/chainer.hh"
#include "fung/util/add_transposed_matrix.hh"

namespace FunG
{
  /// @cond
  namespace Concepts { template <class> struct SquareMatrixConceptCheck; }
  ///@endcond

  namespace LinearAlgebra
  {
    /**
     * @ingroup LinearAlgebraGroup
     * @brief Left Cauchy-Green strain tensor \f$ F^T F \f$ for a symmetric matrix \f$ F \f$.
     *
     * This class is used for nonlinear material models based on the deformation gradient \f$\nabla\varphi\f$, which takes the role of \f$F\f$.
     * Caches both \f$ F^T \f$ and \f$ F^T F \f$.
     */
    template <class Matrix, class = Concepts::SquareMatrixConceptCheck<Matrix> >
    class LeftCauchyGreenStrainTensor :
        public Chainer< LeftCauchyGreenStrainTensor<Matrix , Concepts::SquareMatrixConceptCheck<Matrix> > >
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
        if( !initialized )
        {
          new(&FT) Matrix{transpose(F)};
          new(&FTF) Matrix{FT*F};
          initialized = true;
        }
        else
        {
          FT = transpose(F);
          FTF = FT * F;
        }
      }

      /// Function value \f$ F^T * F \f$.
      Matrix const& d0() const noexcept
      {
        return FTF;
      }

      /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
      Matrix d1(Matrix const& dF1) const
      {
        Matrix FTdF1 = FT * dF1;
        return addTransposed(FTdF1);
      }

      /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
      Matrix d2(Matrix const& dF1, Matrix const& dF2) const
      {
        Matrix dF2TdF1 = transpose(dF2) * dF1;
        return addTransposed(dF2TdF1);
      }

    private:
      Matrix FT, FTF;
      bool initialized = false;
    };

    template <class Matrix>
    auto strainTensor(const Matrix& F)
    {
      return LeftCauchyGreenStrainTensor<Matrix>{F};
    }

//    /**
//     * \ingroup LinearAlgebraGroup
//     * \brief Linearized strain tensor \f$ \frac{1}{2}\left(F^T+F\right) \f$.
//     *
//     * This class is used for linear material models based on the displacement gradient \f$\nabla u\f$, which takes the role of \f$F\f$.
//     * Caches the function value \f$ \frac{1}{2}\left(F^T+F\right) \f$.
//     */
//    template <class Matrix, class = Concepts::SquareMatrixConceptCheck<Matrix> >
//    class LinearizedStrainTensor :
//        public Base ,
//        public Chainer< LinearizedStrainTensor< Matrix , Concepts::SquareMatrixConceptCheck<Matrix> > >
//    {
//    public:
//      /**
//       * @brief Constructor.
//       * @param F point of evaluation
//       */
//      explicit LinearizedStrainTensor(const Matrix& F) { update(F); }

//      /// Reset point of evaluation.
//      void update(Matrix const& F)
//      {
//        d0Result = F + transpose(F);
//        d0Result *= 0.5;
//      }

//      /// Function value \f$ \frac{1}{2}\left(F^T+F\right) \f$.
//      Matrix const& d0() const
//      {
//        return d0Result;
//      }

//      /// First directional derivative \f$ \frac{1}{2}\left(dF^T+dF\right) \f$.
//      template <int>
//      Matrix d1(const Matrix& dF) const
//      {
//        return 0.5*(dF + transpose(dF));
//      }

//    private:
//      Matrix d0Result = Matrix{0.};
//    };

//    /**
//     * \ingroup LinearAlgebraGroup
//     * \brief Model of the geometric nonlinearity in elasticity theory.
//     * Implemented as template-alias to CauchyGreenStrainTensor.
//     */
//    template <class Matrix> using GeometricNonlinearity = LeftCauchyGreenStrainTensor<Matrix>;

//    /**
//     * \ingroup LinearAlgebraGroup
//     * \brief Strain tensor \f$ \frac{1}{2}\left(F^T+F + F^T F\right) \f$.
//     *
//     * This class is used for nonlinear material models based on the displacement gradient \f$\nabla u\f$, which takes the role of \f$F\f$.
//     * Implemented as Sum<LinearizedStrainTensor,GeometricNonlinearity>.
//     */
//    template <class Matrix>
//    class StrainTensor :
//        public MathematicalOperations::Sum< LinearizedStrainTensor<Matrix> , GeometricNonlinearity<Matrix> > ,
//        public Chainer< StrainTensor<Matrix> >
//    {
//      using Base = MathematicalOperations::Sum< LinearizedStrainTensor<Matrix> , GeometricNonlinearity<Matrix> >;
//    public:
//      /**
//       * @brief Constructor.
//       * @param F point of evaluation
//       */
//      explicit StrainTensor(const Matrix& F) : Base(F) {}
//    };
  }
}

#endif // FUNG_LINEAR_ALGEBRA_STRAIN_TENSOR_HH
