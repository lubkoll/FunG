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

#ifndef FUNG_UTIL_ZERO_HH
#define FUNG_UTIL_ZERO_HH

#include <utility>

#include "staticChecks.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Checks
  {
    template <class Matrix>
    using TryCallToZeroes = decltype(std::declval<Matrix>().zeros());

    template <class Matrix>
    using TryCallToFill   = decltype(std::declval<Matrix>().fill(0));
  }
  /**
   * \endcond
   */

  /// Specialize this struct for your matrix type if a zero matrix cannot be generated via Matrix(0.).
  template <class Matrix, class = void>
  struct Zero
  {
    /**
     * @return zero matrix
     */
    Matrix operator()() const
    {
      return Matrix(0.);
    }
  };

  /// Specialization for the case that a matrix can be set to zero by calling the member function fill(0).
  template <class Matrix>
  struct Zero< Matrix , void_t<Checks::TryCallToFill<Matrix> > >
  {
    /**
     * @return zero matrix
     */
    Matrix operator()() const
    {
      Matrix m;
      m.fill(0);
      return m;
    }

    /**
     * @brief Set all entries of m to 0.
     */
    Matrix& operator()(Matrix& m) const
    {
      m.fill(0);
      return m;
    }
  };

  /**
   * Requires that a specialization of struct Zero exists for Matrix.
   * @return constant size zero matrix
   */
  template <class Matrix, class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
  Matrix zero()
  {
    return Zero<Matrix>()();
  }

  /**
   * Requires that a specialization of struct Zero exists for Matrix.
   * @return dynamic size zero matrix
   */
  template <class Matrix, class = std::enable_if_t<!Checks::isConstantSizeMatrix<Matrix>()> >
  constexpr Matrix zero(int rows, int cols)
  {
    Matrix m(rows,cols);
    return Zero<Matrix>()(m);
  }

}

#endif // FUNG_UTIL_ZERO_HH
