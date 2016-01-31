// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_AT_HH
#define FUNG_UTIL_AT_HH

#include <type_traits>
#include <utility>
#include "static_checks.hh"


namespace FunG
{
  /// Traits class for accessing the entries of a matrix. Default: A(i,j).
  template <class Matrix, class = void>
  struct AccessEntryOfMatrix
  {
    template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
    static decltype(auto) apply(Matrix& A, Index i, Index j)
    {
      return A(i,j);
    }
  };

  /// Specialization for the case that matrix entries can be accessed via square brackets: A[i][j].
  template <class Matrix>
  struct AccessEntryOfMatrix< Matrix , void_t< Checks::TryMemFn_SquareBracketAccessForMatrix< std::decay_t<Matrix> > > >
  {
    template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
    static decltype(auto) apply(Matrix& A, Index i, Index j)
    {
      return A[i][j];
    }
  };

  /// Traits class for accessing the entries of a vector. Default: v(i).
  template <class Vector, class = void>
  struct AccessEntryOfVector
  {
    template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
    static decltype(auto) apply(Vector& v, Index i)
    {
      return v(i);
    }
  };

  /// Specialization for the case that vector entries can be accessed via square brackets: v[i].
  template <class Vector>
  struct AccessEntryOfVector< Vector , void_t< Checks::TryMemFn_SquareBracketAccessForVector< std::decay_t<Vector> > > >
  {
    template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
    static decltype(auto) apply(Vector& v, Index i)
    {
      return v[i];
    }
  };

  // suppress warnings coming from the __attribute__((always_inline))
  #if defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wattributes"
  #endif


  /// Access matrix entry \f$A_{ij}\f$.
  template <class Matrix, class Index,
            class = std::enable_if_t< std::is_integral<Index>::value > >
  __attribute__((always_inline)) decltype(auto) at( Matrix&& A, Index i, Index j )
  {
    return AccessEntryOfMatrix<Matrix>::apply( A, i, j );
  }

  /// Access vector entry \f$\v_if$.
  template <class Vector, class Index,
            class = std::enable_if_t< std::is_integral<Index>::value > >
  __attribute__((always_inline)) decltype(auto) at( Vector&& v, Index i )
  {
    return AccessEntryOfVector<Vector>::apply( v, i );
  }
}

#if defined(__GNUC__) || defined(__GNUG__)
  #pragma GCC diagnostic pop
#endif

#endif // FUNG_UTIL_AT_HH
