// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_IDENTITY_HH
#define FUNG_IDENTITY_HH

#include "fung/util/chainer.hh"

namespace FunG
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
  struct Identity : Chainer< Identity<Arg,ArithmeticConceptCheck<Arg> > >
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

#endif // FUNG_IDENTITY_HH
