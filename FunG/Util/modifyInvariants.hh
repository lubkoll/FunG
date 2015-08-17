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

#ifndef FUNG_UTIL_MODIFY_INVARIANTS_HH
#define FUNG_UTIL_MODIFY_INVARIANTS_HH

#include "../operations.hh"
#include "../CMath/pow.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class Matrix,class FirstInvariant>
    using FirstPrincipalInvariantBase  = MathematicalOperations::Product< MathematicalOperations::Chain<CMath::Pow<-1,3>  , LinearAlgebra::ThirdPrincipalInvariant<Matrix> > , FirstInvariant >;

    template <class Matrix,class SecondInvariant>
    using SecondPrincipalInvariantBase = MathematicalOperations::Product< MathematicalOperations::Chain<CMath::Pow<-2,3> , LinearAlgebra::ThirdPrincipalInvariant<Matrix> > , SecondInvariant >;
  }

  template <class Matrix, class FirstInvariant>
  struct ModifyFirstPrincipalInvariant : Detail::FirstPrincipalInvariantBase<Matrix,FirstInvariant>
  {
    ModifyFirstPrincipalInvariant() = default;

    ModifyFirstPrincipalInvariant(const Matrix& A, const FirstInvariant& inv)
      : Detail::FirstPrincipalInvariantBase<Matrix,FirstInvariant> ( A , inv )
    {}
  };

  template <class Matrix, class SecondInvariant>
  struct ModifySecondPrincipalInvariant : Detail::SecondPrincipalInvariantBase<Matrix,SecondInvariant>
  {
    ModifySecondPrincipalInvariant() = default;

    ModifySecondPrincipalInvariant(const Matrix& A, const SecondInvariant& inv)
      : Detail::SecondPrincipalInvariantBase<Matrix,SecondInvariant> ( A , inv )
    {}
  };
  /**
   * \endcond
   */
}

#endif // FUNG_UTIL_MODIFY_INVARIANTS_HH
