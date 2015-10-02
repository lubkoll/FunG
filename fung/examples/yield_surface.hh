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

#ifndef FUNG_EXAMPLES_YIELD_SURFACE_HH
#define FUNG_EXAMPLES_YIELD_SURFACE_HH

#include "fung/finalize.hh"
#include "fung/generate.hh"
#include "fung/linear_algebra.hh"

namespace FunG
{
  /// Yield surface \f$ \frac{\beta}{3}\iota_1(\sigma) + J_2(\sigma)-offset \f$, where \f$\iota_1\f$ is the first principal and \f$J_2\f$ is the second deviatoric invariant.  
  template <class Matrix>
  auto yieldSurface(double beta, double offset, Matrix sigma = LinearAlgebra::unitMatrix<Matrix>())
  {
    auto i1 = LinearAlgebra::FirstPrincipalInvariant<Matrix>( sigma );
    auto j2 = LinearAlgebra::SecondDeviatoricInvariant<Matrix>( sigma );
    auto f = (beta/LinearAlgebra::dimension<Matrix>())*i1 + j2 - offset;
    return finalize_scalar( f );
  }
}

#endif // FUNG_EXAMPLES_YIELD_SURFACE_HH
