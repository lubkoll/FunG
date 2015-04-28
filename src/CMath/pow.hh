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
#include "../Util/third.hh"

namespace RFFGen
{
  namespace CMath
  {
    /**
     * \ingroup CMathGroup
     *
     * \brief Power function with integral exponent including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     * For the cases \f$k=-1\f$ and \f$k=2\f$ specializations are used that avoid the use of std::pow.
     */
    template <int k>
    struct Pow : Base
    {
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Pow(double x=1.) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
        assert( k-3 >= 0 || x != 0);
        xk = x * (xk1 = x * (xk2 = x * ( xk3 = pow(x,k-3)) ) );
      }

      /// Function value. Convenient access to d0().
      double operator()() const noexcept
      {
        return d0();
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
      double xk, xk1, xk2, xk3;
    };

    /**
     * \ingroup STLMATH
     * \return Default initialized object of type Pow<k>.
     */
    template <int k>
    auto power()
    {
      return Pow<k>();
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
    struct Pow<2> : Base
    {
      explicit Pow(double x_=0.) { update(x_); }

      void update(double x_)
      {
        x = x_;
        x2 = x*x;
      }

      double operator()() const noexcept
      {
        return d0();
      }

      /// Function value.
      double  d0() const noexcept
      {
        return x2;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double  d1(double dx) const
      {
        return 2 * x * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double  d2(double dx, double dy) const
      {
        return 2 * dx * dy;
      }

    private:
      double x = 0., x2 = 0.;
    };

    template <>
    struct Pow<3> : Base
    {
      explicit Pow(double x_=0.) { update(x_); }

      void update(double x_)
      {
        x = x_;
        x2 = x*x;
        x3 = x2*x;
      }

      double operator()() const noexcept
      {
        return d0();
      }

      /// Function value.
      double d0() const noexcept
      {
        return x3;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx) const
      {
        return 3 * x2 * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double d2(double dx, double dy) const
      {
        return 6 * x * dx * dy;
      }

      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx,double dy,double dz) const
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
    struct Pow<-1> : Base
    {
      explicit Pow(double x=1.) { update(x); }

      void update(double x)
      {
        assert(x!=0);
        x_inv = 1. / x;
        x_inv2 = x_inv*x_inv;
      }

      double operator()() const noexcept
      {
        return d0();
      }

      /// Function value.
      double d0() const noexcept
      {
        return x_inv;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double  d1(double dx = 1.) const
      {
        return -1 * x_inv2 * dx;
      }

      /// Second (directinal) derivative.
      template < int = -1 , int = -1 >
      double  d2(double dx = 1., double dy = 1.) const
      {
        return 2 * x_inv2 * x_inv * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double  d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return -6 * x_inv2 * x_inv2 * dx * dy * dz;
      }

    private:
      double x_inv = 1., x_inv2 = 1.;
    };
    /**
     * \endcond
     */

    /**
     * \ingroup CMathGroup
     * @brief The function \f$ t\mapsto t^{1/3} \f$ with first three derivatives.
     */
    struct OverThirdRoot : Base
    {
      /**
       * @brief Constructor.
       * @param t point of evaluation
       */
      explicit OverThirdRoot(double t=1.) { update(t); }

      /// Reset point of evaluation.
      void update(double t)
      {
        double  p = pow(t,third);
        d0val =  1/p;
        p *= t;
        d1val = -1/(3*p);
        p *= t;
        d2val =  4/(9*p);
        p *= t;
        d3val = -28/(27*p);
      }

      /// Function value. Convenient access to d0().
      double operator()() const noexcept { return d0(); }

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
      double third = RFFGen::third();
      double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
    };


    /**
     * \ingroup CMathGroup
     * @brief The function \f$ t\mapsto t^{2/3} \f$ with first three derivatives.
     */
    struct OverThirdRootSquared : Base
    {
      /**
       * @brief Constructor.
       * @param t point of evaluation
       */
      explicit OverThirdRootSquared(double t=1.)  { update(t); }

      /// Reset point of evaluation.
      void update(double t)
      {
        double  p = pow(t,twothird);
        d0val =   1/p;
        p *= t;
        d1val =  -2/(3*p);
        p *= t;
        d2val =   10/(9*p);
        p *= t;
        d3val = -80/(27*p);
      }

      /// Function value. Convenient access to d0().
      double operator()() const noexcept { return d0(); }

      /// Function value. Convenient access to d0().
      double d0() const noexcept { return d0val; }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dt) const { return d1val*dt; }

      /// Second (directional) derivative.
      template < int = -1 , int = -1 >
      double d2(double dt0, double dt1) const { return d2val*dt0*dt1; }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dt0, double dt1, double dt2) const { return d3val*dt0*dt1*dt2; }

    private:
      double twothird = RFFGen::twothirds();
      double d0val = 0, d1val = 0, d2val = 0, d3val = 0;
    };
  }
}

#endif // RFFGEN_CMATH_POW_HH
