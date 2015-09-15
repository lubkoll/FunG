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

#ifndef FUNG_CMATH_EXP_HH
#define FUNG_CMATH_EXP_HH

#include <cmath>
#include "fung/util/base.hh"
#include "fung/util/chainer.hh"

namespace FunG
{
  namespace CMath
  {
    /**
     * \ingroup CMathGroup
     *
     * \brief Exponential function including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    struct Exp : Base , Chainer<Exp>
    {
      using Chainer<Exp>::operator ();
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Exp(double x=0.) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
        e_x = ::exp(x);
      }

      /// Function value.
      double d0() const noexcept
      {
        return e_x;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return e_x * dx;
      }

      /// Second (directinal) derivative
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return e_x * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return e_x * dx * dy * dz;
      }

    private:
      double e_x = 1.;
    };

    /**
     * \ingroup CMathGroup
     *
     * \brief Function \f$2^x\f$ including first three derivatives.
     *
     * For scalar functions directional derivatives are less interesting. Incorporating this function as building block for more complex functions requires directional derivatives. These occur
     * during applications of the chain rule.
     */
    struct Exp2 : Base , Chainer<Exp2>
    {
      using Chainer<Exp2>::operator ();
      /**
       * @brief Constructor.
       * @param x point of evaluation
       */
      explicit Exp2(double x=0.) { update(x); }

      /// Reset point of evaluation.
      void update(double x)
      {
        value = exp2(x);
      }

      /// Function value.
      double d0() const noexcept
      {
        return value;
      }

      /// First (directional) derivative.
      template < int = -1 >
      double d1(double dx = 1.) const
      {
        return value * ln2 * dx;
      }

      /// Second (directinal) derivative
      template < int = -1 , int = -1 >
      double d2(double dx = 1., double dy = 1.) const
      {
        return value * ln2 * ln2 * dx * dy;
      }

      /// Third (directional) derivative.
      template < int = -1 , int = -1 , int = -1 >
      double d3(double dx = 1., double dy = 1., double dz = 1.) const
      {
        return value * ln2 * ln2 * ln2 * dx * dy * dz;
      }

    private:
      double value = 1., ln2 = log(2.);
    };

    /**
     * \brief Plug f into exponential function.
     * \return object of type Chain<Exp,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto exp(const Function& f)
    {
      return Exp()(f);
    }

    /**
     * \brief Plug f into \f$2^x\f$.
     * \return object of type Chain<Exp2,Function>.
     */
    template <class Function, class = std::enable_if_t<std::is_base_of<Base,Function>::value> >
    auto ex2p(const Function& f)
    {
      return Exp2()(f);
    }
  }
}

#endif // FUNG_CMATH_EXP_HH
