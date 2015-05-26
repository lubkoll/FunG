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

#ifndef RFFGEN_CMATH_TAN_HH
#define RFFGEN_CMATH_TAN_HH

#include <cmath>
#include "../Util/base.hh"
#include "../Util/chainer.hh"

namespace RFFGen
{
  namespace CMath
  {
    /**
     * \ingroup CMathGroup
     *
     * \brief Tangent function including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    struct Tan : Base , Chainer<Tan>
    {
      using Chainer<Tan>::operator ();
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Tan(double x=0.) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
        value = ::tan(x);
        firstDerivative = 1 + ( value * value );
      }

      /// Function value.
      double d0() const noexcept
      {
        return value;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return firstDerivative * dx;
      }

      /// Second (directinal) derivative
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return ( 2 * value * firstDerivative ) * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return 2 * firstDerivative * ( 1 + ( 3 * value * value ) ) * dx * dy * dz;
      }

    private:
      double value = 0., firstDerivative = 1.;
    };

    /**
     * \brief Plug f into tangent function.
     * \return object of type Chain<Tan,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto tan(const Function& f)
    {
      return Tan()(f);
    }
  }
}

#endif // RFFGEN_CMATH_TAN_HH
