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

#ifndef FUNG_UTIL_CHAINER_HH
#define FUNG_UTIL_CHAINER_HH

#include <type_traits>
#include "base.hh"
#include "FunG/MathematicalOperations/chain.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  template <class Function>
  struct Chainer
  {
    decltype(auto) operator()() const noexcept
    {
      return static_cast<const Function*>(this)->d0();
    }

    template < class Arg ,
               class = std::enable_if_t< !std::is_base_of<Base,Arg>::value > >
    decltype(auto) operator()(const Arg& x)
    {
      static_cast<Function*>(this)->update(x);
      return static_cast<const Function*>(this)->d0();
    }

    template < class OtherFunction ,
               class = std::enable_if_t< std::is_base_of<Base,OtherFunction>::value > >
    auto operator()(const OtherFunction& g)
    {
      return MathematicalOperations::Chain<Function,OtherFunction>(*static_cast<const Function*>(this),g);
    }
  };
  /**
   * \endcond
   */
}
#endif // FUNG_UTIL_CHAINER_HH
