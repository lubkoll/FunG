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

#ifndef RFFGEN_MATHEMATICAL_OPERATIONS_PRODUCT_HH
#define RFFGEN_MATHEMATICAL_OPERATIONS_PRODUCT_HH

#include <type_traits>
#include <utility>

#include "../Util/base.hh"
#include "../Util/computeSum.hh"
#include "../Util/computeProduct.hh"
#include "../Util/derivativeWrappers.hh"
#include "../Util/indexedType.hh"

namespace RFFGen
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
     * \brief %Product \f$fg\f$ of functions of type F and G (F and G must satisfy the requirements of Concepts::FunctionConcept).
     */
      template <class F, class G,
              class = FunctionConceptCheck<F> ,
              class = FunctionConceptCheck<G> >
    struct Product: Base , Chainer<Product<F,G,FunctionConceptCheck<F>,FunctionConceptCheck<G> > >
    {
      using Chainer<Product<F,G,FunctionConceptCheck<F>,FunctionConceptCheck<G> > >::operator();
    private:
      template <class IndexedArg>
      using D1Type = ComputeSum< ComputeProduct< D1<F,IndexedArg> , D0<G> >,
      ComputeProduct< D0<F> , D1<G,IndexedArg> > >;

      template <class IndexedArgX , class IndexedArgY >
      using D2Type = ComputeSum< ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D0<G> >,
      ComputeProduct< D1<F,IndexedArgX> , D1<G,IndexedArgY> >,
      ComputeProduct< D1<F,IndexedArgY> , D1<G,IndexedArgX> >,
      ComputeProduct< D0<F> , D2<G,IndexedArgX,IndexedArgY> >
      >;

      template <class IndexedArgX, class IndexedArgY, class IndexedArgZ>
      using D3Type = ComputeSum< ComputeProduct< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> , D0<G> > ,
      ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> >,
      ComputeProduct< D2<F,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> > ,
      ComputeProduct< D1<F,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> >,
      ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<G,IndexedArgX> > ,
      ComputeProduct< D1<F,IndexedArgY> , D2<G,IndexedArgX,IndexedArgZ> >,
      ComputeProduct< D1<F,IndexedArgZ> , D2<G,IndexedArgX,IndexedArgY> > ,
      ComputeProduct< D0<F> , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> >
      >;

    public:
      /// Default constructor. May leave member variables uninitialized! Call update before using evaluation.
      Product() = default;

      /**
       * @brief Constructor passing arguments to function constructors.
       * @param f_ input for constructor of left side of product
       * @param g_ input for constructor of right side of product
       */
      template <class InitF, class InitG>
      Product(const InitF& f_, const InitG& g_) : f(f_), g(g_)
      { updateResultOfD0(); }

      /// Reset point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        f.update(x);
        g.update(x);
        updateResultOfD0();
      }

      /// Propagate call to updateVariable() to f and g.
      template <int index, class Arg>
      void updateVariable(const Arg& x)
      {
        f.template updateVariable<index>(x);
        g.template updateVariable<index>(x);
      }

      /// Function value.
      auto d0() const noexcept
      {
        return resultOfD0;
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
      template < int id , class Arg ,
                 class IndexedArg = IndexedType<Arg,id> ,
                 class = std::enable_if_t< D1Type<IndexedArg>::present > >
      auto d1(Arg const& dx) const
      {
        return D1Type<IndexedArg>( ComputeProduct< D1<F,IndexedArg> , D0<G> >( D1<F,IndexedArg>(f,dx), D0<G>(g) ),
                                   ComputeProduct< D0<F> , D1<G,IndexedArg> >( D0<F>(f), D1<G,IndexedArg>(g,dx) ) )();
      }

      /**
       * \brief Second directional derivative.
       * \param dx direction for which the derivative is computed
       * \param dy direction for which the derivative is computed
       */
      template < int idx , int idy , class ArgX , class ArgY ,
                 class IndexedArgX = IndexedType<ArgX,idx> ,
                 class IndexedArgY = IndexedType<ArgY,idy> ,
                 class = std::enable_if_t< D2Type<IndexedArgX,IndexedArgY>::present > >
      auto d2(ArgX const& dx, ArgY const& dy) const
      {
        return D2Type<IndexedArgX,IndexedArgY>( ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D0<G> > ( D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) , D0<G>(g) ),
                                                ComputeProduct< D1<F,IndexedArgX> , D1<G,IndexedArgY> > ( D1<F,IndexedArgX>(f,dx) , D1<G,IndexedArgY>(g,dy) ),
                                                ComputeProduct< D1<F,IndexedArgY> , D1<G,IndexedArgX> > (D1<F,IndexedArgY>(f,dy),D1<G,IndexedArgX>(g,dx)),
                                                ComputeProduct< D0<F> , D2<G,IndexedArgX,IndexedArgY> >(D0<F>(f),D2<G,IndexedArgX,IndexedArgY>(g,dx,dy))
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
                 class = std::enable_if_t< D3Type<IndexedArgX,IndexedArgY,IndexedArgZ>::present > >
      auto d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        return D3Type<IndexedArgX,IndexedArgY,IndexedArgZ>( ComputeProduct< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> , D0<G>     >
                                                            ( D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz) , D0<G>(g) ),
                                                            ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> >
                                                            ( D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) , D1<G,IndexedArgZ>(g,dz) ),
                                                            ComputeProduct< D2<F,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> >
                                                            ( D2<F,IndexedArgX,IndexedArgZ>(f,dx,dz) , D1<G,IndexedArgY>(g,dy) ),
                                                            ComputeProduct< D1<F,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> >
                                                            ( D1<F,IndexedArgX>(f,dx) , D2<G,IndexedArgY,IndexedArgZ>(g,dy,dz) ),
                                                            ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<G,IndexedArgX> >
                                                            ( D2<F,IndexedArgY,IndexedArgZ>(f,dy,dz) , D1<G,IndexedArgX>(g,dx) ),
                                                            ComputeProduct< D1<F,IndexedArgY> , D2<G,IndexedArgX,IndexedArgZ> >
                                                            ( D1<F,IndexedArgY>(f,dy) , D2<G,IndexedArgX,IndexedArgZ>(g,dx,dz) ),
                                                            ComputeProduct< D1<F,IndexedArgZ> , D2<G,IndexedArgX,IndexedArgY> >
                                                            ( D1<F,IndexedArgZ>(f,dz) , D2<G,IndexedArgX,IndexedArgY>(g,dx,dy) ),
                                                            ComputeProduct< D0<F> , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> >
                                                            ( D0<F>(f) , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ>(g,dx,dy,dz) ) )();
      }

    private:
      void updateResultOfD0()
      {
        resultOfD0 = f.d0() * g.d0();
      }

      F f;
      G g;

      using type = std::conditional_t<std::is_same<std::decay_t<decltype(std::declval<F>().d0())>,std::decay_t<decltype(std::declval<G>().d0())> >::value,
                                      decltype(std::declval<F>().d0()),
                                      decltype(std::declval<F>().d0() * std::declval<G>().d0())>;
      std::remove_const_t<std::remove_reference_t<type> > resultOfD0;
    };
  }
}

#endif // RFFGEN_MATHEMATICAL_OPERATIONS_PRODUCT_HH
