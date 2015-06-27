#ifndef RFFGEN_LINEAR_ALGEBRA_ROWS_AND_COLS_HH
#define RFFGEN_LINEAR_ALGEBRA_ROWS_AND_COLS_HH

#include <utility>
#include "extractRowsAndCols.hh"
#include "../Util/staticChecks_nRows_nCols.hh"
#include "../Util/staticChecks.hh"

namespace RFFGen
{
  namespace LinearAlgebra
  {
    /**
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
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
    /**
     * \endcond
     */

    /**
     * \brief Number of rows of a dynamic size matrix.
     */
    template < class Matrix ,
               class = std::enable_if_t<!Checks::isConstantSizeMatrix<Matrix>()> ,
               class = std::enable_if_t<Checks::isDynamicMatrix<Matrix>()> >
    auto rows(const Matrix& A) //noexcept(std::declval<Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(A) -> decltype(Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(std::declval<Matrix>()))
    {
      return Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(A);
    }

    /**
     * \brief Number of rows of a constant size matrix.
     */
    template < class Matrix ,
               class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
    constexpr auto rows() //noexcept(std::declval<Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(A) -> decltype(Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(std::declval<Matrix>()))
    {
      return numberOfRows<Matrix>();
    }

    /**
     * \brief Number of columns of a dynamic size matrix.
     */
    template < class Matrix ,
               class = std::enable_if_t<!Checks::isConstantSizeMatrix<Matrix>()> ,
               class = std::enable_if_t<Checks::isDynamicMatrix<Matrix>()> >
    auto cols(const Matrix& A) //noexcept( std::declval< Detail::Cols<Matrix,Checks::hasColsFunction<Matrix>(),Checks::hasMember_n_cols<Matrix>()> >()(A)) //-> decltype(Detail::Cols<Matrix,Checks::hasColsFunction<Matrix>(),Checks::hasMember_n_cols<Matrix>()>()(std::declval<Matrix>()))
    {
      return Detail::Cols<Matrix,Checks::hasColsFunction<Matrix>(),Checks::hasMember_n_cols<Matrix>()>()(A);
    }

    /**
     * \brief Number of columns of a constant size matrix.
     */
    template < class Matrix ,
               class = std::enable_if_t<Checks::isConstantSizeMatrix<Matrix>()> >
    constexpr auto cols() //noexcept(std::declval<Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(A) -> decltype(Detail::Rows<Matrix,Checks::hasRowsFunction<Matrix>(),Checks::hasMember_n_rows<Matrix>()>()(std::declval<Matrix>()))
    {
      return numberOfColumns<Matrix>();
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_ROWS_AND_COLS_HH
