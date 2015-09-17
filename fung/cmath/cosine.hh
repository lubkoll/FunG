/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_CMATH_COSINE_HH
#define FUNG_CMATH_COSINE_HH

#include <cmath>
#include "fung/util/base.hh"
#include "fung/util/chainer.hh"

namespace FunG
{
  /**
   * \ingroup CMathGroup
   *
   * \brief Cosine function including first three derivatives (based on cos(double) in \<cmath\>).
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   *
   * \see cosine
   */
  struct Cos : Base , Chainer<Cos>
  {
    using Chainer<Cos>::operator ();
    /**
     * @brief Constructor.
     * @param x point of evaluation
     */
    explicit Cos(double x=0.)
    {
      update(x);
    }

    /// Reset point of evaluation.
    void update(const double& x)
    {
      sinx = ::sin(x);
      cosx = ::cos(x);
    }

    /// Function value.
    double d0() const noexcept { return cosx; }

    /// First (directional) derivative.
    template < int = -1 >
    double d1(double dx=1.) const { return -sinx*dx; }

    /// Second (directional) derivative.
    template < int = -1 , int = -1 >
    double d2(double dx=1., double dy=1.) const { return -cosx*dx*dy; }

    /// Third (directional) derivative.
    template < int = -1, int = -1 , int = -1 >
    double d3(double dx=1., double dy=1., double dz=1.) const { return sinx*dx*dy*dz; }

  private:
    double sinx, cosx;
  };

  /**
   * \brief Generate \f$ \cos\circ f \f$.
   * \param f function mapping into a scalar space
   */
  template < class F ,
             class = std::enable_if_t< std::is_base_of<Base,F>::value > >
  auto cos(const F& f)
  {
    return Cos()(f);
  }
}

#endif // FUNG_CMATH_COSINE_HH
