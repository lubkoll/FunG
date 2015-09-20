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

#ifndef FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH
#define FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH

#include <type_traits>

#include "fung/cmath/pow.hh"
#include "rows_and_cols.hh"
#include "fung/util/at.hh"
#include "fung/util/chainer.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts { template <class> struct MatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \internal DOCUMENT_IMPLEMENTATION_DETAILS
     */
    namespace Detail
    {
      template <class Matrix,
                std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
      auto computeScalarProduct(const Matrix& A, const Matrix& B) const
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
      auto computeScalarProduct(const Matrix& A, const Matrix& B) const
      {
        using Index = decltype(rows(A));
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < rows(A) ; ++i )
          for( Index j = 0 ; j < cols(A) ; ++j )
            result += at(A,i,j) * at(B,i,j);
        return result;
      }
    }
    /**
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute squared Frobenius norm \f$ \|A\|^2 = A\negthinspace : \negthinspace A = \mathrm{tr}(A^TA) = \sum_{i,j} A_{ij}^2. \f$
     */
    template <class Matrix,
              class = Concepts::MatrixConceptCheck<Matrix> >
    struct SquaredFrobeniusNorm : Base , Chainer< SquaredFrobeniusNorm<Matrix> >
    {
      /// Default constructor.
      SquaredFrobeniusNorm() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute squared norm from.
       */
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
        value = Detail::computeSquareNorm(A_,A_);
      }

      /// Squared matrix norm. Convenient access to d0().
      auto operator()() const noexcept
      {
        return d0();
      }

      /// Squared matrix norm.
      auto d0() const noexcept
      {
        return value;
      }

      /// First directional derivative.
      template <int>
      auto d1(const Matrix& dA) const
      {
        return 2 * Detail::computeScalarProduct(A_,dA);
      }

      /// Second directional derivative.
      template <int,int>
      auto d2(const Matrix& dA1, const Matrix& dA2) const
      {
        return 2 * Detail::computeScalarProduct(dA1,dA2);
      }

    private:
      Matrix A_;
      std::decay_t<decltype(at(std::declval<Matrix>(),0,0))> value;
      bool initialized = false;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
     */
    template <class Matrix>
    using FrobeniusNorm = MathematicalOperations::Chain< Sqrt , SquaredFrobeniusNorm<Matrix> >;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Generate Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
     */
    template <class Matrix,
              std::enable_if_t<!std::is_base_of<Base,Matrix>::value>* = nullptr>
    auto frobeniusNorm(const Matrix& A)
    {
      return FrobeniusNorm<Matrix>(A);
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Generate Frobenius norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
     */
    template <class F,
              std::enable_if_t<std::is_base_of<Base,F>::value>* = nullptr>
    auto frobeniusNorm(const F& f)
    {
      return FrobeniusNorm< std::decay_t<decltype(f.d0())> >(f.d0())(f);
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_FROBENIUS_NORM_HH
