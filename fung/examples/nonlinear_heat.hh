// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_EXAMPLES_NONLINEAR_HEAT_HH
#define FUNG_EXAMPLES_NONLINEAR_HEAT_HH

#include "fung/finalize.hh"
#include "fung/generate.hh"
#include "fung/variable.hh"

namespace FunG
{
  /// Yield surface \f$ \frac{\beta}{3}\iota_1(\sigma) + J_2(\sigma)-offset \f$, where \f$\iota_1\f$ is the first principal and \f$J_2\f$ is the second deviatoric invariant.  
  /*!
    \brief Weak model for nonlinear heat transfer \f$ (c+du^2)\nabla u \f$.
    \param c weighing of linearity
    \param d weighing of nonlinearity
    \param u heat
    \param du heat gradient
   */
  template <class Scalar, class Vector>
  auto heatModel(double c, double d, Scalar u, const Vector& du)
  {
    auto f = (c+d*(variable<0>(u)^2))*variable<0>(du);
//    return f;
    return finalize( f );
  }
}

#endif // FUNG_EXAMPLES_NONLINEAR_HEAT_HH
