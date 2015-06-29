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

#ifndef RFFGEN_CONSTANT_HH
#define RFFGEN_CONSTANT_HH

#include "Util/base.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct ArithmeticConceptCheck;
  /**
   * \endcond
   */

  /**
   * \brief Wrap a constant.
   */
  template <class Type, class = ArithmeticConceptCheck<Type> >
  struct Constant : Base
  {
    Constant() = default;

   /// Construct constant from copy.
   Constant(Type const& t_) : t(t_)
   {}

   /// Function value. Convenient access to d0().
   const Type& operator()() const noexcept
   {
     return d0();
   }

   /// Function value.
   const Type& d0() const noexcept
   {
     return t;
   }

  private:
    Type t;
  };

  /**
   * \brief Generate a constant function that stores its argument as constant reference.
   *
   * This admits to use variable constant arguments, i.e. parameters that we want to study.
   */
  template <class Arg>
  auto constRef(const Arg& x)
  {
    return Constant<const Arg&>(x);
  }

  /**
   * \brief Wrap a constant.
   * \return Constant<Arg>(x)
   */
  template <class Arg>
  auto constant(const Arg& x)
  {
    return Constant<Arg>(x);
  }
}

#endif // RFFGEN_CONSTANT_HH

