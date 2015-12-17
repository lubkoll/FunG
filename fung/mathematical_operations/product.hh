// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_MATHEMATICAL_OPERATIONS_PRODUCT_HH
#define FUNG_MATHEMATICAL_OPERATIONS_PRODUCT_HH

#include <type_traits>
#include <utility>

#include "fung/util/compute_sum.hh"
#include "fung/util/compute_product.hh"
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
     * \brief %Product \f$fg\f$ of functions of type F and G (F and G must satisfy the requirements of Concepts::FunctionConcept).
     */
      template <class F, class G,
              class = Concepts::FunctionConceptCheck<F> ,
              class = Concepts::FunctionConceptCheck<G> >
    struct Product: Chainer<Product<F,G,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > >
    {
    private:
      template <class IndexedArg>
      using D1Type =
      ComputeSum<
      ComputeProduct< D1<F,IndexedArg> , D0<G> >,
      ComputeProduct< D0<F> , D1<G,IndexedArg> > >;

      template <class IndexedArgX , class IndexedArgY >
      using D2Type =
      ComputeSum<
      ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D0<G> >,
      ComputeProduct< D1<F,IndexedArgX> , D1<G,IndexedArgY> >,
      ComputeProduct< D1<F,IndexedArgY> , D1<G,IndexedArgX> >,
      ComputeProduct< D0<F> , D2<G,IndexedArgX,IndexedArgY> >
      >;

      template <class IndexedArgX, class IndexedArgY, class IndexedArgZ>
      using D3Type =
      ComputeSum<
      ComputeProduct< D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> , D0<G> >,
      ComputeProduct< D2<F,IndexedArgX,IndexedArgY> , D1<G,IndexedArgZ> >,
      ComputeProduct< D2<F,IndexedArgX,IndexedArgZ> , D1<G,IndexedArgY> >,
      ComputeProduct< D1<F,IndexedArgX> , D2<G,IndexedArgY,IndexedArgZ> >,
      ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<G,IndexedArgX> >,
      ComputeProduct< D1<F,IndexedArgY> , D2<G,IndexedArgX,IndexedArgZ> >,
      ComputeProduct< D1<F,IndexedArgZ> , D2<G,IndexedArgX,IndexedArgY> >,
      ComputeProduct< D0<F> , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ> >
      >;

    public:
      /**
       * @brief Constructor passing arguments to function constructors.
       * @param f_ input for constructor of left side of product
       * @param g_ input for constructor of right side of product
       */
      template <class InitF, class InitG>
      Product(const InitF& f_, const InitG& g_)
        : f(f_), g(g_), value(f() * g())
      {}

      /// Update point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        update_if_present(f,x);
        update_if_present(g,x);
        value = f()*g();
      }

      /// Update variable corresponding to index.
      template <int index, class Arg>
      void update(const Arg& x)
      {
        update_if_present<index>(f,x);
        update_if_present<index>(g,x);
        value = f()*g();
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
      template < int id , class Arg ,
                 class IndexedArg = IndexedType<Arg,id> ,
                 class = std::enable_if_t< D1Type<IndexedArg>::present > >
      auto d1(Arg const& dx) const
      {
        return sum( product( D1<F,IndexedArg>(f,dx), D0<G>(g) ),
                    product( D0<F>(f), D1<G,IndexedArg>(g,dx) ) )();
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
        return sum( product( D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) , D0<G>(g) ),
                    product( D1<F,IndexedArgX>(f,dx) , D1<G,IndexedArgY>(g,dy) ),
                    product( D1<F,IndexedArgY>(f,dy),D1<G,IndexedArgX>(g,dx) ),
                    product( D0<F>(f),D2<G,IndexedArgX,IndexedArgY>(g,dx,dy) )
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
        return sum( product( D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz) , D0<G>(g) ),
                    product( D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) , D1<G,IndexedArgZ>(g,dz) ),
                    product( D2<F,IndexedArgX,IndexedArgZ>(f,dx,dz) , D1<G,IndexedArgY>(g,dy) ),
                    product( D1<F,IndexedArgX>(f,dx) , D2<G,IndexedArgY,IndexedArgZ>(g,dy,dz) ),
                    product( D2<F,IndexedArgY,IndexedArgZ>(f,dy,dz) , D1<G,IndexedArgX>(g,dx) ),
                    product( D1<F,IndexedArgY>(f,dy) , D2<G,IndexedArgX,IndexedArgZ>(g,dx,dz) ),
                    product( D1<F,IndexedArgZ>(f,dz) , D2<G,IndexedArgX,IndexedArgY>(g,dx,dy) ),
                    product( D0<F>(f) , D3<G,IndexedArgX,IndexedArgY,IndexedArgZ>(g,dx,dy,dz) ) )();
      }

    private:
      F f;
      G g;

      decay_t<decltype( std::declval<F>()() * std::declval<G>()() )> value;
    };
  }
}

#endif // FUNG_MATHEMATICAL_OPERATIONS_PRODUCT_HH
