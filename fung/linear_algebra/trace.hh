/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_LINEAR_ALGEBRA_TRACE_HH
#define FUNG_LINEAR_ALGEBRA_TRACE_HH

#include <type_traits>
#include <utility>

#include "fung/util/at.hh"
#include "fung/util/base.hh"
#include "fung/util/exceptions.hh"
#include "dimension.hh"
#include "rows_and_cols.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct Chainer;
  namespace Concepts { template <class> struct SymmetricMatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
    namespace Detail
    {
      /**
       * \brief Computes the trace of \f$A\f$, i.e. the sum of diagonal elements.
       */
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


    /**
     * \ingroup LinearAlgebraGroup
     * \brief %Trace of a matrix, i.e. sum of diagonal elements.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct ConstantSizeTrace : Base , Chainer< ConstantSizeTrace< Matrix , Concepts::SymmetricMatrixConceptCheck<Matrix> > >
    {
      using Chainer< ConstantSizeTrace< Matrix , Concepts::SymmetricMatrixConceptCheck<Matrix> > >::operator ();
      /// Default constructor.
      ConstantSizeTrace() = default;

      /**
       * @brief Constructor.
       * @param A point of evaluation.
       */
      explicit ConstantSizeTrace(const Matrix& A) { update(A); }

      /// Reset point of evaluation.
      void update(const Matrix& A)
      {
        trace = Detail::ComputeTrace<dimension<Matrix>()>::apply(A);
      }

      /// Function value. Convenient access to d0.
      const auto& operator()() const noexcept
      {
        return d0();
      }

      /// Function value. Convenient access to d0 with prior call to update(A).
      const auto& operator()(const Matrix& A) const
      {
        update(A);
        return d0();
      }

      /// Function value.
      const auto& d0() const noexcept
      {
        return trace;
      }

      /// First directional derivative.
      template <int>
      auto d1(const Matrix& dA) const
      {
        return Detail::ComputeTrace<dimension<Matrix>()>::apply(dA);
      }

    private:
      std::decay_t< at_t<Matrix> > trace = 0.;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief %Trace of a matrix, i.e. sum of diagonal elements.
     */
    template <class Matrix/*, class = Concepts::SymmetricMatrixConceptCheck<Matrix> */>
    struct DynamicSizeTrace : Base , Chainer< DynamicSizeTrace<Matrix> >
    {
      using Chainer< DynamicSizeTrace<Matrix> >::operator ();
      /// Default constructor.
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

      /// Function value. Convenient access to d0.
      const auto& operator()() const noexcept
      {
        return d0();
      }

      /// Function value. Convenient access to d0 with prior call to update(A).
      const auto& operator()(const Matrix& A) const
      {
        update(A);
        return d0();
      }

      /// Function value.
      const auto& d0() const noexcept
      {
        return trace;
      }

      /// First directional derivative.
      template <int>
      auto d1(const Matrix& dA) const
      {
        using Index = decltype(rows(std::declval<Matrix>()));
        auto result = decltype(at(dA,0,0))(0.);
        for(Index i = 0; i < rows(dA); ++i) result += at(dA,i,i);
        return result;
      }

    private:
      std::decay_t< at_t<Matrix> > trace = 0.;
    };
    /**
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Trace of a matrix (sum of diagonal elements).
     */
    template< class Matrix >
    using Trace = std::conditional_t< Checks::isConstantSizeMatrix<Matrix>() , ConstantSizeTrace<Matrix> , DynamicSizeTrace<Matrix> >;

    /**
     * \cond DOCUMENT_IMPLEMENATION_DETAILS
     */
    namespace Detail
    {
      template <bool>
      struct TraceImpl
      {
        template <class Matrix>
        static auto apply(const Matrix& A)
        {
          return Trace<Matrix>(A);
        }
      };

      template <>
      struct TraceImpl<true>
      {
        template <class Function>
        static auto apply(const Function& f)
        {
          return Trace< std::decay_t<decltype(f.d0())> >( f.d0() )( f );
        }
      };
    }
    /**
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Convenient generation of Trace<Matrix>.
     * \return Trace< std::decay_t<decltype(f.d0())> >(arg.d0())( arg ) if arg is a function, and else Trace<Arg>(arg)
     */
    template <class Arg>
    auto trace(const Arg& arg)
    {
      return Detail::TraceImpl<std::is_base_of<Base,Arg>::value>::apply(arg);
    }
  }
}

#endif // FUNG_LINEAR_ALGEBRA_TRACE_HH
