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

#ifndef FUNG_LINEAR_ALGEBRA_DEVIATOR_HH
#define FUNG_LINEAR_ALGEBRA_DEVIATOR_HH

#include "../generate.hh"
#include "../identity.hh"
#include "trace.hh"
#include "unitMatrix.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts { template <class> struct SymmetricMatrixConceptChecks; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief %Deviator of a matrix \f$ A\in\mathbb{R}^{n,n} \f$, i.e. \f$ A - \frac{\mathrm{tr}(A)}{n}I \f$.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix>>
    auto deviator(const Matrix& A)
    {
      return Identity<Matrix>(A) + (-1./dimension<Matrix>()) * ( trace(A) * Constant<Matrix>( unitMatrix<Matrix>() ) );
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Type of the %deviator of a matrix \f$ A\in\mathbb{R}^{n,n} \f$, i.e. \f$ A - \frac{\mathrm{tr}(A)}{n}I \f$.
     */
    template <class Matrix>
    class Deviator : public decltype( deviator( std::declval<Matrix>() ) )
    {
    private:
      using Base = decltype( deviator( std::declval<Matrix>() ) );
    public:
      Deviator() = default;
      Deviator(const Matrix& A) : Base( deviator(A) )
      {}
    };
  }
}

#endif // FUNG_LINEAR_ALGEBRA_DEVIATOR_HH
