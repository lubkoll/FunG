// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_DETERMINANT_HH
#define FUNG_LINEAR_ALGEBRA_DETERMINANT_HH

#include <type_traits>
#include <utility>

#include "dimension.hh"
#include "rows_and_cols.hh"
#include "fung/util/at.hh"
#include "fung/util/chainer.hh"
#include "fung/util/exceptions.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/type_traits.hh"
#include "fung/util/zero.hh"

namespace FunG
{
  /// @cond
  namespace Concepts {  template <class> struct SquareMatrixConceptCheck; }
  /// @endcond

  namespace LinearAlgebra
  {
    /** @addtogroup LinearAlgebraGroup
     *  @{ */

    /// @cond
    namespace Detail
    {
      template < class Matrix >
      inline auto composeResult(Matrix const& A, Matrix const& B)
      {
        return at(A,0,0) * at(B,1,1) + at(A,1,1) * at(B,0,0) - ( at(A,0,1) * at(B,1,0) + at(A,1,0) * at(B,0,1) );
      }

      template < class Matrix >
      inline auto composeResult(Matrix const& dA, Matrix const& dB, Matrix const& dC)
      {
        return at(dB,1,1) * ( at(dA,0,0) * at(dC,2,2) - at(dA,2,0) * at(dC,0,2) ) +
            at(dB,1,2) * ( at(dA,0,1) * at(dC,2,0) - at(dA,2,1) * at(dC,0,0) ) +
            at(dB,1,0) * ( at(dA,0,2) * at(dC,2,1) - at(dA,2,2) * at(dC,0,1) );
      }

      template < class Matrix >
      inline auto composeSemiSymmetricResult(Matrix const& dA, Matrix const& dB, Matrix const& dC)
      {
        return at(dB,1,1) * ( at(dA,0,0) * at(dC,2,2) + at(dA,2,2) * at(dC,0,0) - at(dA,2,0) * at(dC,0,2) - at(dA,0,2) * at(dC,2,0) ) +
            at(dB,1,2) * ( at(dA,0,1) * at(dC,2,0) + at(dA,2,0) * at(dC,0,1) - at(dA,2,1) * at(dC,0,0) - at(dA,0,0) * at(dC,2,1) ) +
            at(dB,1,0) * ( at(dA,0,2) * at(dC,2,1) + at(dA,2,1) * at(dC,0,2) - at(dA,2,2) * at(dC,0,1) - at(dA,0,1) * at(dC,2,2) );
      }

      template <class Matrix, int dim, class = Concepts::SquareMatrixConceptCheck<Matrix> >
      class DeterminantImpl;

      template<class Matrix>
      class DeterminantImpl< Matrix , 2 , Concepts::SquareMatrixConceptCheck<Matrix> >
          : public Chainer< DeterminantImpl<Matrix,2,Concepts::SquareMatrixConceptCheck<Matrix> > >
      {
      public:
        DeterminantImpl() = default;

        explicit DeterminantImpl(Matrix const& A_) { update(A_); }

        void update(Matrix const& A_)
        {
          if( !initialized )
          {
            new(&A) Matrix(A_);
            initialized = true;
          }
          else A = A_;
          value = at(A,0,0) * at(A,1,1) - at(A,0,1) * at(A,1,0);
        }

        auto d0() const
        {
          return value;
        }

        auto d1(Matrix const& dA1) const
        {
          return composeResult(A, dA1);
        }

        auto d2(Matrix const& dA1, Matrix const& dA2) const
        {
          return composeResult(dA2,dA1);
        }

      private:
        Matrix A;
        std::decay_t< decltype(at(std::declval<Matrix>(),0,0)) > value = 0.;
        bool initialized = false;
      };

