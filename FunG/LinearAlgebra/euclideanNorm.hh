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

#ifndef FUNG_LINEAR_ALGEBRA_EUCLIDEAN_NORM_HH
#define FUNG_LINEAR_ALGEBRA_EUCLIDEAN_NORM_HH

#include <type_traits>

#include "../LinearAlgebra/extractRowsAndCols.hh"
#include "../MathematicalOperations/chain.hh"
#include "../CMath/pow.hh"
#include "../Util/staticChecks.hh"

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
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
    namespace Detail
    {
      template < class Matrix , std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
      inline auto computeSquareEuclideanNorm(const Matrix& A)
      {
        using Index = decltype(numberOfRows<Matrix>());
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < numberOfRows<Matrix>() ; ++i )
          for( Index j = 0 ; j < numberOfColumns<Matrix>() ; ++j )
            result += at(A,i,j) * at(A,i,j);
        return result;
      }

      template < class Vector , std::enable_if_t<Checks::isStaticVector<Vector>() && !Checks::isConstantSizeMatrix<Vector>()> >
      inline auto computeSquareEuclideanNorm(const Vector& A)
      {
        using Index = decltype(numberOfRows<Vector>());
        auto result = decltype(at(A,0)){0.};
        for( Index i = 0; i < numberOfRows<Vector>() ; ++i )
            result += at(A,i) * at(A,i);
        return result;
      }

      template < class Matrix , std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
      inline auto computeEuclideanScalarProduct(const Matrix& A, const Matrix& B)
      {
        using Index = decltype(numberOfRows<Matrix>());
        auto result = decltype(at(A,0,0)){0.};
        for( Index i = 0; i < numberOfRows<Matrix>() ; ++i )
          for( Index j = 0 ; j < numberOfColumns<Matrix>() ; ++j )
            result += at(A,i,j) * at(B,i,j);
        return result;
      }

      template < class Vector , std::enable_if_t<Checks::isStaticVector<Vector>() && !Checks::isConstantSizeMatrix<Vector>()> >
      inline auto computeEuclideanScalarProduct(const Vector& v, const Vector& w)
      {
        using Index = decltype(numberOfRows<Vector>());
        auto result = at(v,0) * at(w,1);
        for( Index i = 1; i < numberOfRows<Vector>() ; ++i )
            result += at(v,i) * at(w,i);
        return result;
      }
    }
    /**
     * \endcond
     */


    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute squared matrix norm \f$ \|A\|^2 = A\negthinspace : \negthinspace A = \mathrm{tr}(A^TA) = \sum_{i,j} A_{ij}^2. \f$
     */
    template <class Matrix, class = Concepts::MatrixConceptCheck<Matrix> >
    struct SquaredEuclideanNorm : Base
    {
      /// Default constructor.
      SquaredEuclideanNorm() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute squared norm from.
       */
      explicit SquaredEuclideanNorm(const Matrix& A) { update(A); }

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
      Matrix A_;
      std::remove_const_t< std::remove_reference_t< at_t<Matrix> > > resultOfD0;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Compute matrix norm \f$ \|A\| = \sqrt{A\negthinspace : \negthinspace A }= \sqrt{\mathrm{tr}(A^TA)} = \sqrt{\sum_{i,j} A_{ij}^2}. \f$
     */
    template <class Matrix>
    using EuclideanNorm = MathematicalOperations::Chain< CMath::Sqrt , SquaredEuclideanNorm<Matrix> >;
  }
}

#endif // FUNG_LINEAR_ALGEBRA_EUCLIDEAN_NORM_HH
