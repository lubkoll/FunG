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

#ifndef RFFGEN_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH
#define RFFGEN_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH

#include <type_traits>

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts { template <class> struct MatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /// Specialize this for your matrix class. Number of rows must be provided by a static member variable called value.
    template < class Matrix , class = Concepts::MatrixConceptCheck<Matrix> > struct ExtractNumberOfRows;

    /// Specialize this for your matrix class. Number of columns must be provided by a static member variable called value.
    template < class Matrix , class = Concepts::MatrixConceptCheck<Matrix> > struct ExtractNumberOfColumns;


    template < template <class,int,int> class Matrix, class T, int n, int m, class MatrixConceptCheck>
    struct ExtractNumberOfRows< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    template < template <class,unsigned,unsigned> class Matrix, class T, unsigned n, unsigned m, class MatrixConceptCheck>
    struct ExtractNumberOfRows< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};


    template < template <int,int> class Matrix, int n, int m, class MatrixConceptCheck>
    struct ExtractNumberOfRows< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    template < template <unsigned,unsigned> class Matrix, unsigned n, unsigned m, class MatrixConceptCheck>
    struct ExtractNumberOfRows< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};



    template < template <class,int,int> class Matrix, class T, int n, int m, class MatrixConceptCheck>
    struct ExtractNumberOfColumns< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<int,m> {};

    template < template <class,unsigned,unsigned> class Matrix, class T, unsigned n, unsigned m, class MatrixConceptCheck>
    struct ExtractNumberOfColumns< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,m> {};


    template < template <int,int> class Matrix, int n, int m, class MatrixConceptCheck>
    struct ExtractNumberOfColumns< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<int,m> {};

    template < template <unsigned,unsigned> class Matrix, unsigned n, unsigned m, class MatrixConceptCheck>
    struct ExtractNumberOfColumns< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,m> {};


    /// Number of rows \f$n\f$ of a fixed size matrix in \f$\mathbb{R}^{n,m}\f$.
    template <class Matrix>
    constexpr int numberOfRows()
    {
      return ExtractNumberOfRows<Matrix>::value;
    }

    /// Number of columns \f$m\f$ of a fixed size matrix in \f$\mathbb{R}^{n,m}\f$.
    template <class Matrix>
    constexpr int numberOfColumns()
    {
      return ExtractNumberOfColumns<Matrix>::value;
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH
