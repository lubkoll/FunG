// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_TAN_HH
#define FUNG_CMATH_TAN_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /** @addtogroup CMathGroup
   *  @{ */

  /*!
    @brief Tangent function including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct Tan : Chainer<Tan>
  {
    //! @copydoc Cos::Cos()
    explicit Tan(double x=0.) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
      value = ::tan(x);
      firstDerivative = 1 + ( value * value );
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! @copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return firstDerivative * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return ( 2 * value * firstDerivative ) * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return 2 * firstDerivative * ( 1 + ( 3 * value * value ) ) * dx * dy * dz;
    }

  private:
    double value = 0., firstDerivative = 1.;
  };

  /*!
    @brief Generate \f$ \tan\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Tan,Function>
   */
  template <class Function,
            class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto tan(const Function& f)
  {
    return Tan()(f);
  }
  /** @} */
}

#endif // FUNG_CMATH_TAN_HH
