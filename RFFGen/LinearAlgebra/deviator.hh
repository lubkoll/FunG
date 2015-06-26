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

#ifndef RFFGEN_LINEAR_ALGEBRA_DEVIATOR_HH
#define RFFGEN_LINEAR_ALGEBRA_DEVIATOR_HH

#include <tuple>

#include "../constant.hh"
#include "../identity.hh"
#include "trace.hh"
#include "unitMatrix.hh"

#include "../MathematicalOperations/product.hh"
#include "../MathematicalOperations/sum.hh"

namespace RFFGen
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
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct Deviator : MathematicalOperations::Sum< Identity<Matrix> , MathematicalOperations::Product< Trace<Matrix> , Constant<Matrix> > >
    {
      Deviator() = default;
      /**
       * @brief Constructor.
       * @param A matrix for which the deviator is computed
       */
      explicit Deviator(const Matrix& A)
        : MathematicalOperations::Sum< Identity<Matrix> , MathematicalOperations::Product< Trace<Matrix> , Constant<Matrix> > >
          ( A,
            std::make_tuple( A , - 1./dimension<Matrix>() * unitMatrix<Matrix>() ) )
      {}
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Create object Deviator<Matrix>(A).
     */
    template <class Matrix>
    auto deviator(const Matrix& A)
    {
      return Deviator<Matrix>(A);
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_DEVIATOR_HH
