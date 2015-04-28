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

#ifndef RFFGEN_UTIL_ARMADILLO_HH
#define RFFGEN_UTIL_ARMADILLO_HH

#include <armadillo>
#include "zero.hh"

namespace RFFGen
{
  /// Specialization for fixed size armadillo matrices. Provides a functor to get a fixed size zero armadillo matrix.
  template <unsigned n, unsigned m>
  struct Zero<arma::Mat<double>::fixed<n,m> >
  {
    /// Create zero matrix.
    arma::Mat<double>::fixed<n,m> operator()() const
    {
      arma::Mat<double>::fixed<n,m> mat;
      mat.zeros();
      return mat;
    }
  };
}

#endif // RFFGEN_UTIL_ARMADILLO_HH
