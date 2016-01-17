// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH
#define FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH

#include <type_traits>

#include "fung/cmath/pow.hh"
#include "fung/util/at.hh"
#include "fung/util/chainer.hh"
#include "fung/util/static_checks.hh"
#include "rows_and_cols.hh"

namespace FunG
{
  /// @cond
  namespace Concepts { template <class> struct MatrixConceptCheck; }
  /// @endcond

  namespace LinearAlgebra
  {
    /** @addtogroup LinearAlgebraGroup
     *   @{ */

    /// @cond
    namespace FrobeniusDetail
    {
      template <class Matrix,
                std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
      inline auto computeScalarProduct(const Matrix& A, const Matrix& B)
      {
        using Index = decltype(rows<Matrix>());
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < rows<Matrix>() ; ++i )
          for( Index j = 0 ; j < cols<Matrix>() ; ++j )
            result += at(A,i,j) * at(B,i,j);
        return result;
      }

      template <class Matrix,
                std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
      inline auto computeScalarProduct(const Matrix& A, const Matrix& B)
      {
        using Index = decltype(rows(A));
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < rows(A) ; ++i )
          for( Index j = 0 ; j < cols(A) ; ++j )
            result += at(A,i,j) * at(B,i,j);
        return result;
      }
    }
    /// @endcond

    /// Compute squared Frobenius norm \f$ \|A\|^2 = A\negthinspace : \negthinspace A = \mathrm{tr}(A^TA) = \sum_{i,j} A_{ij}^2. \f$
    template <class Matrix,
              class = Concepts::MatrixConceptCheck<Matrix> >
    struct SquaredFrobeniusNorm : Chainer< SquaredFrobeniusNorm<Matrix> >
    {
      SquaredFrobeniusNorm() = default;

      explicit SquaredFrobeniusNorm(const Matrix& A) { update(A); }

      /// Reset matrix to compute squared norm from.
      void update(const Matrix& A)
      {
        if( !initialized )
        {
          new(A_) Matrix(A);
          initialized = true;
        }
        else A_ = A;
        value = FrobeniusDetail::computeScalarProduct(A_,A_);
      }

      /// Squared matrix norm.
      auto d0() const noexcept
      {
        return value;
      }

      /// First directional derivative.
      auto d1(const Matrix& dA) const
      {
        return 2 * FrobeniusDetail::computeScalarProduct(A_,dA);
      }

      /// Second directional derivative.
      auto d2(const Matrix& dA1, const Matrix& dA2) const
      {
        return 2 * FrobeniusDetail::computeScalarProduct(dA1,dA2);
      }

    private:
      Matrix A_;
      std::decay_t<decltype(at(std::declval<Matrix>(),0,0))> value;
      bool initialized = false;
    };


    /// Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
    template <class Matrix>
    using FrobeniusNorm = MathematicalOperations::Chain< Sqrt , SquaredFrobeniusNorm<Matrix> >;

    /// Generate Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
    template <class Matrix,
              std::enable_if_t<!Checks::isFunction<Matrix>()>* = nullptr>
    auto frobeniusNorm(const Matrix& A)
    {
      return FrobeniusNorm<Matrix>(A);
    }

    /// Generate Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
    template <class F,
              std::enable_if_t<Checks::isFunction<F>()>* = nullptr>
    auto frobeniusNorm(const F& f)
    {
      return FrobeniusNorm< std::decay_t<decltype(f.d0())> >(f.d0())(f);
    }
    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH
