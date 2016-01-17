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
    namespace Detail
    {
      template <class Matrix, bool accessViaRows, bool accessVia_n_rows> struct Rows;

      template < class Matrix , bool accessVia_n_rows >
      struct Rows<Matrix,true,accessVia_n_rows>
      {
        auto operator()(const Matrix& A) const
        {
          return A.rows();
        }
      };

      template < class Matrix  >
      struct Rows<Matrix,false,true>
      {
        const auto& operator()(const Matrix& A) const noexcept
        {
          return A.n_rows;
        }
      };

      template <class Matrix, bool accessViaCols, bool accessVia_n_cols> struct Cols;

      template < class Matrix , bool accessVia_n_cols >
      struct Cols<Matrix,true,accessVia_n_cols>
      {
        auto operator()(const Matrix& A) const
        {
          return A.cols();
        }
      };

      template < class Matrix >
      struct Cols<Matrix,false,true>
      {
        const auto& operator()(const Matrix& A) const noexcept
        {
          return A.n_cols;
        }
      };
    }
    /// @endcond

    /// Number of rows of a dynamic size matrix.
    template < class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
    auto rows(const Matrix& A)
    {
      return Detail::Rows<Matrix,Checks::hasMemFn_rows<Matrix>(),Checks::hasMem_n_rows<Matrix>()>()(A);
    }

    /// Number of rows of a constant size matrix.
    template < class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
    constexpr auto rows()
    {
      return numberOfRows<Matrix>();
    }

    /// Number of columns of a dynamic size matrix.
    template < class Matrix ,
               std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr>
    auto cols(const Matrix& A)
    {
      return Detail::Cols<Matrix,Checks::hasMemFn_cols<Matrix>(),Checks::hasMem_n_cols<Matrix>()>()(A);
    }

    /// Number of columns of a constant size matrix.
    template < class Matrix ,
               std::enable_if_t<Checks::isConstantSize<Matrix>()>* = nullptr >
    constexpr auto cols()
    {
      return numberOfColumns<Matrix>();
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_ROWS_AND_COLS_HH
