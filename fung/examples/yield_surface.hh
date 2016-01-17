// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

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
