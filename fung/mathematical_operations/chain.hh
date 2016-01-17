// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_MATHEMATICAL_OPERATION_CHAIN_HH
#define FUNG_MATHEMATICAL_OPERATION_CHAIN_HH

#include <type_traits>
#include <utility>

#include "fung/util/compute_sum.hh"
#include "fung/util/compute_chain.hh"
#include "fung/util/derivative_wrappers.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct Chainer;
  namespace Concepts { template <class> struct FunctionConceptCheck; }
  /**
   * \endcond
   */

  namespace MathematicalOperations
  {
    /**
     * \ingroup MathematicalOperationsGroup
     *
     * \brief %Chain \f$ f\circ g \f$ of functions \f$f\f$ and \f$g\f$ of type F resp. G (F and G must satisfy the requirements of Concepts::FunctionConcept).
     */
    template < class F , class G ,
               class = Concepts::FunctionConceptCheck<F>,
               class = Concepts::FunctionConceptCheck<G> >
    struct Chain : Chainer< Chain<F,G,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > >
    {
    private:
      using FArg = decltype(std::declval<G>()());

      template < class IndexedArgX , class IndexedArgY , class IndexedFArgX , class IndexedFArgY >
      using D2LazyType =
      ComputeSum<
      ComputeChainD2< F , D1<G,IndexedArgX> , D1<G,IndexedArgY> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD1< F , D2<G,IndexedArgX,IndexedArgY> , IndexedFArgX > >;


      template < class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
                 class IndexedFArgX , class IndexedFArgY , class IndexedFArgZ >
      using D3LazyType =
      ComputeSum<
      ComputeChainD3<  F , D1<G,IndexedArgX> , D1<G,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgY , IndexedFArgZ >,
      ComputeChainD2<  F , D2<G,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD2<  F , D1<G,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD2<  F , D2<G,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgZ >,
      ComputeChainD1<  F , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> , IndexedFArgX > >;

    public:
      /**
       * \brief Constructor.
       * \param init input for a constructor of G
       */
      template <class... InitFunction>
      Chain(const InitFunction&... init)
        : g(init...), f(g())
      {}

      /**
       * @brief Constructor taking copies of the functions to be chained.
       * @param f_ outer function
       * @param g_ inner function
       */
      Chain(const F& f_, const G& g_)
        : g(g_), f(f_)
      {
        update_if_present(f,g());
      }

      /// Update point of evaluation.
      template <class Arg>
      void update(const Arg& x)
      {
        update_if_present(g,x);
        update_if_present(f,g());
      }

      /// Update variable corresponding to index.
      template <int index, class Arg>
      void update(const Arg & x)
      {
        update_if_present<index>(g,x);
        update_if_present(f,g());
      }

      /// Function value.
      decltype(auto) d0() const noexcept
      {
        return f();
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
      template < int id , class Arg ,
                 class IndexedArg = IndexedType<Arg,id> ,
                 class IndexedFArg = IndexedType<FArg,id> ,
                 class = std::enable_if_t< ComputeChainD1< F , D1<G,IndexedArg> , IndexedFArg >::present> >
      auto d1(Arg const& dx) const
      {
        return chain<IndexedFArg>( f, D1<G,IndexedArg>(g,dx))();
      }

      /**
       * \brief Second directional derivative.
       * \param dx direction for which the derivative is computed
       * \param dy direction for which the derivative is computed
       */
      template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class IndexedFArgX = IndexedType<FArg,idx> ,
                 class IndexedFArgY = IndexedType<FArg,idy> ,
                 class = std::enable_if_t< D2LazyType<IndexedArgX,IndexedArgY,IndexedFArgX,IndexedFArgY>::present > >
      auto d2(ArgX const& dx, ArgY const& dy) const
      {
        return sum( chain< IndexedFArgX , IndexedFArgY > (f,D1<G,IndexedArgX>(g,dx),D1<G,IndexedArgY>(g,dy)),
                    chain< IndexedFArgX > (f,D2<G,IndexedArgX,IndexedArgY>(g,dx,dy)) )();
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
                 class IndexedFArgX = IndexedType<FArg,idx> ,
                 class IndexedFArgY = IndexedType<FArg,idy> ,
                 class IndexedFArgZ = IndexedType<FArg,idz> ,
                 class = std::enable_if_t< D3LazyType<IndexedArgX,IndexedArgY,IndexedArgZ,IndexedFArgX,IndexedFArgY,IndexedFArgZ>::present > >
      auto d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        D1<G,IndexedArgX> dGdx(g,dx);
        D1<G,IndexedArgY> dGdy(g,dy);
        D1<G,IndexedArgZ> dGdz(g,dz);
        return sum( chain< IndexedFArgX , IndexedFArgY , IndexedFArgZ > ( f , dGdx , dGdy , dGdz ),
                    chain< IndexedFArgX , IndexedFArgY > ( f , D2<G,IndexedArgX,IndexedArgZ>(g,dx,dz) , dGdy ),
                    chain< IndexedFArgX , IndexedFArgY > ( f , dGdx , D2<G,IndexedArgY,IndexedArgZ>(g,dy,dz) ),
                    chain< IndexedFArgX , IndexedFArgZ > ( f , D2<G,IndexedArgX,IndexedArgY>(g,dx,dy) , dGdz ),
                    chain< IndexedFArgX > ( f , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> (g,dx,dy,dz) ) )();
      }

    private:
      G g;
      F f;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATION_CHAIN_HH
