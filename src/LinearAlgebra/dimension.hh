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

#ifndef RFFGEN_LINEAR_ALGEBRA_DIMENSION_HH
#define RFFGEN_LINEAR_ALGEBRA_DIMENSION_HH

#include <type_traits>
#include "extractRowsAndCols.hh"

namespace RFFGen
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
    /// Specialize this for your matrix class. Dimension (number of rows/columns for square matrices) must be provided by a static member variable called value.
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct ExtractDimension : ExtractNumberOfRows<Matrix> {};

    /// Dimension \f$n\f$ of a fixed size matrix in \f$\mathbb{R}^{n,n}\f$.
    template <class Matrix>
    constexpr int dimension()
    {
      return ExtractDimension<Matrix>::value;
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_DIMENSION_HH
