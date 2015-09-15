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

#ifndef FUNG_LINEAR_ALGEBRA_DEVIATORIC_INVARIANTS_HH
#define FUNG_LINEAR_ALGEBRA_DEVIATORIC_INVARIANTS_HH

#include "deviator.hh"
#include "matrix_norm.hh"
#include "fung/mathematical_operations/chain.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup InvariantGroup
     * \brief Second deviator invariant \f$ J_2(\sigma)=\sqrt{\bar\sigma\negthinspace:\negthinspace\bar\sigma} \f$ with \f$\bar\sigma = \sigma - \frac{\mathrm{tr}(\sigma)}{n}I\f$ and \f$\sigma\in\mathbb{R}^{n,n}\f$.
     */
    template <class Matrix>
    using SecondDeviatoricInvariant = MathematicalOperations::Chain< MatrixNorm<Matrix> , Deviator<Matrix> >;
  }
}
#endif // FUNG_LINEAR_ALGEBRA_DEVIATORIC_INVARIANTS_HH
