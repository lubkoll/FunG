// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CMATH_POW_HH
#define FUNG_CMATH_POW_HH

#include <cmath>
#include "fung/util/chainer.hh"
#include "fung/util/exceptions.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  /** @addtogroup CMathGroup
   *  @{ */

  /*!
    @brief Power function with rational exponent \f$ k = \frac{dividend}{divisor} \f$ including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
    For the cases \f$k=-1\f$ and \f$k=2\f$ specializations are used that avoid the use of std::pow.
   */
  template <int dividend, int divisor=1>
  struct Pow : Chainer< Pow<dividend,divisor> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double x=1) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( k < 3 &&  x == 0 ) throw OutOfDomainException("Pow<" + std::to_string(dividend) + "," + std::to_string(divisor) + ">" , "]-inf,inf[ \\ {0}",x,__FILE__,__LINE__);
#endif
      xk = x * (xk1 = x * (xk2 = x * ( xk3 = ::pow(x,k-3)) ) );
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return xk;
    }

    //! @copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return k * xk1 * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return k * (k-1) * xk2 * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return k * (k-1) * (k-2) * xk3 * dx * dy * dz;
    }

  private:
    const double k = static_cast<double>(dividend)/divisor;
    double xk = 0, xk1 = 0, xk2 = 0, xk3 = 0;
  };

  /// @cond
  template <>
  struct Pow<2,1> : Chainer< Pow<2,1> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double x_=0) { update(x_); }

    //! @copydoc Cos::update()
    void update(const double& x_)
    {
      x = 2*x_;
      x2 = x_*x_;
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return x2;
    }

    //! @copydoc Cos::d1()
    double d1(double dx=1.) const
    {
      return x * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx=1, double dy=1) const
    {
      return 2 * dx * dy;
    }

  private:
    double x = 0., x2 = 0.;
  };

  template <>
  struct Pow<3,1> : Chainer< Pow<3,1> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double x_=0) { update(x_); }

    //! @copydoc Cos::update()
    void update(double x_)
    {
      x = x_;
      x2 = x*x;
      x3 = x2*x;
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return x3;
    }

    //! @copydoc Cos::d1()
    double d1(double dx=1) const
    {
      return 3 * x2 * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx=1, double dy=1) const
    {
      return 6 * x * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx=1,double dy=1,double dz=1) const
    {
      return 6*dx*dy*dz;
    }

  private:
    double x = 0., x2 = 0., x3 = 0.;
  };

  /**
   * @brief Power function with integral exponent including first three derivatives. Specialization for k=-1, avoiding the use of std::pow.
   * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
  template <>
  struct Pow<-1,1> : Chainer< Pow<-1,1> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double x=1.) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x == 0 ) throw OutOfDomainException("Pow<-1,1>","]-inf,inf[ \\ {0}",x,__FILE__,__LINE__);
#endif
      x_inv = 1. / x;
      x_inv2 = x_inv*x_inv;
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return x_inv;
    }

    //! @copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return -1 * x_inv2 * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return 2 * x_inv2 * x_inv * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return -6 * x_inv2 * x_inv2 * dx * dy * dz;
    }

  private:
    double x_inv = 1., x_inv2 = 1.;
  };

  template <>
  struct Pow<1,2> : Chainer< Pow<1,2> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double x=0) { update(x); }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x < 0 ) throw OutOfDomainException("Pow<1,2>","[0,inf[",x,__FILE__,__LINE__);
#endif
      x_ = x;
      sqrt_x = ::sqrt(x);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept
    {
      return sqrt_x;
    }

    //! @copydoc Cos::d1()
    double d1(double dx = 1.) const
    {
      return 0.5 / sqrt_x * dx;
    }

    //! @copydoc Cos::d2()
    double d2(double dx = 1., double dy = 1.) const
    {
      return -0.25 / (x_*sqrt_x) * dx * dy;
    }

    //! @copydoc Cos::d3()
    double d3(double dx = 1., double dy = 1., double dz = 1.) const
    {
      return 0.375 / (x_*x_*sqrt_x) * dx * dy * dz;
    }

  private:
    double x_ = 0., sqrt_x = 1.;
  };

  /// The function \f$ t\mapsto t^{-1/3} \f$ with first three derivatives.
  template <>
  struct Pow<-1,3> : Chainer< Pow<-1,3> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double t=1) { update(t); }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x < 0 ) throw OutOfDomainException("Pow<1,3>","[0,inf[",x,__FILE__,__LINE__);
#endif
      auto p = cbrt(x);
      d0val =  1/p;
      p *= x;
      d1val = -1/(3*p);
      p *= x;
      d2val =  4/(9*p);
      p *= x;
      d3val = -28/(27*p);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept { return d0val; }

    //! @copydoc Cos::d1()
    double d1(double dt=1) const { return d1val*dt; }

    //! @copydoc Cos::d2()
    double d2(double dt0=1, double dt1=1) const { return d2val*dt0*dt1; }

    //! @copydoc Cos::d3()
    double d3(double dt0=1, double dt1=1, double dt2=1) const { return d3val*dt0*dt1*dt2; }

  private:
    double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
  };


  /// The function \f$ t\mapsto t^{2/3} \f$ with first three derivatives.
  template <>
  struct Pow<-2,3> : Chainer< Pow<-2,3> >
  {
    //! @copydoc Cos::Cos()
    explicit Pow(double t=1.)  { update(t); }

    //! @copydoc Cos::update()
    void update(double x)
    {
#ifdef FUNG_ENABLE_EXCEPTIONS
      if( x < 0 ) throw OutOfDomainException("Pow<2,3>","[0,inf[",x,__FILE__,__LINE__);
#endif
      auto p0 = cbrt(x);
      auto p = p0*p0;
      d0val =   1/p;
      p *= x;
      d1val =  -2/(3*p);
      p *= x;
      d2val =   10/(9*p);
      p *= x;
      d3val = -80/(27*p);
    }

    //! @copydoc Cos::d0()
    double d0() const noexcept { return d0val; }

    //! @copydoc Cos::d1()
    double d1(double dt=1) const { return d1val*dt; }

    //! @copydoc Cos::d2()
    double d2(double dt0=1, double dt1=1) const { return d2val*dt0*dt1; }

    //! @copydoc Cos::d3()
    double d3(double dt0=1, double dt1=1, double dt2=1) const { return d3val*dt0*dt1*dt2; }

  private:
    double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
  };
  /// @endcond

  /// Square root including first three derivatives (based on sqrt(double) in \<cmath\>).
  using Sqrt = Pow<1,2>;

  /// Third root including first three derivatives (based on sqrt(double) in \<cmath\>).
  using Cbrt = Pow<1,3>;

  /// Third root squared including first three derivatives (based on sqrt(double) in \<cmath\>).
  using Cbrt2 = Pow<2,3>;

  /*!
    @brief Generate \f$ \sqrt{f} \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Sqrt,Function>
   */
  template <class Function, class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto sqrt(const Function& f)
  {
    return Sqrt()(f);
  }

  /*!
    @brief Generate \f$ \sqrt[3]{f} \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Cbrt,Function>
   */
  template <class Function, class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto cbrt(const Function& f)
  {
    return Cbrt()(f);
  }

  /*!
    @brief Generate \f$ \sqrt[3]{f^2}\f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Cbrt2,Function>
   */
  template <class Function, class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto cbrt2(const Function& f)
  {
    return Cbrt2()(f);
  }

  /*!
    @brief Generate \f$ f^{k/l} \f$.
    @param f function mapping into a scalar space
    @tparam k dividend
    @tparam l divisor
    @return object of type MathematicalOperations::Chain< Pow<dividend,divisor> , Function >
   */
  template <int k, int l,
            class Function,
            class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto pow(const Function& f)
  {
    return Pow<k,l>()(f);
  }

  /*!
    @brief Generate \f$ f^k,\ k\in\mathbb{N}\f$.
    @param f function mapping into a scalar space
    @tparam k exponent
    @return object of type MathematicalOperations::Chain< Pow<dividend,divisor> , Function >
   */
  template <int k,
            class Function,
            class = std::enable_if_t<Checks::isFunction<Function>()> >
  auto pow(const Function& f)
  {
    return Pow<k>()(f);
  }
  /** @} */
}

#endif // FUNG_CMATH_POW_HH
