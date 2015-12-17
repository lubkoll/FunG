// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_COSINE_HH
#define FUNG_CMATH_COSINE_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /*!
    @ingroup CMathGroup

    @brief Cosine function including first three derivatives (based on cos(double) in \<cmath\>).

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
  struct Cos : Chainer<Cos>
  {
    /**
     * @brief Constructor.
     * @param x point of evaluation
     */
    explicit Cos(double x=0.)
    {
      update(x);
    }

    /// Set point of evaluation.
    void update(const double& x)
    {
      sinx = ::sin(x);
      cosx = ::cos(x);
    }

    /// Function value.
    double d0() const noexcept { return cosx; }

    /// First (directional) derivative.
    double d1(double dx=1.) const { return -sinx*dx; }

    /// Second (directional) derivative.
    double d2(double dx=1., double dy=1.) const { return -cosx*dx*dy; }

    /// Third (directional) derivative.
    double d3(double dx=1., double dy=1., double dz=1.) const { return sinx*dx*dy*dz; }

  private:
    double sinx = 0, cosx = 1;
  };

  /*!
    @ingroup CMathGroup
    @brief Generate \f$ \cos\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Cos,Function>
   */
  template < class Function ,
             class = std::enable_if_t< Checks::isFunction<Function>() > >
  auto cos(const Function& f)
  {
    return Cos()(f);
  }
}

#endif // FUNG_CMATH_COSINE_HH
