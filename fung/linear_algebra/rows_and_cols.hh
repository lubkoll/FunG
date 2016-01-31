// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_ROWS_AND_COLS_HH
#define FUNG_LINEAR_ALGEBRA_ROWS_AND_COLS_HH

#include <utility>
#include "fung/util/extract_rows_and_cols.hh"
#include "fung/util/static_checks_nrows_ncols.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /// @cond
    template <class Matrix, class = void> struct DynamicNumberOfRows;

    template < class Matrix >
    struct DynamicNumberOfRows< Matrix , void_t< Checks::TryMemFn_rows<Matrix> > >
    {
      static auto apply(const Matrix& A)
      {
        return A.rows();
      }
    };

    template < class Matrix  >
    struct DynamicNumberOfRows< Matrix , void_t< Checks::TryMem_n_rows<Matrix> > >
    {
      static const auto& apply(const Matrix& A) noexcept
      {
        return A.n_rows;
      }
    };


    template <class Matrix, class = void> struct DynamicNumberOfColumns;

    template < class Matrix >
    struct DynamicNumberOfColumns< Matrix , void_t< Checks::TryMemFn_cols<Matrix> > >
    {
      static auto apply(const Matrix& A)
      {
        return A.cols();
      }
    };

    template < class Matrix  >
    struct DynamicNumberOfColumns< Matrix , void_t< Checks::TryMem_n_cols<Matrix> > >
    {
      static const auto& apply(const Matrix& A) noexcept
      {
        return A.n_cols;
      }
    };
    /// @endcond


    /// Number of rows of a dynamic size matrix.
    template < class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
    auto rows(const Matrix& A)
    {
      return DynamicNumberOfRows< Matrix >::apply( A );
    }

    /// Number of rows of a constant size matrix.
    template < class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
    constexpr auto rows()
    {
      return NumberOfRows< Matrix >::value;
    }

    /// Number of columns of a dynamic size matrix.
    template < class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr>
    auto cols(const Matrix& A)
    {
      return DynamicNumberOfColumns< Matrix >::apply( A );
    }

    /// Number of columns of a constant size matrix.
    template < class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
    constexpr auto cols()
    {
      return NumberOfColumns< Matrix >::value;
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_ROWS_AND_COLS_HH
