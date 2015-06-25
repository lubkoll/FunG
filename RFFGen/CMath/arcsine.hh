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

#ifndef RFFGEN_CMATH_ARCSINE_HH
#define RFFGEN_CMATH_ARCSINE_HH

#include <cmath>
#include "../Util/base.hh"
#include "../Util/chainer.hh"
#include "../Util/exceptions.hh"

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
    struct ASin : Base , Chainer<ASin>
    {
      using Chainer<ASin>::operator ();
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit ASin(double x=0.)
      {
        update(x);
      }

      /// Reset point of evaluation.
      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
        if( x < -1 || x > 1 ) throw OutOfDomainException("ASin","[-1,1]",x,__FILE__,__LINE__);
#endif
        value = ::asin(x);
        firstDerivative = 1/::sqrt(1-(x*x));
        firstDerivative3 = firstDerivative * firstDerivative * firstDerivative;
        x_ = x;
      }

      /// Function value.
      double d0() const noexcept
      {
        return value;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx=1) const
      {
        return firstDerivative * dx;
      }

      /// Second (directional) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx=1, double dy=1) const
      {
        return x_ * firstDerivative3 * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1>
      double d3(double dx=1, double dy=1, double dz=1) const
      {
        return firstDerivative3 * ( 1 + ( 3 * x_ * x_ /(firstDerivative*firstDerivative) ) ) * dx * dy * dz;
      }

    private:
      double value = 0., firstDerivative = 1., firstDerivative3 = 1., x_ = 0.;
    };

    /**
     * \brief Plug f into arcsine.
     * \return object of type Chain<ASin,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto asin(const Function& f)
    {
      return ASin()(f);
    }
  }
}

#endif // RFFGEN_CMATH_ARCSINE_HH
