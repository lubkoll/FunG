// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_ARCSINE_HH
#define FUNG_CMATH_ARCSINE_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/exceptions.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /*!
    @ingroup CMathGroup

    @brief Arc sine function including first three derivatives (based on asin(double) in \<cmath\>).

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct ASin : Chainer<ASin>
  {
    //! @copydoc Cos::Cos()
    explicit ASin(double x=0.)
    {
      update(x);
    }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x < -1 || x > 1 ) throw OutOfDomainException("ASin","[-1,1]",x,__FILE__,__LINE__);
#endif
      value = ::asin(x);
      firstDerivative = 1/::sqrt(1-(x*x));
      firstDerivative3 = firstDerivative * firstDerivative * firstDerivative;
      x_ = x;
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! @copydoc Cos::d1()
    double d1(double dx=1) const
    {
      return firstDerivative * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx=1, double dy=1) const
    {
      return x_ * firstDerivative3 * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx=1, double dy=1, double dz=1) const
    {
      return firstDerivative3 * ( 1 + ( 3 * x_ * x_ /(firstDerivative*firstDerivative) ) ) * dx * dy * dz;
    }

  private:
    double value = 0., firstDerivative = 1., firstDerivative3 = 1., x_ = 0.;
  };

  /*!
    @ingroup CMathGroup
    @brief Generate \f$ \arcsin\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<ASin,Function>
   */
  template <class Function,
            class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto asin(const Function& f)
  {
    return ASin()(f);
  }
}

#endif // FUNG_CMATH_ARCSINE_HH
