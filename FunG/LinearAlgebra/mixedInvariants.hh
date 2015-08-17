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

#ifndef FUNG_LINEAR_ALGEBRA_MIXED_INVARIANTS_HH
#define FUNG_LINEAR_ALGEBRA_MIXED_INVARIANTS_HH

#include "../MathematicalOperations/chain.hh"
#include "../MathematicalOperations/product.hh"
#include "../MathematicalOperations/squared.hh"
#include "../constant.hh"
#include "../identity.hh"
#include "principalInvariants.hh"
#include "shiftedInvariant.hh"

namespace FunG
{

  namespace LinearAlgebra
  {
    /**
     * \ingroup InvariantGroup
     * \brief First mixed invariant \f$ \iota_4=\iota_1(AM) \f$ of a matrix \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using FirstMixedInvariant  = MathematicalOperations::Chain< FirstPrincipalInvariant<Matrix> , MathematicalOperations::Product< Identity<Matrix> , Constant<Matrix> > >;

    /**
     * \ingroup InvariantGroup
     * \brief Second mixed invariant \f$ \iota_5=\iota_1(A^2M) \f$ of a matrix \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using SecondMixedInvariant = MathematicalOperations::Chain< FirstPrincipalInvariant<Matrix> , MathematicalOperations::Product< MathematicalOperations::Squared<Identity<Matrix> >  , Constant<Matrix> > >;

    /**
     * \ingroup InvariantGroup
     * \brief Third mixed invariant \f$ \iota_6=\iota_1(AM^2) \f$ of a matrix \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using ThirdMixedInvariant  = MathematicalOperations::Chain< FirstPrincipalInvariant<Matrix> , MathematicalOperations::Product< Identity<Matrix> , MathematicalOperations::Squared< Constant<Matrix> > > >;

    /**
     * \ingroup InvariantGroup
     * \brief Shifted first mixed invariant \f$ \iota_4 - 1 \f$.
     */
    template <class Matrix>
    using ShiftedFirstMixedInvariant  = ShiftedInvariant< FirstMixedInvariant<Matrix>  , 1 >;

    /**
     * \ingroup InvariantGroup
     * \brief Shifted second mixed invariant \f$ \iota_5 - 1 \f$.
     */
    template <class Matrix>
    using ShiftedSecondMixedInvariant = ShiftedInvariant< SecondMixedInvariant<Matrix> , 1 >;

    /**
     * \ingroup InvariantGroup
     * \brief Shifted third mixed invariant \f$ \iota_6 - 1 \f$.
     */
    template <class Matrix>
    using ShiftedThirdMixedInvariant = ShiftedInvariant< ThirdMixedInvariant<Matrix>   , 1 >;
  }
}

#endif // FUNG_LINEAR_ALGEBRA_MIXED_INVARIANTS_HH
