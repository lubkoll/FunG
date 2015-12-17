// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_AT_HH
#define FUNG_UTIL_AT_HH

#include <type_traits>
#include <utility>
#include "static_checks.hh"


namespace FunG
{
  /// @cond
  namespace AtDetail
  {
    template <class Matrix, class = void>
    struct At
    {
      static decltype(auto) apply(Matrix& A, int i, int j)
      {
        return A(i,j);
      }

      static decltype(auto) apply(const Matrix& A, int i, int j)
      {
        return A(i,j);
      }
    };

    template <class Matrix>
    struct At< Matrix , void_t< Checks::TryMemFn_SquareBracketAccessForMatrix<Matrix> > >
    {
      static decltype(auto) apply(Matrix& A, int i, int j)
      {
        return A[i][j];
      }

      static decltype(auto) apply(const Matrix& A, int i, int j)
      {
        return A[i][j];
      }
    };

    template <class Vector, class = void>
    struct At_v
    {
      static decltype(auto) apply(Vector& v, int i)
      {
        return v(i);
      }

      static decltype(auto) apply(const Vector& v, int i)
      {
        return v(i);
      }
    };

    template <class Vector>
    struct At_v< Vector , void_t< Checks::TryMemFn_SquareBracketAccessForVector<Vector> > >
    {
      static decltype(auto) apply(Vector& v, int i)
      {
        return v[i];
      }

      static decltype(auto) apply(const Vector& v, int i)
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
  __attribute__((always_inline)) decltype(auto) at(Matrix& A, int i, int j)
  {
    return AtDetail::At<Matrix>::apply(A,i,j);
  }

  template <class Matrix>
  __attribute__((always_inline)) decltype(auto) at(const Matrix& A, int i, int j)
  {
    return AtDetail::At<Matrix>::apply(A,i,j);
  }

  template <class Vector>
  __attribute__((always_inline)) decltype(auto) at(Vector& v, int i)
  {
    return AtDetail::At_v<Vector>::apply(v,i);
  }

  template <class Vector>
  __attribute__((always_inline)) decltype(auto) at(const Vector& v, int i)
  {
    return AtDetail::At_v<Vector>::apply(v,i);
  }
  /// @endcond
}

#if defined(__GNUC__) || defined(__GNUG__)
  #pragma GCC diagnostic pop
#endif

#endif // FUNG_UTIL_AT_HH
