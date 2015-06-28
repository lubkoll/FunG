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

#ifndef RFFGEN_EXAMPLES_YIELD_SURFACE_HH
#define RFFGEN_EXAMPLES_YIELD_SURFACE_HH

#include "../linearAlgebra.hh"
#include "../generate.hh"
#include "../finalize.hh"

namespace RFFGen
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

#endif // RFFGEN_EXAMPLES_YIELD_SURFACE_HH
