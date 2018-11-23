// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_EXP_HH
#define FUNG_CMATH_EXP_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /** @addtogroup CMathGroup
   *  @{ */

  /*!
    @brief Exponential function including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct Exp : Chainer<Exp>
  {
    //! @copydoc Cos::d0()
    explicit Exp(double x=0.) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
      e_x = ::exp(x);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return e_x;
    }

    //! @copydoc Cos::d0()
    double d1(double dx = 1.) const
    {
      return e_x * dx;
    }

    //! @copydoc Cos::d0()
    double d2(double dx = 1., double dy = 1.) const
    {
      return e_x * dx * dy;
    }

    //! @copydoc Cos::d0()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return e_x * dx * dy * dz;
    }

  private:
    double e_x = 1.;
  };

  /*!
    @brief Function \f$2^x\f$ including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct Exp2 : Chainer<Exp2>
  {
    //! @copydoc Cos::Cos()
    explicit Exp2(double x=0.) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
      value = exp2(x);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! @copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return value * ln2 * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return value * ln2 * ln2 * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return value * ln2 * ln2 * ln2 * dx * dy * dz;
    }

  private:
    double value = 1., ln2 = log(2.);
  };

  /*!
    @brief Generate \f$ \exp(f) \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Exp,Function>
   */
  template <class Function, class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto exp(const Function& f)
  {
    return Exp()(f);
  }

  /*!
    @brief Generate \f$2^f\f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Exp2,Function>
   */
  template <class Function, class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto exp2(const Function& f)
  {
    return Exp2()(f);
  }
  /** @} */
}

#endif // FUNG_CMATH_EXP_HH
