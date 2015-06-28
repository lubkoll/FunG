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

#ifndef RFFGEN_LINEAR_ALGEBRA_MATRIX_NORM_HH
#define RFFGEN_LINEAR_ALGEBRA_MATRIX_NORM_HH

#include <type_traits>

#include "../LinearAlgebra/extractRowsAndCols.hh"
#include "../MathematicalOperations/chain.hh"
#include "../CMath/pow.hh"

namespace RFFGen
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
     * \ingroup LinearAlgebraGroup
     * \brief Compute squared matrix norm \f$ \|A\|^2 = A\negthinspace : \negthinspace A = \mathrm{tr}(A^TA) = \sum_{i,j} A_{ij}^2. \f$
     */
    template <class Matrix, class = Concepts::MatrixConceptCheck<Matrix> >
    struct SquaredMatrixNorm : Base
    {
      /// Default constructor.
      SquaredMatrixNorm() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute squared norm from.
       */
      explicit SquaredMatrixNorm(const Matrix& A) { update(A); }

      /// Reset matrix to compute squared norm from.
      void update(const Matrix& A)
      {
        A_ = A;
        resultOfD0 = computeSquareNorm(A_);
      }

      /// Squared matrix norm. Convenient access to d0().
      auto operator()() const noexcept
      {
        return d0();
      }

      /// Squared matrix norm.
      auto d0() const noexcept
      {
        return resultOfD0;
      }

      /// First directional derivative.
      template <int>
      auto d1(const Matrix& dA) const
      {
        return 2 * computeScalarProduct(A_,dA);
      }

      /// Second directional derivative.
      template <int,int>
      auto d2(const Matrix& dA1, const Matrix& dA2) const
      {
        return 2 * computeScalarProduct(dA1,dA2);
      }


    private:
      auto computeSquareNorm(const Matrix& A) const
      {
        using Index = decltype(numberOfRows<Matrix>());
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < numberOfRows<Matrix>() ; ++i )
          for( Index j = 0 ; j < numberOfColumns<Matrix>() ; ++j )
            result += at(A,i,j) * at(A,i,j);
        return result;
      }

      auto computeScalarProduct(const Matrix& A, const Matrix& B) const
      {
        using Index = decltype(numberOfRows<Matrix>());
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < numberOfRows<Matrix>() ; ++i )
          for( Index j = 0 ; j < numberOfColumns<Matrix>() ; ++j )
            result += at(A,i,j) * at(B,i,j);
        return result;
      }

      Matrix A_;
      std::remove_const_t< std::remove_reference_t< at_t<Matrix> > > resultOfD0;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute matrix norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
     */
    template <class Matrix>
    using MatrixNorm = MathematicalOperations::Chain< CMath::Sqrt , SquaredMatrixNorm<Matrix> >;
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_MATRIX_NORM_HH
