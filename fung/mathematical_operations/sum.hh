// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_MATHEMATICAL_OPERATIONS_SUM_HH
#define FUNG_MATHEMATICAL_OPERATIONS_SUM_HH

#include <type_traits>
#include <utility>

#include "fung/util/compute_sum.hh"
#include "fung/util/derivative_wrappers.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"

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
     * \brief %Sum of functions of type F and G (F and G must satisfy the requirements of Concepts::FunctionConcept).
     */
    template < class F, class G,
               class CheckF = Concepts::FunctionConceptCheck<F> ,
               class CheckG = Concepts::FunctionConceptCheck<G> >
    struct Sum : Chainer< Sum< F, G, CheckF, CheckG > >
    {
      /**
       * @brief Constructor passing arguments to function constructors.
       * @param f_ input for constructor of first summand
       * @param g_ input for constructor of second summand
       */
      template < class InitF, class InitG >
      Sum( InitF&& f_, InitG&& g_ )
        : f( std::forward<InitF>(f_) ),
          g( std::forward<InitG>(g_) ),
          value( f() + g() )
      {}

      /// Update point of evaluation.
      template < class Arg >
      void update( Arg&& x )
      {
        update_if_present( f, x );
        update_if_present( g, std::forward<Arg>(x) );
        value = f() + g();
      }

      /// Update variable corresponding to index.
      template < int index, class Arg >
      void update( Arg&& x )
      {
        update_if_present<index>( f, x );
        update_if_present<index>( g, std::forward<Arg>(x) );
        value = f() + g();
      }

      /// Function value.
      decltype(auto) d0() const noexcept
      {
        return value;
      }

      /// First directional derivative.
      template < int id , class Arg ,
                 class IndexedArg = IndexedType< std::decay_t<Arg>, id > ,
                 class = std::enable_if_t< ComputeSum< D1<F,IndexedArg>, D1<G,IndexedArg> >::present > >
      auto d1( Arg&& dx ) const
      {
        return ComputeSum< D1<F,IndexedArg>, D1<G,IndexedArg> >( f, g,
                                                                 std::forward<Arg>(dx) )();
      }

      /// Second directional derivative.
      template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType< std::decay_t<ArgX>, idx > ,
                 class IndexedArgY = IndexedType< std::decay_t<ArgY>, idy > ,
                 class = std::enable_if_t< ComputeSum< D2<F,IndexedArgX,IndexedArgY>, D2<G,IndexedArgX,IndexedArgY> >::present > >
      auto d2( ArgX&& dx, ArgY&& dy ) const
      {
        return ComputeSum< D2<F,IndexedArgX,IndexedArgY>, D2<G,IndexedArgX,IndexedArgY> >( f, g,
                                                                                           std::forward<ArgX>(dx),
                                                                                           std::forward<ArgY>(dy) )();
      }

      /// Third directional derivative.
      template < int idx , int idy , int idz ,
                 class ArgX, class ArgY, class ArgZ ,
                 class IndexedArgX = IndexedType< std::decay_t<ArgX>, idx > ,
                 class IndexedArgY = IndexedType< std::decay_t<ArgY>, idy > ,
                 class IndexedArgZ = IndexedType< std::decay_t<ArgZ>, idz > ,
                 class = std::enable_if_t< ComputeSum< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>, D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> >::present > >
      auto d3( ArgX&& dx, ArgY&& dy, ArgZ&& dz ) const
      {
        return ComputeSum< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>, D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> >( f, g,
                                                                                                                   std::forward<ArgX>(dx),
                                                                                                                   std::forward<ArgY>(dy),
                                                                                                                   std::forward<ArgZ>(dz) )();
      }

    private:
      F f;
      G g;
      decay_t<decltype( std::declval<F>()() + std::declval<G>()() )> value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_SUM_HH
