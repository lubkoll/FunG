// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later./

#ifndef FUNG_LINEAR_ALGEBRA_TRACE_HH
#define FUNG_LINEAR_ALGEBRA_TRACE_HH

#include <type_traits>
#include <utility>

#include "fung/util/at.hh"
#include "fung/util/exceptions.hh"
#include "fung/util/type_traits.hh"
#include "dimension.hh"
#include "rows_and_cols.hh"

namespace FunG
{
  /// @cond
  template <class> struct Chainer;
  namespace Concepts { template <class> struct SquareMatrixConceptCheck; }
  /// @endcond

  namespace LinearAlgebra
  {
    /** @addtogroup LinearAlgebraGroup
     *  @{ */

    /// @cond
    namespace Detail
    {
      /// f Computes the trace of \f$A\f$, i.e. the sum of diagonal elements.
      template <int>
      struct ComputeTrace
      {
        template <class Matrix>
        static auto apply(const Matrix& A)
        {
          auto val = at(A,0,0);
          for(int i = 1; i < rows<Matrix>(); ++i) val += at(A,i,i);
          return val;
        }
      };

      /// Trace of 2x2-matrix.
      template <>
      struct ComputeTrace<2>
      {
        template <class Matrix>
        static auto apply(const Matrix& A)
        {
          return at(A,0,0) + at(A,1,1) ;
        }
      };

      /// Trace of a 3x3 matrix.
      template <>
      struct ComputeTrace<3>
      {
        template <class Matrix>
        static auto apply(const Matrix& A)
        {
          return at(A,0,0) + at(A,1,1) + at(A,2,2);
        }
      };
    }


    /// %Trace of a matrix, i.e. sum of diagonal elements.
    template <class Matrix, class = Concepts::SquareMatrixConceptCheck<Matrix> >
    struct ConstantSizeTrace : Chainer< ConstantSizeTrace< Matrix , Concepts::SquareMatrixConceptCheck<Matrix> > >
    {
      ConstantSizeTrace() = default;

      /**
       * @brief Constructor.
       * @param A point of evaluation.
       */
      explicit ConstantSizeTrace(const Matrix& A) { update(A); }

      /// Reset point of evaluation.
      void update(const Matrix& A)
      {
        trace = Detail::ComputeTrace<dim<Matrix>()>::apply(A);
      }

      /// Function value.
      auto d0() const noexcept
      {
        return trace;
      }

      /// First directional derivative.
      auto d1(const Matrix& dA) const
      {
        return Detail::ComputeTrace<dim<Matrix>()>::apply(dA);
      }

    private:
      std::decay_t< decltype( Detail::ComputeTrace<dim<Matrix>()>::apply(std::declval<Matrix>()) ) > trace = 0;
    };

    /// %Trace of a matrix, i.e. sum of diagonal elements.
    template <class Matrix>
    struct DynamicSizeTrace : Chainer< DynamicSizeTrace<Matrix> >
    {
      DynamicSizeTrace() = default;

      /**
       * @brief Constructor.
       * @param A point of evaluation.
       */
      explicit DynamicSizeTrace(const Matrix& A) { update(A); }

      /// Reset point of evaluation.
      void update(const Matrix& A)
      {
#ifdef FUNG_ENABLE_EXCEPTIONS
        if( rows(A) != cols(A) ) throw NonSymmetricMatrixException("DynamicSizeTrace",rows(A),cols(A),__FILE__,__LINE__);
#endif

        using Index = decltype(rows(std::declval<Matrix>()));
        trace = 0.;
        for(Index i = 0; i < rows(A); ++i) trace += at(A,i,i);
      }

      /// Function value.
      auto d0() const noexcept
      {
        return trace;
      }

      /// First directional derivative.
      auto d1(const Matrix& dA) const
      {
        using Index = decltype(rows(std::declval<Matrix>()));
        auto result = decltype(at(dA,0,0))(0.);
        for(Index i = 0; i < rows(dA); ++i) result += at(dA,i,i);
        return result;
      }

    private:
      std::decay_t< decltype(at(std::declval<Matrix>(),0,0)) > trace = 0;
    };
    /// @endcond


    /// %Trace of a matrix (sum of diagonal elements).
    template< class Matrix >
    using Trace = std::conditional_t< Checks::isConstantSize<Matrix>() , ConstantSizeTrace<Matrix> , DynamicSizeTrace<Matrix> >;

    /**
     * \brief Generate \f$\mathrm{tr}(A)\f$.
     * \return Trace<Matrix>(A)
     */
    template <class Matrix,
              std::enable_if_t<!Checks::isFunction<Matrix>()>* = nullptr>
    auto trace(const Matrix& A)
    {
      return Trace<Matrix>(A);
    }


    /**
     * \brief Generate \f$\mathrm{tr}\circ f\f$, where \f$f:\cdot\mapsto\mathbb{R}^{n,n} \f$.
     * \return Trace< std::decay_t<decltype(f.d0())> >(f.d0())( f )
     */
    template <class F,
              std::enable_if_t<Checks::isFunction<F>() >* = nullptr>
    auto trace(const F& f)
    {
      return Trace< decay_t<decltype(f.d0())> >( f.d0() )( f );
    }
    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TRACE_HH
