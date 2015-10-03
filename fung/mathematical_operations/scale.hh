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
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;
      /**
       * \brief Constructor passing arguments to function constructor.
       * \param a_ scaling
       * \param f_ input for constructor of outer function
       */
      template <class InitF>
      Scale(double a_, const InitF& f_)
        : a(a_), f(f_)
      {}

      /// Update point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        update_if_present(f,x);
      }

      /// Update variable corresponding to index.
      template <int index, class Arg>
      void update(const Arg& x)
      {
        update_if_present<index>(f,x);
      }

      /// Function value.
      ReturnType d0() const noexcept
      {
        return a*f.d0();
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
        template < int idx , class Arg ,
                 class IndexedArg = IndexedType<Arg,idx> ,
                 class = std::enable_if_t< D1<F,IndexedArg>::present> >
      ReturnType d1(Arg const& dx) const
      {
        return a * D1<F,IndexedArg>(f,dx)();
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
      ReturnType d2(ArgX const& dx, ArgY const& dy) const
      {
        return a * D2<F,IndexedArgX,IndexedArgY>(f,dx,dy)();
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
      ReturnType d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        return a * D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz)();
      }

    private:
      double a = 1.;
      F f;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH
