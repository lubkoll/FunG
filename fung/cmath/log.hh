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

#ifndef FUNG_CMATH_LOG_HH
#define FUNG_CMATH_LOG_HH

#include <cmath>
#include "fung/util/base.hh"
#include "fung/util/chainer.hh"
#include "fung/util/exceptions.hh"

namespace FunG
{
  /**
   * \ingroup CMathGroup
   *
   * \brief Natural logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
  struct LN : Base , Chainer<LN>
  {
    //! \copydoc Cos::Cos()
    explicit LN(double x=1.) { update(x); }

    //! \copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x <= 0 ) throw OutOfDomainException("LN","]0,inf[",x,__FILE__,__LINE__);
#endif
      x_inv = 1./x;
      value = ::log(x);
    }

    //! \copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! \copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return x_inv * dx;
    }

    //! \copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return - x_inv * x_inv * dx * dy;
    }

    //! \copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return 2 * x_inv * x_inv * x_inv * dx * dy * dz;
    }

  private:
    double value = 0., x_inv = 1.;
  };

  /**
   * \ingroup CMathGroup
   *
   * \brief Common (base 10) logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
  struct Log10 : Base , Chainer<Log10>
  {
    //! \copydoc Cos::Cos()
    explicit Log10(double x=1.) { update(x); }

    //! \copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x <= 0 ) throw OutOfDomainException("Log10","]0,inf[",x,__FILE__,__LINE__);
#endif
      x_inv = 1./x;
      value = ::log10(x);
    }

    //! \copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! \copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return ln10inv * x_inv * dx;
    }

    //! \copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return - ln10inv * x_inv * x_inv * dx * dy;
    }

    //! \copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return 2 * ln10inv * x_inv * x_inv * x_inv * dx * dy * dz;
    }

  private:
    double value = 0., x_inv = 1., ln10inv = 1/log(10.);
  };

  /**
   * \ingroup CMathGroup
   *
   * \brief %Base 2 logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
  struct Log2 : Base , Chainer<Log2>
  {
    //! \copydoc Cos::Cos()
    explicit Log2(double x=1.) { update(x); }

    //! \copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x <= 0 ) throw OutOfDomainException("Log2","]0,inf[",x,__FILE__,__LINE__);
#endif
      x_inv = 1./x;
      value = ::log2(x);
    }

    //! \copydoc Cos::d0()
    double d0() const noexcept
    {
      return value;
    }

    //! \copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return ln2inv * x_inv * dx;
    }

    //! \copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return - ln2inv * x_inv * x_inv * dx * dy;
    }

    //! \copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return 2 * ln2inv * x_inv * x_inv * x_inv * dx * dy * dz;
    }

  private:
    double value = 0., x_inv = 1., ln2inv = 1/log(2.);
  };

  /*!
    \ingroup CMathGroup
    \brief Generate \f$ \mathrm{ln}\circ f \f$.
    \param f function mapping into a scalar space
    \return object of type MathematicalOperations::Chain<Log,Function>
   */
  template <class Function,
            class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
  auto ln(const Function& f)
  {
    return LN()(f);
  }

  /*!
    \ingroup CMathGroup
    \brief Generate \f$ \mathrm{log}_{10}\circ f \f$.
    \param f function mapping into a scalar space
    \return object of type MathematicalOperations::Chain<Log10,Function>
   */
  template <class Function,
            class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
  auto log10(const Function& f)
  {
    return Log10()(f);
  }

  /*!
    \ingroup CMathGroup
    \brief Generate \f$ \mathrm{log}_{2}\circ f \f$.
    \param f function mapping into a scalar space
    \return object of type MathematicalOperations::Chain<Log2,Function>
   */
  template <class Function,
            class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
  auto log2(const Function& f)
  {
    return Log2()(f);
  }
}

#endif // FUNG_CMATH_LOG_HH
