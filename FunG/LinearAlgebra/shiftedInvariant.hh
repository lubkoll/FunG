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

#ifndef FUNG_LINEAR_ALGEBRA_SHIFTED_INVARIANT_HH
#define FUNG_LINEAR_ALGEBRA_SHIFTED_INVARIANT_HH

#include <type_traits>

#include "FunG/constant.hh"
#include "FunG/Util/base.hh"
#include "FunG/Util/derivativeWrappers.hh"
#include "FunG/Util/evaluateIfPresent.hh"
#include "FunG/Util/indexedType.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Possibly scaled, shifted invariant \f$scaling (invariant - offset)\f$, where
     * \f$offset = dim\f$ for the first two (principal,modified) invariants and \f$offset = 1\f$
     * for the third (principal,modified) and mixed invariants.
     */
    template <class Invariant, int offset>
    class ShiftedInvariant : Base
    {
    public:
      ShiftedInvariant() = default;

      /**
       * \param A matrix to compute invariant from.
       */
      template <class Matrix>
      ShiftedInvariant(Matrix const& A) : inv(A)
      {
        update(A);
      }

      /**
       * \param A matrix to compute invariant from.
       * \param M structural tensor for incorporation of anisotropic information.
       */
      template <class Matrix>
      ShiftedInvariant(const Matrix& A, const Constant<Matrix>& M) : inv(A,M)
      {
        update(A);
      }

      /**
       * \param A matrix to compute invariant from.
       * \param M structural tensor for incorporation of anisotropic information.
       */
      template <class Matrix>
      ShiftedInvariant(const Matrix& A, const Matrix& M) : ShiftedInvariant(A,Constant<Matrix>(M))
      {}

      /// Reset matrix to compute shifted invariant from.
      template <class Matrix>
      void update(Matrix const& A)
      {
        inv.update(A);
        resultOfD0 = inv.d0() - offset;
      }

      /// Value of the shifted invariant.
      auto d0() const noexcept
      {
        return resultOfD0;
      }

      /**
       * \brief First directional derivative of the (scaled) shifted invariant.
       * \param dA1 direction for which the derivative is computed
       * \return derivative in direction dA1
       */
      template < int id , class Arg ,
                 class IndexedArg = IndexedType<Arg,id> ,
                 class = std::enable_if_t<EvaluateIfPresent< D1<Invariant,IndexedArg> >::present> >
      auto d1(const Arg& dA1) const
      {
        return EvaluateIfPresent< D1<Invariant,IndexedArg> > ( inv, dA1 )();
      }

      /**
       * \brief Second directional derivative of the (scaled) shifted invariant.
       * \param dA1 direction for which the derivative is computed
       * \param dA2 direction for which the derivative is computed
       * \return second derivative in the directions dA1 and dA2
       */
      template < int idx, int idy, class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idx> ,
                 class = std::enable_if_t<EvaluateIfPresent< D2<Invariant,IndexedArgX,IndexedArgY> >::present> >
      auto d2(const ArgX& dA1, const ArgY& dA2) const
      {
        return EvaluateIfPresent< D2<Invariant,IndexedArgX,IndexedArgY> >(inv,dA1,dA2)();
      }

      /**
       * \brief Third directional derivative of the (scaled) shifted invariant.
       * \param dA1 direction for which the derivative is computed
       * \param dA2 direction for which the derivative is computed
       * \param dA3 direction for which the derivative is computed
       * \return second derivative in the directions dA1, dA2 and dA3
       */
      template < int idx , int idy , int idz , class ArgX , class ArgY , class ArgZ ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class IndexedArgZ = IndexedType<ArgZ,idz> ,
                 class = std::enable_if_t< EvaluateIfPresent< D3<Invariant,IndexedArgX,IndexedArgY,IndexedArgZ> >::present > >
      auto d3(ArgX const& dA1, ArgY const& dA2, ArgZ const& dA3) const
      {
        return EvaluateIfPresent< D3<Invariant,IndexedArgX,IndexedArgY,IndexedArgZ> >(inv,dA1,dA2,dA3)();
      }

    private:
      Invariant inv;
      std::remove_const_t<std::remove_reference_t<decltype(std::declval<Invariant>().d0())> > resultOfD0 = 0.;
    };
  }
}

#endif // FUNG_LINEAR_ALGEBRA_SHIFTED_INVARIANT_HH
