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

#ifndef FUNG_UTIL_AT_HH
#define FUNG_UTIL_AT_HH

#include <type_traits>
#include <utility>
#include "static_checks.hh"


namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace AtDetail
  {
    template <class Matrix, class = void>
    struct At
    {
      static auto& apply(Matrix& A, int i, int j)
      {
        return A(i,j);
      }

      static const auto& apply(const Matrix& A, int i, int j)
      {
        return A(i,j);
      }
    };

    template <class Matrix>
    struct At< Matrix , void_t< Checks::TryAccessViaSquareBracketsForMatrix<Matrix> > >
    {
      static auto& apply(Matrix& A, int i, int j)
      {
        return A[i][j];
      }

      static const auto& apply(const Matrix& A, int i, int j)
      {
        return A[i][j];
      }
    };

    template <class Vector, class = void>
    struct At_v
    {
      static auto& apply(Vector& v, int i)
      {
        return v(i);
      }

      static const auto& apply(const Vector& v, int i)
      {
        return v(i);
      }
    };

    template <class Vector>
    struct At_v< Vector , void_t< Checks::TryAccessViaSquareBracketsForVector<Vector> > >
    {
      static auto& apply(Vector& v, int i)
      {
        return v[i];
      }

      static const auto& apply(const Vector& v, int i)
      {
        return v[i];
      }
    };
  }

  // suppress warnings coming from the __attribute__((always_inline))
  #if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wattributes"
  #endif

  template <class Matrix>
  __attribute__((always_inline)) auto& at(Matrix& A, int i, int j)
  {
    return AtDetail::At<Matrix>::apply(A,i,j);
  }

  template <class Matrix>
  __attribute__((always_inline)) const auto& at(const Matrix& A, int i, int j)
  {
    return AtDetail::At<Matrix>::apply(A,i,j);
  }

  template <class Vector>
  __attribute__((always_inline)) auto& at(Vector& v, int i)
  {
    return AtDetail::At_v<Vector>::apply(v,i);
  }

  template <class Vector>
  __attribute__((always_inline)) const auto& at(const Vector& v, int i)
  {
    return AtDetail::At_v<Vector>::apply(v,i);
  }
  /**
   * \endcond
   */
}

#if defined(__GNUC__) || defined(__GNUG__)
  #pragma GCC diagnostic pop
#endif

#endif // FUNG_UTIL_AT_HH
