// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_SINE_HH
#define FUNG_CMATH_SINE_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /*!
    @ingroup CMathGroup

    @brief Sine function including first three derivatives (based on sin(double) in \<cmath\>).

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct Sin : Chainer<Sin>
  {
    //! @copydoc Cos::Cos()
    explicit Sin(double x=0)
    {
      update(x);
    }

    //! @copydoc Cos::update()
    void update(double x)
    {
      sinx = ::sin(x);
      cosx = ::cos(x);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept { return sinx; }

    //! @copydoc Cos::d1()
    double d1(double dx=1.) const { return cosx*dx; }

    //! @copydoc Cos::d2()
    double d2(double dx=1., double dy=1.) const { return -sinx*dx*dy; }

    //! @copydoc Cos::d3()
    double d3(double dx=1., double dy=1., double dz=1.) const { return -cosx*dx*dy*dz; }

  private:
    double sinx = 0, cosx = 1;
  };

  /*!
    @ingroup CMathGroup
    @brief Generate \f$ \sin\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Sin,Function>
   */
  template <class Function,
            class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto sin(const Function& f)
  {
    return Sin()(f);
  }
}

#endif // FUNG_CMATH_SINE_HH
