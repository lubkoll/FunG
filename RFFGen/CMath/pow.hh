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

#ifndef RFFGEN_CMATH_POW_HH
#define RFFGEN_CMATH_POW_HH

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
     * \brief Power function with rational exponent \f$ k = \frac{dividend}{divisor} \f$ including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     * For the cases \f$k=-1\f$ and \f$k=2\f$ specializations are used that avoid the use of std::pow.
     */
    template <int dividend, int divisor=1>
    struct Pow : Base , Chainer< Pow<dividend,divisor> >
    {
      using Chainer< Pow<dividend,divisor> >::operator ();

      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Pow(double x=1) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
        if( k < 3 &&  x == 0 ) throw OutOfDomainException("Pow<" + std::to_string(dividend) + "," + std::to_string(divisor) + ">", "]-inf,inf[\{0}",x,__FILE__,__LINE__);
#endif
        xk = x * (xk1 = x * (xk2 = x * ( xk3 = ::pow(x,k-3)) ) );
      }

      /// Function value.
      double d0() const noexcept
      {
        return xk;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return k * xk1 * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return k * (k-1) * xk2 * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return k * (k-1) * (k-2) * xk3 * dx * dy * dz;
      }

    private:
      const double k = static_cast<double>(dividend)/divisor;
      double xk, xk1, xk2, xk3;
    };

    /**
     * \ingroup CMathGroup
     * \return Default initialized object of type Pow<k>.
     */
    template <int dividend, int divisor=1>
    auto power()
    {
      return Pow<dividend,divisor>();
    }
    /**
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */

    /**
     * \brief Power function with integral exponent including first three derivatives. Specialization for quadratic functions, avoiding the use of std::pow.
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    template <>
    struct Pow<2,1> : Base, Chainer< Pow<2,1> >
    {
      using Chainer< Pow<2,1> >::operator ();

      explicit Pow(double x_=0) { update(x_); }

      void update(const double& x_)
      {
        x = 2*x_;
        x2 = x_*x_;
      }

      /// Function value.
      double d0() const noexcept
      {
        return x2;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx=1.) const
      {
        return x * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx=1, double dy=1) const
      {
        return 2 * dx * dy;
      }

    private:
      double x = 0., x2 = 0.;
    };

    template <>
    struct Pow<3,1> : Base , Chainer< Pow<3,1> >
    {
      using Chainer< Pow<3,1> >::operator ();

      explicit Pow(double x_=0) { update(x_); }

      void update(double x_)
      {
        x = x_;
        x2 = x*x;
        x3 = x2*x;
      }

      /// Function value.
      double d0() const noexcept
      {
        return x3;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx=1) const
      {
        return 3 * x2 * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx=1, double dy=1) const
      {
        return 6 * x * dx * dy;
      }

      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx=1,double dy=1,double dz=1) const
      {
        return 6*dx*dy*dz;
      }

    private:
      double x = 0., x2 = 0., x3 = 0.;
    };

    /**
     * \brief Power function with integral exponent including first three derivatives. Specialization for k=-1, avoiding the use of std::pow.
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    template <>
    struct Pow<-1,1> : Base , Chainer< Pow<-1,1> >
    {
      using Chainer< Pow<-1,1> >::operator ();

      explicit Pow(double x=1.) { update(x); }

      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
        if( x == 0 ) throw OutOfDomainException("Pow<-1,1>","]-inf,inf[\{0}",x,__FILE__,__LINE__);
#endif
        x_inv = 1. / x;
        x_inv2 = x_inv*x_inv;
      }

      /// Function value.
      double d0() const noexcept
      {
        return x_inv;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return -1 * x_inv2 * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return 2 * x_inv2 * x_inv * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return -6 * x_inv2 * x_inv2 * dx * dy * dz;
      }

    private:
      double x_inv = 1., x_inv2 = 1.;
    };

    template <>
    struct Pow<1,2> : Base, Chainer< Pow<1,2> >
    {
      using Chainer< Pow<1,2> >::operator ();

      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Pow(double x=0) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
        if( x < 0 ) throw OutOfDomainException("Pow<1,2>","[0,inf[",x,__FILE__,__LINE__);
#endif
        x_ = x;
        sqrt_x = ::sqrt(x);
      }

      /// Function value.
      double d0() const noexcept
      {
        return sqrt_x;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return 0.5 / sqrt_x * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return -0.25 / (x_*sqrt_x) * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return 0.375 / (x_*x_*sqrt_x) * dx * dy * dz;
      }

    private:
      double x_ = 0., sqrt_x = 1.;
    };

    /**
     * \ingroup CMathGroup
     * @brief The function \f$ t\mapsto t^{-1/3} \f$ with first three derivatives.
     */
    template <>
    struct Pow<-1,3> : Base , Chainer< Pow<-1,3> >
    {
      using Chainer< Pow<-1,3> >::operator ();

      /**
       * @brief Constructor.
       * @param t point of evaluation
       */
      explicit Pow(double t=1) { update(t); }

      /// Reset point of evaluation.
      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
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

      /// Function value.
      double d0() const noexcept { return d0val; }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dt=1) const { return d1val*dt; }

      /// Second (directional) derivative.
      template < int = -1 , int = -1 >
      double d2(double dt0=1, double dt1=1) const { return d2val*dt0*dt1; }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dt0=1, double dt1=1, double dt2=1) const { return d3val*dt0*dt1*dt2; }

    private:
      double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
    };


    /**
     * \ingroup CMathGroup
     * @brief The function \f$ t\mapsto t^{2/3} \f$ with first three derivatives.
     */
    template <>
    struct Pow<-2,3> : Base , Chainer< Pow<-2,3> >
    {
      using Chainer< Pow<-2,3> >::operator ();

      /**
       * @brief Constructor.
       * @param t point of evaluation
       */
      explicit Pow(double t=1.)  { update(t); }

      /// Reset point of evaluation.
      void update(double x)
      {
#ifdef RFFGEN_ENABLE_DYNAMIC_CHECKS
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

      /// Function value. Convenient access to d0().
      double d0() const noexcept { return d0val; }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dt=1) const { return d1val*dt; }

      /// Second (directional) derivative.
      template < int = -1 , int = -1 >
      double d2(double dt0=1, double dt1=1) const { return d2val*dt0*dt1; }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dt0=1, double dt1=1, double dt2=1) const { return d3val*dt0*dt1*dt2; }

    private:
      double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
    };
    /**
     * \endcond
     */

    /**
     * \ingroup CMathGroup
     * \brief Square root including first three derivatives (based on sqrt(double) in \<cmath\>).
     */
    using Sqrt = Pow<1,2>;

    /**
     * \ingroup CMathGroup
     * \brief Third root including first three derivatives (based on sqrt(double) in \<cmath\>).
     */
    using Cbrt = Pow<1,3>;

    /**
     * \ingroup CMathGroup
     * \brief Third root squared including first three derivatives (based on sqrt(double) in \<cmath\>).
     */
    using Cbrt2 = Pow<2,3>;

    /**
     * \brief Plug f into square root function.
     * \return object of type Chain<Sqrt,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto sqrt(const Function& f)
    {
      return Sqrt()(f);
    }

    /**
     * \brief Plug f into third root function.
     * \return object of type Chain<Cbrt,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto cbrt(const Function& f)
    {
      return Cbrt()(f);
    }

    /**
     * \brief Plug f into third root squared function.
     * \return object of type Chain<Cbrt2,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto cbrt2(const Function& f)
    {
      return Cbrt2()(f);
    }

    /**
     * \brief Plug f into pow function.
     * \return object of type Chain< Pow<dividend,divisor> , Function >.
     */
    template <int dividend, int divisor,
              class Function,
              class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto pow(const Function& f)
    {
      return Pow<dividend,divisor>()(f);
    }
  }
}

#endif // RFFGEN_CMATH_POW_HH
