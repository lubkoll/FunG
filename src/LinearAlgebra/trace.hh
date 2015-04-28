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
#include "../Util/base.hh"
#include "../Util/at.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
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
        for(int i = 1; i < dimension<Matrix>(); ++i) val += at(A,i,i);
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
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief %Trace of a matrix, i.e. sum of diagonal elements.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct Trace : Base
    {
      /// Default constructor.
      Trace() = default;

      /**
       * @brief Constructor.
       * @param A point of evaluation.
       */
      explicit Trace(const Matrix& A) { update(A); }

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
      std::remove_const_t<std::remove_reference_t< at_t<Matrix> > > trace = 0.;
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Convenient generation of Trace<Matrix>.
     * \return Trace<Matrix>(A)
     */
    template <class Matrix>
    auto trace(const Matrix& A)
    {
      return Trace<Matrix>(A);
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_TRACE_HH
