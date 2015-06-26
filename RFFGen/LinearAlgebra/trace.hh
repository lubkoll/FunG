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

#ifndef RFFGEN_LINEAR_ALGEBRA_TRACE_HH
#define RFFGEN_LINEAR_ALGEBRA_TRACE_HH

#include <type_traits>
#include <utility>

#include "dimension.hh"
#include "rowsAndCols.hh"
#include "../Util/base.hh"
#include "../Util/at.hh"
#include "../Util/exceptions.hh"

namespace RFFGen
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
      template <class Matrix>
      auto trace(Matrix const& A)
      {
        auto val = at(A,0,0);
        for(int i = 1; i < rows<Matrix>(); ++i) val += at(A,i,i);
        return val;
      }

      /// Trace of 2x2-matrix.
      template <class Matrix>
      auto computeTrace(const Matrix& A, std::integral_constant<int,2>)
      {
        return at(A,0,0) + at(A,1,1) ;
      }

      /// Trace of a 3x3 matrix.
      template <class Matrix>
      __attribute__((always_inline)) auto computeTrace(const Matrix& A, std::integral_constant<int,3>)
      {
        return at(A,0,0) + at(A,1,1) + at(A,2,2);
      }

      template <int a, int b> struct Bigger { static constexpr bool value = a > b; };

      /// General implementation of a matrix trace.
      template <class Matrix, int n, class = std::enable_if_t<Bigger<n,3>::value> >
      auto computeTrace(const Matrix& A, std::integral_constant<int,n>)
      {
        return trace(A);
      }
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
        trace = Detail::computeTrace(A,std::integral_constant<int,dimension<Matrix>()>());
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
        return Detail::computeTrace(dA,std::integral_constant<int,dimension<Matrix>()>());
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
#ifdef RFFGEN_ENABLE_EXCEPTIONS
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
    template <class Matrix>
    auto trace_impl(const Matrix& A, std::true_type)
    {
      return Trace<Matrix>(A);
    }

    template <class Function>
    auto trace_impl(const Function& f, std::false_type)
    {
      return Trace< std::decay_t<decltype(f.d0())> >( f.d0() )( f );
    }
    /**
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Convenient generation of Trace<Matrix>.
     * \return Trace<Matrix>(A)
     */
    template <class Arg>
    auto trace(const Arg& arg)
    {
      return trace_impl( arg , std::integral_constant<bool,!std::is_base_of<Base,Arg>::value>() );
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_TRACE_HH
