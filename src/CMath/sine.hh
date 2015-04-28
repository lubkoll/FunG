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

#ifndef RFFGEN_CMATH_SINE_HH
#define RFFGEN_CMATH_SINE_HH

#include <cmath>
#include "../Util/base.hh"

namespace RFFGen
{
  namespace CMath
  {
    /**
     * \ingroup CMathGroup
     *
     * \brief Sine function including first three derivatives (based on sin(double) in \<cmath\>).
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    struct Sin : Base
    {
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Sin(double x=0.)
      {
        update(x);
      }

      /// Reset point of evaluation.
      void update(double x)
      {
        sinx = sin(x);
        cosx = cos(x);
      }

      /// Function value. Convenient access to d0().
      double operator()() const noexcept
      {
        return d0();
      }

      /// Function value.
      double d0() const noexcept { return sinx; }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx=1.) const { return cosx*dx; }

      /// Second (directional) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx=1., double dy=1.) const { return -sinx*dx*dy; }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1>
      double d3(double dx=1., double dy=1., double dz=1.) const { return -cosx*dx*dy*dz; }

    private:
      double sinx, cosx;
    } sine ; ///< Object of type Sine for the convenient generation of functions.
  }
}


#endif // RFFGEN_CMATH_SINE_HH
