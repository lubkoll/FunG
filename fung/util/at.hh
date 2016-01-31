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
      template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
      static decltype(auto) apply(Matrix&& A, Index i, Index j)
      {
        return A(i,j);
      }
    };

    template <class Matrix>
    struct At< Matrix , void_t< Checks::TryMemFn_SquareBracketAccessForMatrix< std::decay_t<Matrix> > > >
    {
      template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
      static decltype(auto) apply(Matrix&& A, Index i, Index j)
      {
        return A[i][j];
      }
    };

    template <class Vector, class = void>
    struct At_v
    {
      template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
      static decltype(auto) apply(Vector&& v, Index i)
      {
        return v(i);
      }
    };

    template <class Vector>
    struct At_v< Vector , void_t< Checks::TryMemFn_SquareBracketAccessForVector< std::decay_t<Vector> > > >
    {
      template <class Index, class = std::enable_if_t< std::is_integral<Index>::value > >
      static decltype(auto) apply(Vector&& v, Index i)
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

  template <class Matrix, class Index,
            class = std::enable_if_t< std::is_integral<Index>::value > >
  __attribute__((always_inline)) decltype(auto) at(Matrix&& A, Index i, Index j)
  {
    return AtDetail::At<Matrix>::apply( std::forward<Matrix>(A), i, j );
  }

  template <class Vector, class Index,
            class = std::enable_if_t< std::is_integral<Index>::value > >
  __attribute__((always_inline)) decltype(auto) at(Vector&& v, Index i)
  {
    return AtDetail::At_v<Vector>::apply(std::forward<Vector>(v),i);
  }
  /// @endcond
}

#if defined(__GNUC__) || defined(__GNUG__)
  #pragma GCC diagnostic pop
#endif

#endif // FUNG_UTIL_AT_HH
