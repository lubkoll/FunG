// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH
#define FUNG_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH

#include <type_traits>

namespace FunG
{
  /// @cond
  namespace Concepts { template <class> struct MatrixConceptCheck; }
  /// @endcond

  namespace LinearAlgebra
  {
    /// Specialize this for your matrix class. Number of rows must be provided by a static member variable called value.
    template < class Matrix , class = Concepts::MatrixConceptCheck<Matrix> > struct NumberOfRows : std::integral_constant<int,-1> {};

    /// Specialize this for your matrix class. Number of columns must be provided by a static member variable called value.
    template < class Matrix , class = Concepts::MatrixConceptCheck<Matrix> > struct NumberOfColumns : std::integral_constant<int,-1> {};

    /// Specialization for vectors.


    /// Specialization for matrices.
    template < template <class,int,int...> class Matrix, class T, int n, class MatrixConceptCheck, int... m>
    struct NumberOfRows< Matrix<T,n,m...> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    /// Specialization for matrices.
    template < template <class,unsigned,unsigned> class Matrix, class T, unsigned n, unsigned m, class MatrixConceptCheck>
    struct NumberOfRows< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};    

    /// Specialization for matrices.
    template < template <int,int> class Matrix, int n, int m, class MatrixConceptCheck>
    struct NumberOfRows< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    /// Specialization for matrices.
    template < template <unsigned,unsigned> class Matrix, unsigned n, unsigned m, class MatrixConceptCheck>
    struct NumberOfRows< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};


    /// Specialization for vectors.
    template < template <class,int> class Vector, class T, int n, class MatrixConceptCheck>
    struct NumberOfRows< Vector<T,n> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    /// Specialization for vectors.
    template < template <class,unsigned> class Vector, class T, unsigned n, class MatrixConceptCheck>
    struct NumberOfRows< Vector<T,n> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};

    /// Specialization for vectors.
    template < template <int> class Vector, int n, class MatrixConceptCheck>
    struct NumberOfRows< Vector<n> , MatrixConceptCheck > : std::integral_constant<int,n> {};

    /// Specialization for vectors.
    template < template <unsigned> class Vector, unsigned n, class MatrixConceptCheck>
    struct NumberOfRows< Vector<n> , MatrixConceptCheck > : std::integral_constant<unsigned,n> {};


    /// Specialization for matrices.
    template < template <class,int,int> class Matrix, class T, int n, int m, class MatrixConceptCheck>
    struct NumberOfColumns< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<int,m> {};

    /// Specialization for matrices.
    template < template <class,int,int,int...> class Matrix, class T, int n, int m, class MatrixConceptCheck, int... other>
    struct NumberOfColumns< Matrix<T,n,m,other...> , MatrixConceptCheck > : std::integral_constant<int,m> {};

    /// Specialization for matrices.
    template < template <class,unsigned,unsigned> class Matrix, class T, unsigned n, unsigned m, class MatrixConceptCheck>
    struct NumberOfColumns< Matrix<T,n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,m> {};


    /// Specialization for matrices.
    template < template <int,int> class Matrix, int n, int m, class MatrixConceptCheck>
    struct NumberOfColumns< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<int,m> {};

    /// Specialization for matrices.
    template < template <unsigned,unsigned> class Matrix, unsigned n, unsigned m, class MatrixConceptCheck>
    struct NumberOfColumns< Matrix<n,m> , MatrixConceptCheck > : std::integral_constant<unsigned,m> {};


    /// Number of rows \f$n\f$ of a fixed size matrix in \f$\mathbb{R}^{n,m}\f$ or a fixed size vector in \f$\mathbb{R}^n\f$.
    template <class Matrix>
    constexpr int numberOfRows()
    {
      return NumberOfRows<Matrix>::value;
    }

    /// Number of columns \f$m\f$ of a fixed size matrix in \f$\mathbb{R}^{n,m}\f$.
    template <class Matrix>
    constexpr int numberOfColumns()
    {
      return NumberOfColumns<Matrix>::value;
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_EXTRACTROWSANDCOLS_HH
