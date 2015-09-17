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

#include "fung/generate.hh"
#include "fung/identity.hh"
#include "trace.hh"
#include "unit_matrix.hh"

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
     * \brief Generate %deviator \f$ \mathrm{dev}(A) = A - \frac{\mathrm{tr}(A)}{n}I \f$ of a matrix \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template <class Matrix,
              int n = dim<Matrix>(),
              std::enable_if_t<Checks::isConstantSize<Matrix>() && !Checks::is_base_of<Base,Matrix>::value>* = nullptr,
              class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    auto deviator(const Matrix& A)
    {
      return identity(A) + (-1./n) * ( trace(A) * constant( unitMatrix<Matrix>() ) );
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Generate %deviator \f$ \mathrm{dev}(A) = A - \frac{\mathrm{tr}(A)}{n}I \f$ of a matrix \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template <class Matrix,
              std::enable_if_t<!Checks::isConstantSize<Matrix>() && !Checks::is_base_of<Base,Matrix>::value>* = nullptr,
              class = Concepts::SymmetricMatrixConceptCheck<Matrix>>
    auto deviator(const Matrix& A)
    {
      assert(rows(A)==cols(A));
      return identity(A) + (-1./rows(A)) * ( trace(A) * constant( unitMatrix<Matrix>() ) );
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Generate %deviator \f$ \mathrm{dev}\circ f\f$.
     */
    template <class F,
              std::enable_if_t<Checks::is_base_of<Base,F>::value>* = nullptr>
    auto deviator(const F& f)
    {
      return deviator(f())( f );
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_DEVIATOR_HH
