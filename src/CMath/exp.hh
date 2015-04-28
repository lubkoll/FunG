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

#ifndef RFFGEN_CMATH_EXP_HH
#define RFFGEN_CMATH_EXP_HH

#include <cmath>
#include "../Util/base.hh"

namespace RFFGen
{
  namespace CMath
  {
    /**
     * \ingroup CMathGroup
     *
     * \brief Exponential function including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    struct Exp : Base
    {
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Exp(double x=0.) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
        e_x = exp(x);
      }

      /// Function value. Convenient access to d0().
      double operator()() const noexcept
      {
        return d0();
      }

      /// Function value.
      double d0() const noexcept
      {
        return e_x;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return e_x * dx;
      }

      /// Second (directinal) derivative
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return e_x * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return e_x * dx * dy * dz;
      }

    private:
      double e_x = 1.;
    } expo ; ///< Object of type Exp for the convenient generation of functions.
  }
}

#endif // RFFGEN_CMATH_EXP_HH
