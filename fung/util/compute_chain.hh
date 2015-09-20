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

#ifndef FUNG_UTIL_COMPUTE_CHAIN_HH
#define FUNG_UTIL_COMPUTE_CHAIN_HH

#include <utility>
#include "consistency_check.hh"
#include "indexed_type.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
  namespace Detail
  {
    template <class F, class IndexedX, bool allPresent,
              class X = typename IndexedX::type,
              int id = IndexedX::index>
    struct ComputeChainD1Impl
    {
      static constexpr bool present = false;
      ComputeChainD1Impl(const F&, const X&) {}
    };

    template <class F, class IndexedX, class IndexedY, bool allPresent,
              class X = typename IndexedX::type,
              class Y = typename IndexedY::type,
              int idx = IndexedX::index,
              int idy = IndexedY::index>
    struct ComputeChainD2Impl
    {
      static constexpr bool present = false;
      ComputeChainD2Impl(const F&, const X&, const Y&) {}
    };

    template <class F, class IndexedX, class IndexedY, class IndexedZ, bool allPresent,
              class X = typename IndexedX::type,
              class Y = typename IndexedY::type,
              class Z = typename IndexedZ::type,
              int idx = IndexedX::index,
              int idy = IndexedY::index,
              int idz = IndexedZ::index>
    struct ComputeChainD3Impl
    {
      static constexpr bool present = false;
      ComputeChainD3Impl(const F&, const X&, const Y&, const Z&) {}
    };


    template <class F, class IndexedX, class X, int id>
    struct ComputeChainD1Impl<F,IndexedX,true,X,id>
    {
      static constexpr bool present = true;

      ComputeChainD1Impl(const F& f, const X& x) : value(f.template d1<id>(x()))
      {}

      auto operator()() const
      {
        return value;
      }

      decltype(std::declval<F>().template d1<id>(std::declval<X>()())) value;
    };

    template <class F, class IndexedX, class IndexedY, class X, class Y, int idx, int idy>
    struct ComputeChainD2Impl<F,IndexedX,IndexedY,true,X,Y,idx,idy>
    {
      static constexpr bool present = true;

      ComputeChainD2Impl(const F& f, const X& x, const Y& y) : value(f.template d2<idx,idy>(x(),y()))
      {}

      auto operator()() const
      {
        return value;
      }

      decltype( std::declval<F>().template d2<idx,idy>( std::declval<X>()() , std::declval<Y>()() ) ) value;
    };

    template <class F, class IndexedX, class IndexedY, class IndexedZ, class X, class Y, class Z, int idx, int idy, int idz>
    struct ComputeChainD3Impl<F,IndexedX,IndexedY,IndexedZ,true,X,Y,Z,idx,idy,idz>
    {
      static constexpr bool present = true;

      ComputeChainD3Impl(const F& f, const X& x, const Y& y, const Z& z) : value(f.template d3<idx,idy,idz>(x(),y(),z()))
      {}

      auto operator()() const
      {
        return value;
      }

      decltype( std::declval<F>().template d3<idx,idy,idz>( std::declval<X>()() , std::declval<Y>()() , std::declval<Z>()() ) ) value;
    };
  }

  template < class F , class X , class IndexedArg ,
             class IndexedX = IndexedType<X,IndexedArg::index> >
  struct ComputeChainD1 : public Detail::ComputeChainD1Impl<F,IndexedX,HasD1MemberFunction<F,IndexedArg>::value && X::present>
  {
    ComputeChainD1(F const& f, X const& x)
      : Detail::ComputeChainD1Impl<F,IndexedX,HasD1MemberFunction<F,IndexedArg>::value && X::present> (f,x)
    {}
  };

  template <class IndexedArg, class F, class X>
  auto chain(const F& f, const X& x)
  {
    return ComputeChainD1<F,X,IndexedArg>(f,x);
  }

  template < class F , class X , class Y , class IndexedArgX , class IndexedArgY ,
             class IndexedX = IndexedType<X,IndexedArgX::index> ,
             class IndexedY = IndexedType<Y,IndexedArgY::index> >
  struct ComputeChainD2
      : public Detail::ComputeChainD2Impl<F,IndexedX,IndexedY,HasD2MemberFunction<F,IndexedArgX,IndexedArgY>::value && X::present && Y::present>
  {
    ComputeChainD2(const F& f, const X& x, const Y& y)
      : Detail::ComputeChainD2Impl<F,IndexedX,IndexedY,HasD2MemberFunction< F , IndexedArgX , IndexedArgY >::value && X::present && Y::present >
        (f,x,y)
    {}
  };

  template <class IndexedArgX, class IndexedArgY,
            class F, class X, class Y>
  auto chain(const F &f, const X &x, const Y& y)
  {
    return ComputeChainD2<F,X,Y,IndexedArgX,IndexedArgY>(f,x,y);
  }

  template < class F , class X , class Y , class Z , class IndexedArgX , class IndexedArgY , class IndexedArgZ ,
             class IndexedX = IndexedType<X,IndexedArgX::index> ,
             class IndexedY = IndexedType<Y,IndexedArgY::index> ,
             class IndexedZ = IndexedType<Z,IndexedArgZ::index> >
  struct ComputeChainD3
      : public Detail::ComputeChainD3Impl<F,IndexedX,IndexedY,IndexedZ,HasD3MemberFunction<F,IndexedArgX,IndexedArgY,IndexedArgZ>::value && X::present && Y::present && Z::present>
  {
    ComputeChainD3(const F& f, const X& x, const Y& y, const Z& z)
      : Detail::ComputeChainD3Impl<F,IndexedX,IndexedY,IndexedZ,HasD3MemberFunction<F,IndexedArgX,IndexedArgY,IndexedArgZ>::value && X::present && Y::present && Z::present> (f,x,y,z)
    {}
  };

  template <class IndexedArgX, class IndexedArgY, class IndexedArgZ,
            class F, class X, class Y, class Z>
  auto chain(const F &f, const X &x, const Y& y, const Z& z)
  {
    return ComputeChainD3<F,X,Y,Z,IndexedArgX,IndexedArgY,IndexedArgZ>(f,x,y,z);
  }
  /**
   * \endcond
   */
}

#endif // FUNG_UTIL_COMPUTE_CHAIN_HH
