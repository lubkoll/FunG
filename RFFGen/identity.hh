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

#ifndef RFFGEN_IDENTITY_HH
#define RFFGEN_IDENTITY_HH

#include "Util/base.hh"
#include "Util/chainer.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct ArithmeticConceptCheck;
  /**
   * \endcond
   */

  /// %Identity mapping \f$ f(x)=x \f$.
  template <class Arg, class = ArithmeticConceptCheck<Arg> >
  struct Identity : Base , Chainer< Identity<Arg,ArithmeticConceptCheck<Arg> > >
  {
    /// Default constructor.
    Identity() = default;

    /**
     * @brief Constructor.
     * @param x point of evaluation.
     */
    Identity(const Arg& x) { update(x); }

    /// Reset point of evaluation
    void update(const Arg& x)
    {
      x_ = x;
    }

    /// Function value.
    const Arg& d0() const noexcept { return x_; }

    /// First directional derivative.
    template <int>
    const Arg& d1(const Arg& dx) const noexcept { return dx; }

  private:
    Arg x_;
  };

  /// Construct Identity<Arg>(x).
  template <class Arg>
  auto identity(const Arg& x)
  {
    return Identity<Arg>(x);
  }
}

#endif // RFFGEN_IDENTITY_HH
