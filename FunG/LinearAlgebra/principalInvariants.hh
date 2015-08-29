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

#ifndef FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH
#define FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH

#include <type_traits>

#include "dimension.hh"
#include "trace.hh"
#include "cofactor.hh"
#include "determinant.hh"
#include "shiftedInvariant.hh"
#include "FunG/Util/base.hh"

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
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
    namespace Detail
    {
      template <class Matrix>
      auto computeCofactors(const Matrix& A, std::integral_constant<int,2>)
      {
        return computeCofactor<0,0>(A) + computeCofactor<1,1>(A);
      }

      template <class Matrix>
      auto computeCofactors(const Matrix& A, std::integral_constant<int,3>)
      {
        return computeCofactor<0,0>(A) + computeCofactor<1,1>(A) + computeCofactor<2,2>(A);
      }

      template < class Matrix >
      auto computeCofactors(const Matrix& A, std::integral_constant<int,-1>)
      {
        if(rows(A) == 2) return computeCofactors(A,std::integral_constant<int,2>());
        /*if(rows(A) == 3)*/ return computeCofactors(A,std::integral_constant<int,3>());
      }

      template <int row, class Matrix>
      auto symmetricCofactorDerivative(const Matrix& A, const Matrix& dA)
      {
        return computeCofactorDirectionalDerivative<row,row>(A,dA) + computeCofactorDirectionalDerivative<row,row>(dA,A);
      }


      template <class Matrix>
      auto computeSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B, std::integral_constant<int,2>)
      {
        return symmetricCofactorDerivative<0>(A,B) + symmetricCofactorDerivative<1>(A,B);
      }

      template <class Matrix>
      auto computeSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B, std::integral_constant<int,3>)
      {
        return symmetricCofactorDerivative<0>(A,B) + symmetricCofactorDerivative<1>(A,B) + symmetricCofactorDerivative<2>(A,B);
      }


      template < class Matrix >
      auto computeSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B, std::integral_constant<int,-1>)
      {
        if(rows(A) == 2) return computeSymmetricCofactorDerivatives(A,B,std::integral_constant<int,2>());
        /*if(rows(A) == 3)*/ return computeSymmetricCofactorDerivatives(A,B,std::integral_constant<int,3>());

      }
    }
    /**
     * \endcond
     */


    /**
     * \ingroup InvariantGroup
     * \brief First principal invariant \f$ \iota_1(A)=\mathrm{tr}(A) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using FirstPrincipalInvariant = Trace<Matrix>;

    /**
     * \ingroup InvariantGroup
     * \brief Second principal invariant \f$ \iota_2(A)=\mathrm{tr}(\mathrm{cof}(A)) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    class SecondPrincipalInvariant : Base
    {
      using DimIdentifier = std::integral_constant<int,dimension<Matrix>()>;
    public:
      /// Default constructor.
      SecondPrincipalInvariant() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute second principal invariant from
       */
      SecondPrincipalInvariant(const Matrix& A) { update(A); }

      /// Reset matrix to compute second principal invariant from.
      void update(const Matrix& A)
      {
        A_ = A;
        resultOfD0 = Detail::computeCofactors(A,dimIdentifier);
      }

      /// Value of the second principal invariant
      auto d0() const
      {
        return resultOfD0;
      }

      /**
       * @brief First directional derivative
       * @param dA1 direction for which the derivative is computed
       */
      template <int,int>
      auto d1(const Matrix& dA1) const
      {
        return Detail::computeSymmetricCofactorDerivatives(A_,dA1,dimIdentifier);
      }

      /**
       * @brief Second directional derivative
       * @param dA1 direction for which the derivative is computed
       * @param dA2 direction for which the derivative is computed
       */
      template <int,int>
      auto d2(const Matrix& dA1, const Matrix& dA2) const
      {
        return Detail::computeSymmetricCofactorDerivatives(dA1,dA2,dimIdentifier);
      }

    private:
      Matrix A_;
      std::decay_t< at_t<Matrix> > resultOfD0 = 0.;
      DimIdentifier dimIdentifier;
    };

    /**
     * \ingroup InvariantGroup
     * \brief Third principal invariant \f$ \iota_3(A)=\det(A) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using ThirdPrincipalInvariant = Determinant<Matrix>;


    /**
     * \ingroup InvariantGroup
     * \brief Shifted first principal invariant \f$ \iota_1(A) - n \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template < class Matrix , int offset = dimension<Matrix>() >
    using ShiftedFirstPrincipalInvariant  = ShiftedInvariant< FirstPrincipalInvariant<Matrix> , offset >;

    /**
     * \ingroup InvariantGroup
     * \brief Shifted second principal invariant \f$ \iota_2(A) - n \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template < class Matrix , int offset = dimension<Matrix>() >
    using ShiftedSecondPrincipalInvariant = ShiftedInvariant< SecondPrincipalInvariant<Matrix> , offset >;

    /**
     * \ingroup InvariantGroup
     * \brief Shifted third principal invariant \f$ \iota_3(A) - 1 \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template < class Matrix >
    using ShiftedThirdPrincipalInvariant  = ShiftedInvariant< ThirdPrincipalInvariant<Matrix>   , 1 >;
  }
}

#endif // FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH
