// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
#define FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH

#include <type_traits>

#include "fung/util/at.hh"
#include "fung/util/chainer.hh"
#include "fung/util/extract_rows_and_cols.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/zero.hh"
#include "rows_and_cols.hh"
#include "fung/concept_check.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /// @cond
    namespace Detail
    {
      template <class Matrix, class TransposedMatrix = typename GetTransposed<Matrix>::type,
                std::enable_if_t<std::is_same<Matrix,TransposedMatrix>::value && Checks::isConstantSize<Matrix>()>* = nullptr>
      TransposedMatrix transpose(Matrix A)
      {
        auto a = at(A,0,0);
        for(int i=0; i<rows<Matrix>(); ++i)
          for(int j=i+1; j<cols<Matrix>(); ++j)
          {
            a = at(A,i,j);
            at(A,i,j) = at(A,j,i);
            at(A,j,i) = a;
          }

        return A;
      }

      /// Compute transpose of non-square matrix.
      template <class Matrix, class TransposedMatrix = Transposed_t<Matrix>,
                std::enable_if_t<!std::is_same<Matrix,TransposedMatrix>::value && Checks::isConstantSize<Matrix>()>* = nullptr >
      TransposedMatrix transpose(const Matrix& A)
      {
        TransposedMatrix B = zero<TransposedMatrix>();
        for(int i=0; i<rows<Matrix>(); ++i)
          for(int j=0; j<cols<Matrix>(); ++j)
            at(B,j,i) = A(i,j);
        return B;
      }


      /// Compute transpose of square matrix.
      template <class Matrix ,
                std::enable_if_t<!Checks::isConstantSize<Matrix>()>* = nullptr >
      Matrix transpose(Matrix A)
      {
        assert(rows(A) == cols(A));
        using Index = decltype(rows(std::declval<Matrix>()));
        auto a = std::decay_t<decltype(at(A,0,0))>(0.);
        for(Index i=0; i<rows(A); ++i)
          for(Index j=i+1; j<cols(A); ++j)
          {
            a = at(A,i,j);
            at(A,i,j) = at(A,j,i);
            at(A,j,i) = a;
          }

        return A;
      }
    }
    /// @endcond

    /** @addtogroup LinearAlgebraGroup
     *  @{ */
    template < class Matrix, class = Concepts::MatrixConceptCheck<Matrix> >
    class Transpose;

    /// Represents transposition of constant-size matrices.
    template <class Matrix>
    class Transpose< Matrix, Concepts::MatrixConceptCheck<Matrix> >
        : public Chainer< Transpose< Matrix, Concepts::MatrixConceptCheck<Matrix> > >
    {
    public:
      explicit Transpose( const Matrix& A ) {
        AT_ = Detail::transpose( A );
      }

      void update( const Matrix& A ) {
        AT_ = Detail::transpose( A );
      }

      const auto& d0() const noexcept {
        return AT_;
      }

      auto d1( const Matrix& dA ) const {
        return Detail::transpose( dA );
      }

    private:
      struct WrappedMatrix {
        using type = Matrix;
      };

      typename std::conditional< Checks::isConstantSize<Matrix>(), GetTransposed<Matrix>, WrappedMatrix >::type::type AT_;
    };

    /**
     * \brief Generate \f$A^T\in\mathbb{R}^{n,n}\f$.
     * \param A square matrix
     * \return Transpose<Matrix>(A)
     */
    template <class Matrix,
              std::enable_if_t<!Checks::isFunction<Matrix>()>* = nullptr>
    auto transpose(const Matrix& A)
    {
      return Transpose<Matrix>( A );
    }


    /**
     * \brief Generate \f$f^T\f$, where \f$f:\cdot\mapsto\mathbb{R}^{n,n} \f$.
     * \param f function object mapping into a space of square matrices
     * \return Transpose< decay_t<decltype(f())> >(f())( f )
     */
    template <class F,
              std::enable_if_t<Checks::isFunction<F>() >* = nullptr>
    auto transpose(const F& f)
    {
      return Transpose< decay_t<decltype(f())> >( f() )( f );
    }
    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TRANSPOSE_HH
