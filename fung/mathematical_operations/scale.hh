// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH
#define FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH

#include <type_traits>
#include <utility>

#include "fung/util/derivative_wrappers.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"

namespace FunG
{
  /// @cond
  template <class> struct Chainer;
  namespace Concepts{ template <class> struct FunctionConceptCheck; }
  /// @endcond

  namespace MathematicalOperations
  {
    /**
     * @ingroup MathematicalOperationsGroup
     * @brief Scaling \f$ af \f$ of some function \f$ f \f$ with a double \f$ a \f$ (F must satisfy the requirements of Concepts::FunctionConcept).
     */
    template <class F, class = Concepts::FunctionConceptCheck<F> >
    struct Scale : Chainer< Scale< F , Concepts::FunctionConceptCheck<F> > >
    {
      /**
       * @brief Constructor passing arguments to function constructor.
       * @param a_ scaling
       * @param f_ input for constructor of outer function
       */
      template <class... InitF>
      Scale(double a_, InitF&&... f_)
        : a(a_), f(std::forward<InitF>(f_)...), value(a*f())
      {}

      /// Update point of evaluation.
      template <class Arg>
      void update(const Arg& x)
      {
        update_if_present(f,x);
        value = a*f();
      }

      /// Update variable corresponding to index.
      template <int index, class Arg>
      void update(const Arg& x)
      {
        update_if_present<index>(f,x);
        value = a*f();
      }

      /// Function value.
      decltype(auto) d0() const noexcept
      {
        return value;
      }

      /// First directional derivative.
        template < int idx , class Arg ,
                 class IndexedArg = IndexedType<Arg,idx> ,
                 class = std::enable_if_t< D1<F,IndexedArg>::present> >
      auto d1(const Arg& dx) const
      {
        return a * D1_<F,IndexedArg>::apply(f,dx);
      }

      /// Second directional derivative.
      template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class = std::enable_if_t< D2<F,IndexedArgX,IndexedArgY>::present> >
      auto d2(const ArgX& dx, const ArgY& dy) const
      {
        return a * D2_<F,IndexedArgX,IndexedArgY>::apply(f,dx,dy);
      }

      /// Third directional derivative.
      template < int idx , int idy , int idz , class ArgX , class ArgY , class ArgZ ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class IndexedArgZ = IndexedType<ArgZ,idz> ,
                 class = std::enable_if_t< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>::present > >
      auto d3(const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        return a * D3_<F,IndexedArgX,IndexedArgY,IndexedArgZ>::apply(f,dx,dy,dz);
      }

    private:
      double a = 1.;
      F f;
      std::decay_t<decltype(std::declval<F>()())> value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_SCALE_HH
