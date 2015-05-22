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

#ifndef RFFGEN_MATHEMATICAL_OPERATIONS_SQUARED_HH
#define RFFGEN_MATHEMATICAL_OPERATIONS_SQUARED_HH

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
     * \brief %Squared function (F must satisfy the requirements of Concepts::FunctionConcept).
     */
    template <class F, class = FunctionConceptCheck<F> >
    struct Squared : Base , Chainer< Squared< F , FunctionConceptCheck<F> > >
    {
      using Chainer< Squared< F , FunctionConceptCheck<F> > >::operator();
    private:
      template < class IndexedArgX , class IndexedArgY >
      using D2Sum = ComputeSum< ComputeProduct< D0<F> , D2<F,IndexedArgX,IndexedArgY> >,
      ComputeProduct< D1<F,IndexedArgY> , D1<F,IndexedArgX> > >;

      template < class IndexedArgX , class IndexedArgY , class IndexedArgZ >
      using D3Sum = ComputeSum< ComputeProduct< D0<F> , D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> > ,
      ComputeProduct< D1<F,IndexedArgZ> , D2<F,IndexedArgX,IndexedArgY> > ,
      ComputeProduct< D1<F,IndexedArgY> , D2<F,IndexedArgX,IndexedArgZ> > ,
      ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<F,IndexedArgX> > >;

    public:
      /// Default constructor. May leave member variables uninitialized! Call update before using evaluation.
      Squared() = default;

      /**
       * @brief Constructor passing arguments to function constructor.
       * @param f_ input for constructor of outer function
       */
      template <class InitF>
      Squared(const InitF& f_)
        : f(f_)
      {
        updateResultOfD0();
      }

      /// Reset point of evaluation.
      template <class Arg>
      void update(Arg const& x)
      {
        f.update(x);
        updateResultOfD0();
      }

      /// Propagate call to updateVariable() to f.
      template <int index, class Arg>
      void updateVariable(const Arg& x)
      {
        f.template updateVariable<index>(x);
      }

      /// Function value. Convenient access to d0().
      const auto& operator()() const noexcept
      {
        return d0();
      }

      /// Function value. Convenient access to d0() with prior call to update(x).
      template <class Arg>
      const auto& operator()(const Arg& x) const noexcept
      {
        update(x);
        return d0();
      }

      /// Function value.
      const auto& d0() const noexcept
      {
        return resultOfD0;
      }

      /**
       * \brief First directional derivative.
       * \param dx direction for which the derivative is computed
       */
      template < int id , class Arg , class IndexedArg = IndexedType<Arg,id> , class = std::enable_if_t< ComputeProduct< D0<F> , D1<F,IndexedArg> >::present > >
      auto d1(Arg const& dx) const
      {
        return 2 * f.d0() * f.template d1<id>(dx);
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
        return 2 * D2Sum<IndexedArgX,IndexedArgY>(  ComputeProduct< D0<F>             , D2<F,IndexedArgX,IndexedArgY> >( D0<F>(f)        , D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) ),
                                                  ComputeProduct< D1<F,IndexedArgY> , D1<F,IndexedArgX> >( D1<F,IndexedArgY>(f,dy)     , D1<F,IndexedArgX>(f,dx) )
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
                 class = std::enable_if_t< D3Sum<IndexedArgX,IndexedArgY,IndexedArgZ>::present > >
      auto d3(ArgX const& dx, ArgY const& dy, ArgZ const& dz) const
      {
        return 2 * D3Sum<IndexedArgX,IndexedArgY,IndexedArgZ>
              ( ComputeProduct< D0<F> , D3<F,IndexedArgX,IndexedArgY,IndexedArgZ> > ( D0<F>(f) , D3<F,IndexedArgX,IndexedArgY,IndexedArgZ>(f,dx,dy,dz) ),
                ComputeProduct< D1<F,IndexedArgZ> , D2<F,IndexedArgX,IndexedArgY> > ( D1<F,IndexedArgZ>(f,dz) , D2<F,IndexedArgX,IndexedArgY>(f,dx,dy) ),
                ComputeProduct< D1<F,IndexedArgY> , D2<F,IndexedArgX,IndexedArgZ> > ( D1<F,IndexedArgY>(f,dy) , D2<F,IndexedArgX,IndexedArgZ>(f,dx,dz) ),
                ComputeProduct< D2<F,IndexedArgY,IndexedArgZ> , D1<F,IndexedArgX> > ( D2<F,IndexedArgY,IndexedArgZ>(f,dy,dz) , D1<F,IndexedArgX>(f,dx) )
              )();
      }

    private:
      void updateResultOfD0()
      {
        resultOfD0 = f.d0() * f.d0();
      }

      F f;
      std::remove_const_t<std::remove_reference_t<decltype(std::declval<F>().d0())> > resultOfD0;
    };
  }
}

#endif // RFFGEN_MATHEMATICAL_OPERATIONS_SQUARED_HH
