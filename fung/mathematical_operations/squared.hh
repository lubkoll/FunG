// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_MATHEMATICAL_OPERATIONS_SQUARED_HH
#define FUNG_MATHEMATICAL_OPERATIONS_SQUARED_HH

#include <type_traits>
#include <utility>

#include "fung/util/compute_sum.hh"
#include "fung/util/compute_product.hh"
#include "fung/util/derivative_wrappers.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"
#include "fung/util/type_traits.hh"

namespace FunG
{
  /// @cond
  template <class> struct Chainer;
  namespace Concepts { template <class> struct FunctionConceptCheck; }
  /// @endcond

  namespace MathematicalOperations
  {
    /**
     * \ingroup MathematicalOperationsGroup
     *
     * \brief %Squared function (F must satisfy the requirements of Concepts::FunctionConcept).
     */
    template <class F, class = Concepts::FunctionConceptCheck<F> >
    struct Squared : Chainer< Squared< F , Concepts::FunctionConceptCheck<F> > >
    {
    private:
      template < class IndexedArgX , class IndexedArgY >
      using D2Sum =
      ComputeSum<
      ComputeProduct< D0<F> , D2<F,IndexedArgX,IndexedArgY> >,
      ComputeProduct< D1<F,IndexedArgY> , D1<F,IndexedArgX> > >;

      template < class IndexedArgX , class IndexedArgY , class IndexedArgZ >
      using D3Sum =
      ComputeSum<
      ComputeProduct< D0<F> , D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> > ,
      ComputeProduct< D1<F,IndexedArgZ> , D2<F,IndexedArgX,IndexedArgY> > ,
      ComputeProduct< D1<F,IndexedArgY> , D2<F,IndexedArgX,IndexedArgZ> > ,
      ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<F,IndexedArgX> > >;

    public:
      /**
       * @brief Constructor passing arguments to function constructor.
       * @param f_ input for constructor of outer function
       */
      template <class InitF>
      Squared(const InitF& f_)
        : f(f_), value(f()*f())
      {}

      /// Update point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        update_if_present(f,x);
        value = f()*f();
      }

      /// Update variable corresponding to index.
      template <int index, class Arg>
      void update(const Arg& x)
      {
        update_if_present<index>(f,x);
        value = f()*f();
      }

      /// Function value.
      decltype(auto) d0() const noexcept
      {
        return value;
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
      template < int id , class Arg , class IndexedArg = IndexedType<Arg,id> , class = std::enable_if_t< ComputeProduct< D0<F> , D1<F,IndexedArg> >::present > >
      auto d1(Arg const& dx) const
      {
        return 2 * f() * D1_<F,IndexedArg>::apply(f,dx);
      }

      /**
       * \brief Second directional derivative.
       * \param dx direction for which the derivative is computed
       * \param dy direction for which the derivative is computed
       */
      template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class = std::enable_if_t< D2Sum<IndexedArgX,IndexedArgY>::present > >
      auto d2(ArgX const& dx, ArgY const& dy) const
      {
        return 2 * sum( product( D0<F>(f) , D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) ),
                        product( D1<F,IndexedArgY>(f,dy) , D1<F,IndexedArgX>(f,dx) ) )();
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
                 class = std::enable_if_t< D3Sum<IndexedArgX,IndexedArgY,IndexedArgZ>::present > >
      auto d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        return 2 * sum( product( D0<F>(f) , D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz) ),
                        product( D1<F,IndexedArgZ>(f,dz) , D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) ),
                        product( D1<F,IndexedArgY>(f,dy) , D2<F,IndexedArgX,IndexedArgZ>(f,dx,dz) ),
                        product( D2<F,IndexedArgY,IndexedArgZ>(f,dy,dz) , D1<F,IndexedArgX>(f,dx) ) )();
      }

    private:
      F f;
      decay_t<decltype(std::declval<F>()()*std::declval<F>()())> value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_SQUARED_HH
