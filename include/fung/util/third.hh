// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_THIRD_HH
#define FUNG_THIRD_HH

#define HAVE_BOOST

#ifdef HAVE_BOOST
#include <boost/math/constants/constants.hpp>
#endif

/// @cond
namespace FunG
{
#ifdef HAVE_BOOST
  constexpr double third() noexcept { return boost::math::constants::third<double>(); }
  constexpr double twothirds() noexcept { return boost::math::constants::twothirds<double>(); }
#else
  constexpr double third() noexcept { return 1./3.; }
  constexpr double twothirds() noexcept { return 2*third(); }
#endif
}
/// @endcond

#endif // FUNG_THIRD_HH
