// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_COFACTOR_HH
#define FUNG_LINEAR_ALGEBRA_COFACTOR_HH

#include <cassert>
#include "dimension.hh"
#include "fung/concept_check.hh"
#include "fung/util/at.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /** @addtogroup LinearAlgebraGroup
     *  @{ */

    /// @cond
    namespace Detail
    {
      constexpr int getFirst(int row)
      {
        return (row==0) ? 1 : 0;
      }

      constexpr int getLast(int row)
      {
        return (row==2) ? 1 : 2;
      }

      template <int row, int col>
      struct CofactorIndices
      {
        static constexpr int firstRow = getFirst(row);
        static constexpr int lastRow  = getLast(row);
        static constexpr int firstCol = getFirst(col);
        static constexpr int lastCol  = getLast(col);
      };

      constexpr int sign(int row, int col)
      {
        return (((row+col)%2==0) ? 1 : -1 );
      }

      template <int row, int col, class Matrix>
      auto computeCofactorImpl(Matrix const&, Matrix const& A, std::integral_constant<int,2>)
      {
        using Id = Detail::CofactorIndices<row,col>;
        return sign(row,col) * at(A,Id::firstRow,Id::firstCol);
      }

      template <int row, int col, class Matrix>
      auto computeCofactorImpl(Matrix const& A, Matrix const& B, std::integral_constant<int,3>)
      {
        using Id = Detail::CofactorIndices<row,col>;
        return sign(row,col) * (at(A,Id::firstRow,Id::firstCol)*at(B,Id::lastRow,Id::lastCol) - at(A,Id::firstRow,Id::lastCol)*at(B,Id::lastRow,Id::firstCol));
      }
    }
    /// @endcond

    /**
     * @brief Compute the \f$(row,col)\f$-cofactor of \f$ A \f$. Implemented for \f$ A\in \mathbb{R}^{n,n} \f$ with \f$ n=2,3 \f$.
     *
     * The \f$(i,j)\f$-cofactor of a matrix \f$ A \f$ is \f$ (-1)^{i+j} \det(A^\#_{ij}) \f$, where
     * \f$ A^\#_ij \f$ is obtained from \f$ A \f$ by deleting the \f$i\f$-th row and \f$ j \f$-th column.
     */
    template < int row , int col , class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr ,
               class = Concepts::MatrixConceptCheck<Matrix> >
    auto computeCofactor(Matrix const& A)
    {
      static_assert( dim<Matrix>() == 2 || dim<Matrix>() == 3 ,
                     "Cofactors are currently only implemented for 2x2 and 3x3 matrices. Efficient general implementations are non-trivial and may or may not be implemented in the future." );
      return Detail::computeCofactorImpl<row,col>(A, A, std::integral_constant<int,dim<Matrix>()>());
    }

    /**
     * @brief Compute the \f$(row,col)\f$-cofactor of \f$ A \f$. Implemented for \f$ A\in \mathbb{R}^{n,n} \f$ with \f$ n=2,3 \f$.
     *
     * The \f$(i,j)\f$-cofactor of a matrix \f$ A \f$ is \f$ (-1)^{i+j} \det(A^\#_{ij}) \f$, where
     * \f$ A^\#_ij \f$ is obtained from \f$ A \f$ by deleting the \f$i\f$-th row and \f$ j \f$-th column.
     */
    template < int row , int col , class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr ,
               class = Concepts::MatrixConceptCheck<Matrix> >
    auto computeCofactor(Matrix const& A)
    {
      assert( ( rows(A)==2 && cols(A)==2 ) || ( rows(A)==3 && cols(A)==3) );
      if( rows(A) == 2 ) return Detail::computeCofactorImpl<row,col>(A, A, std::integral_constant<int,2>());
      /*if( rows(A) == 3 )*/ return Detail::computeCofactorImpl<row,col>(A, A, std::integral_constant<int,3>());
    }

    /**
     * @brief Compute the first directional derivative in direction \f$ B \f$ of the \f$(row,col)\f$-cofactor of \f$ A \f$. Implemented for \f$ A\in \mathbb{R}^{n,n} \f$ with \f$ n=2,3 \f$.
     *
     * The \f$(i,j)\f$-cofactor of a matrix \f$ A \f$ is \f$ (-1)^{i+j} \det(A^\#_{ij}) \f$, where
     * \f$ A^\#_{ij} \f$ is obtained from \f$ A \f$ by deleting the \f$i\f$-th row and \f$ j \f$-th column.
     * If \f$ A\in \mathbb{R}^{3,3} \f$, then the cofactors are quadratic polynomials of the entries of \f$ A^\#_{ij} \f$.
     * In this case this function can also used to compute the second directional derivative in directions \f$ A \f$ and \f$ B \f$.
     */
    template < int row , int col , class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr ,
               class = Concepts::MatrixConceptCheck<Matrix> >
    auto computeCofactorDirectionalDerivative(Matrix const& A, Matrix const& B)
    {
      static_assert( dim<Matrix>() == 2 || dim<Matrix>() == 3 ,
                     "Cofactors are currently only implemented for 2x2 and 3x3 matrices. Efficient general implementations are non-trivial and may or may not be implemented in the future." );
      return Detail::computeCofactorImpl<row,col>(A, B, std::integral_constant<int,dim<Matrix>()>());
    }

    /**
     * @brief Compute the first directional derivative in direction \f$ B \f$ of the \f$(row,col)\f$-cofactor of \f$ A \f$. Implemented for \f$ A\in \mathbb{R}^{n,n} \f$ with \f$ n=2,3 \f$.
     *
     * The \f$(i,j)\f$-cofactor of a matrix \f$ A \f$ is \f$ (-1)^{i+j} \det(A^\#_{ij}) \f$, where
     * \f$ A^\#_{ij} \f$ is obtained from \f$ A \f$ by deleting the \f$i\f$-th row and \f$ j \f$-th column.
     * If \f$ A\in \mathbb{R}^{3,3} \f$, then the cofactors are quadratic polynomials of the entries of \f$ A^\#_{ij} \f$.
     * In this case this function can also used to compute the second directional derivative in directions \f$ A \f$ and \f$ B \f$.
     */
    template < int row , int col , class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr ,
               class = Concepts::MatrixConceptCheck<Matrix> >
    auto computeCofactorDirectionalDerivative(Matrix const& A, Matrix const& B)
    {
      assert( ( rows(A)==2 && cols(A)==2 ) || ( rows(A)==3 && cols(A)==3) );
      if ( rows(A)==2 ) return Detail::computeCofactorImpl<row,col>(A, B, std::integral_constant<int,2>());
      /*if ( rows(A)==3 )*/ return Detail::computeCofactorImpl<row,col>(A, B, std::integral_constant<int,3>());
    }

    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_COFACTOR_HH
