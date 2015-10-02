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

#ifndef FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH
#define FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH

#include <type_traits>
#include <utility>

#include "fung/util/base.hh"
#include "fung/util/derivative_wrappers.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct Chainer;
  namespace Concepts{ template <class> struct FunctionConceptCheck; }
  /**
   * \endcond
   */

  namespace MathematicalOperations
  {
    /**
     * \ingroup MathematicalOperationsGroup
     *
     * \brief Scaling \f$ af \f$ of some function \f$ f \f$ with a double \f$ a \f$ (F must satisfy the requirements of Concepts::FunctionConcept).
     */
    template <class F, class = Concepts::FunctionConceptCheck<F> >
    struct Scale : Base , Chainer< Scale< F , Concepts::FunctionConceptCheck<F> > >
    {
      /**
       * \brief Constructor passing arguments to function constructor.
       * \param a_ scaling
       * \param f_ input for constructor of outer function
       */
      template <class InitF>
      Scale(double a_, const InitF& f_)
        : a(a_), f(f_), value(a * f.d0())
      {}

      /// Reset point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        update_if_present(f,x);
        value = a * f.d0();
      }

      /// Propagate call to updateVariable() to f.
      template <int index, class Arg>
      void updateVariable(const Arg& x)
      {
        f.template updateVariable<index>(x);
      }

      /// Function value.
      const auto& d0() const noexcept
      {
        return value;
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
        template < int idx , class Arg ,
                 class IndexedArg = IndexedType<Arg,idx> ,
                 class = std::enable_if_t< D1<F,IndexedArg>::present> >
      auto d1(Arg const& dx) const
      {
        return a * D1<F,IndexedArg>(f,dx)();//f.template d1<idx>(dx);
      }

      /**
       * \brief Second directional derivative.
       * \param dx direction for which the derivative is computed
       * \param dy direction for which the derivative is computed
       */
        template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class = std::enable_if_t< D2<F,IndexedArgX,IndexedArgY>::present> >
      auto d2(ArgX const& dx, ArgY const& dy) const
      {
        return a * D2<F,IndexedArgX,IndexedArgY>(f,dx,dy)();//f.template d2<idx,idy>(dx,dy);
      }

      /**
       * \brief Third directional derivative.
       * \param dx direction for which the derivative is computed
       * \param dy direction for which the derivative is computed
       * \param dz direction for which the derivative is computed
       */
      template < int idx , int idy , int idz , class ArgX , class ArgY , class ArgZ ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class IndexedArgZ = IndexedType<ArgZ,idz> ,
                 class = std::enable_if_t< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>::present > >
      auto d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        return a * D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz)();//f.template d3<idx,idy,idz>(dx,dy,dz);
      }

    private:
      double a = 1.;
      F f;
      std::decay_t<decltype(std::declval<F>().d0())> value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH
