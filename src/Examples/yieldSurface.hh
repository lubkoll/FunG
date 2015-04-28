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

#include "../LinearAlgebra/deviatoricInvariants.hh"
#include "../LinearAlgebra/principalInvariants.hh"
#include "../math.hh"
#include "../finalize.hh"
#include "../Util/third.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class Matrix>
    using YieldSurfaceBase = Sum < Scale< FirstPrincipalInvariant<Matrix> >,
                                   SecondDeviatoricInvariant<Matrix>,
                                   Constant<double>
                                 >;
  }
  /**
   * \endcond
   */


  /// Yield surface \f$ \frac{\beta}{3}\iota_1(\sigma) + J_2(\sigma)-offset \f$, where \f$\iota_1\f$ is the first principal and \f$J_2\f$ is the second deviatoric invariant.
  template <class Matrix>
  struct YieldSurface : public Finalize< Detail::YieldSurfaceBase<Matrix> >
  {
    /**
     * @brief Constructor.
     * @param beta scaling
     * @param offset offset
     * @param sigma stress tensor
     */
    explicit YieldSurface(double beta, double offset, Matrix sigma = unitMatrix<Matrix>())
      : Finalize< Detail::YieldSurfaceBase<Matrix> >
        (
          std::make_tuple(beta*third() , sigma), // constructor argument for type Scale< FirstPrincipalInvariant<Matrix> >
          sigma,                                                                 // constructor argument for SecondDeviatoricInvariant<Matrix>
          -offset                                                                // constructor argument for Constant<double>
        )
    {}
  };
}

#endif // RFFGEN_EXAMPLES_YIELD_SURFACE_HH
