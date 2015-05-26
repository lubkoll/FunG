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

#ifndef RFFGEN_THIRD_HH
#define RFFGEN_THIRD_HH

#define HAVE_BOOST

#ifdef HAVE_BOOST
#include <boost/math/constants/constants.hpp>
#endif

/**
 * \cond DOCUMENT_IMPLEMENTATION_DETAILS
 */
namespace RFFGen
{
#ifdef HAVE_BOOST
  constexpr double third() noexcept { return boost::math::constants::third<double>(); }
  constexpr double twothirds() noexcept { return boost::math::constants::twothirds<double>(); }
#else
  constexpr double third() noexcept { return 1./3.; }
  constexpr double twothirds() noexcept { return 2*third(); }
#endif
}
/**
 * \endcond
 */

#endif // RFFGEN_THIRD_HH