      template <class Matrix>
      class DeterminantImpl<Matrix,3,Concepts::SquareMatrixConceptCheck<Matrix> >
          : public Chainer< DeterminantImpl<Matrix,3,Concepts::SquareMatrixConceptCheck<Matrix> > >
      {
      public:
        DeterminantImpl() = default;

        DeterminantImpl(Matrix const& A_) { update(A_); }

        void update(Matrix const& A_)
        {
          if( !initialized )
          {
            new(&A) Matrix(A_);
            initialized = true;
          }
          else A = A_;
          value = composeResult(A,A,A);
        }

        auto d0() const { return value; }

        auto d1(Matrix const& dA1) const
        {
          return composeResult(dA1,A,A) + composeResult(A,dA1,A) + composeResult(A,A,dA1);
        }

        auto d2(Matrix const& dA1, Matrix const& dA2) const
        {
          return composeSemiSymmetricResult(A,dA2,dA1) + composeSemiSymmetricResult(dA1,A,dA2) + composeSemiSymmetricResult(A,dA1,dA2);
        }

        auto d3(Matrix const& dA1, Matrix const& dA2, Matrix const& dA3) const
        {
          return composeSemiSymmetricResult(dA1,dA2,dA3) + composeSemiSymmetricResult(dA1,dA3,dA2) + composeSemiSymmetricResult(dA2,dA1,dA3);
        }

      private:
        Matrix A;
        std::decay_t< decltype(at(std::declval<Matrix>(),0,0)) > value = 0.;
        bool initialized = false;
      };
    }

    /// Determinant of constant size matrix with first three derivatives.
    template <class Matrix>
    using ConstantSizeDeterminant = Detail::DeterminantImpl<Matrix,dim<Matrix>()>;

    /// Determinant of dynamic size matrix with first three derivatives.
    template <class Matrix>
    class DynamicSizeDeterminant :
        public Chainer< DynamicSizeDeterminant<Matrix> >
    {
    public:
      DynamicSizeDeterminant() = default;

      /// Constructor.
      DynamicSizeDeterminant(Matrix const& A) : dim(rows(A))
      {
        update(A);
      }

      /// Reset point of evaluation.
      void update(Matrix const& A)
      {
#ifdef FUNG_ENABLE_EXCEPTIONS
        if( rows(A) != cols(A) ) throw NonSymmetricMatrixException("DynamicSizeDeterminant",rows(A),cols(A),__FILE__,__LINE__);
#endif
        dim = rows(A);
        if( dim == 2 ) det2D.update(A);
        if( dim == 3 ) det3D.update(A);
      }

      /// Function value.
      auto d0() const { return ( dim==2 ) ? det2D.d0() : det3D.d0(); }

      /// First (directional) derivative.
      auto d1(Matrix const& dA1) const
      {
        return ( dim==2 ) ? det2D.d1(dA1) : det3D.d1(dA1) ;
      }

      /// Second (directional) derivative.
      auto d2(Matrix const& dA1, Matrix const& dA2) const
      {
        return ( dim==2 ) ? det2D.d2(dA1,dA2) : det3D.d2(dA1,dA2);
      }

        /// Third (directional) derivative.
      auto d3(Matrix const& dA1, Matrix const& dA2, Matrix const& dA3) const
      {
        return ( dim==2 ) ? 0 : det3D.d3(dA1,dA2,dA3);
      }

    private:
      int dim = 0;
      Detail::DeterminantImpl<Matrix,2> det2D;
      Detail::DeterminantImpl<Matrix,3> det3D;
    };

    /// Determinant with first three derivatives.
    template < class Matrix >
    using Determinant = std::conditional_t< Checks::isConstantSize<Matrix>() , ConstantSizeDeterminant<Matrix> , DynamicSizeDeterminant<Matrix> >;
    /// @endcond


    /**
     * @brief Generate \f$\det(A)\f$.
     * @param A square matrix
     * @return Determinant<Matrix>(A)
     */
    template<class Matrix,
             std::enable_if_t<!Checks::isFunction<Matrix>()>* = nullptr>
    auto det(Matrix const& A)
    {
      return Determinant<Matrix>(A);
    }

    /**
     * @brief Generate \f$\det\circ f\f$.
     * @param f function mapping into a space of square matrices
     * @return Determinant< std::decay_t<decltype(f.d0())> >(f.d0())(f)
     */
    template<class F,
             std::enable_if_t<Checks::isFunction<F>() >* = nullptr>
    auto det(F const& f)
    {
      return Determinant< decay_t<decltype(f.d0())> >(f.d0())(f);
    }

    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_DETERMINANT_HH
