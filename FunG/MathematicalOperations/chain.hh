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

#ifndef FUNG_MATHEMATICAL_OPERATION_CHAIN_HH
#define FUNG_MATHEMATICAL_OPERATION_CHAIN_HH

#include <type_traits>
#include <utility>

#include "../Util/base.hh"
#include "../Util/computeSum.hh"
#include "../Util/computeChain.hh"
#include "../Util/derivativeWrappers.hh"
#include "../Util/indexedType.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  template <class> struct Chainer;
  /**
   * \endcond
   */

  namespace MathematicalOperations
  {
    /**
     * \cond DOCUMENT_FORWARD_DECLARATIONS
     */
    template <class> struct FunctionConceptCheck;
    /**
     * \endcond
     */

    /**
     * \ingroup MathematicalOperationsGroup
     *
     * \brief %Chain \f$ f\circ g \f$ of functions \f$f\f$ and \f$g\f$ of type F resp. G (F and G must satisfy the requirements of Concepts::FunctionConcept).
     */
    template < class F , class G ,
               class = FunctionConceptCheck<F>,
               class = FunctionConceptCheck<G> >
    struct Chain : Base , Chainer< Chain<F,G,FunctionConceptCheck<F>,FunctionConceptCheck<G> > >
    {
      using Chainer< Chain<F,G,FunctionConceptCheck<F>,FunctionConceptCheck<G> > >::operator ();
    private:
      using FArg = decltype(std::declval<G>().d0());

      template < class IndexedArgX , class IndexedArgY , class IndexedFArgX , class IndexedFArgY >
      using D2LazyType = ComputeSum< ComputeChainD2< F , D1<G,IndexedArgX> , D1<G,IndexedArgY> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD1< F , D2<G,IndexedArgX,IndexedArgY> , IndexedFArgX > >;


      template < class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
                 class IndexedFArgX , class IndexedFArgY , class IndexedFArgZ >
      using D3LazyType = ComputeSum< ComputeChainD3<  F , D1<G,IndexedArgX> , D1<G,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgY , IndexedFArgZ >,
      ComputeChainD2<  F , D2<G,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD2<  F , D1<G,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> , IndexedFArgX , IndexedFArgY >,
      ComputeChainD2<  F , D2<G,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgZ >,
      ComputeChainD1<  F , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> , IndexedFArgX > >;

    public:
      /// Default constructor. May leave member variables uninitialized! Call update before using evaluation.
      Chain() = default;

      /**
       * \brief Constructor.
       * \param init input for a constructor of G
       */
      template <class... InitFunction>
      Chain(const InitFunction&... init)
        : g(init...), f(g.d0())
      { updateValue(); }

      /**
       * @brief Constructor taking copies of the functions to be chained.
       * @param f_ outer function
       * @param g_ inner function
       */
      Chain(const F& f_, const G& g_) : g(g_), f(f_)
      {
        f.update(g.d0());
        updateValue();
      }

      /// Reset point of evaluation.
      template <class Arg>
      void update(const Arg& x)
      {
        g.update(x);
        f.update(g.d0());
        updateValue();
      }

      /// Propagate call to updateVariable() to f and g.
      template <int index, class Arg>
      void updateVariable(const Arg & x)
      {
        g.template updateVariable<index>(x);
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
      template < int id , class Arg ,
                 class IndexedArg = IndexedType<Arg,id> ,
                 class IndexedFArg = IndexedType<FArg,id> ,
                 class = std::enable_if_t< ComputeChainD1< F , D1<G,IndexedArg> , IndexedFArg >::present> >
      auto d1(Arg const& dx) const
      {
        return ComputeChainD1< F , D1<G,IndexedArg>, IndexedFArg >(f, D1<G,IndexedArg>(g,dx))();
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
        return D2LazyType<IndexedArgX,IndexedArgY,IndexedFArgX,IndexedFArgY>
            ( ComputeChainD2< F , D1<G,IndexedArgX> , D1<G,IndexedArgY>, IndexedFArgX , IndexedFArgY > (f,D1<G,IndexedArgX>(g,dx),D1<G,IndexedArgY>(g,dy)),
              ComputeChainD1< F , D2<G,IndexedArgX,IndexedArgY> , IndexedFArgX >                       (f,D2<G,IndexedArgX,IndexedArgY>(g,dx,dy))
              )();
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
        return D3LazyType<IndexedArgX,IndexedArgY,IndexedArgZ,IndexedFArgX,IndexedFArgY,IndexedFArgZ>
            ( ComputeChainD3< F , D1<G,IndexedArgX> , D1<G,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgY , IndexedFArgZ > ( f , dGdx , dGdy , dGdz ),
              ComputeChainD2< F , D2<G,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> , IndexedFArgX , IndexedFArgY > ( f , D2<G,IndexedArgX,IndexedArgZ>(g,dx,dz) , dGdy ),
              ComputeChainD2< F , D1<G,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> , IndexedFArgX , IndexedFArgY > ( f , dGdx , D2<G,IndexedArgY,IndexedArgZ>(g,dy,dz) ),
              ComputeChainD2< F , D2<G,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> , IndexedFArgX , IndexedFArgZ > ( f , D2<G,IndexedArgX,IndexedArgY>(g,dx,dy) , dGdz ),
              ComputeChainD1< F , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> , IndexedFArgX > ( f , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> (g,dx,dy,dz) )
              )();
      }

    private:
      void updateValue()
      {
        value = f.d0();
      }

      G g;
      F f;
      std::remove_const_t<std::remove_reference_t<decltype(std::declval<F>().d0())> > value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATION_CHAIN_HH
