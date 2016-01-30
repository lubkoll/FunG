// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later./

#ifndef FUNG_FINALIZE_HH
#define FUNG_FINALIZE_HH

#include <type_traits>

#include "fung/util/derivative_wrappers.hh"
#include "fung/util/indexed_type.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/type_traits.hh"
#include "fung/util/zero.hh"
#include "fung/variable.hh"

namespace FunG
{
  /// @cond
  namespace Detail
  {
    template <class ReturnType>
    struct FillDefault
    {
      template <class... Args>
      __attribute__((always_inline)) ReturnType operator()(const Args&...) const
      {
        return zero<ReturnType>();
      }
    };


    template <int id,class ReturnType, bool present>
    struct FinalizeD1 : FillDefault<ReturnType> {};

    template <int id,class ReturnType>
    struct FinalizeD1<id,ReturnType,true>
    {
      template <class F, class Arg>
      __attribute__((always_inline)) ReturnType operator()(const F& f, const Arg& dx) const
      {
        return D1_< F , IndexedType<Arg,id> >::apply(f,dx);
      }
    };


    template <int idx, int idy, class ReturnType, bool present>
    struct FinalizeD2 : FillDefault<ReturnType> {};

    template <int idx, int idy, class ReturnType>
    struct FinalizeD2<idx,idy,ReturnType,true>
    {
      template <class F, class ArgX, class ArgY>
      __attribute__((always_inline)) ReturnType operator()(const F& f, const ArgX& dx, const ArgY& dy) const
      {
        return D2_< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::apply(f,dx,dy);
      }
    };


    template <int idx, int idy, int idz, class ReturnType, bool present>
    struct FinalizeD3 : FillDefault<ReturnType> {};

    template <int idx, int idy, int idz, class ReturnType>
    struct FinalizeD3<idx,idy,idz,ReturnType,true>
    {
      template <class F, class ArgX, class ArgY, class ArgZ>
      __attribute__((always_inline)) ReturnType operator()(const F& f, const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        return D3_< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::apply(f,dx,dy,dz);
      }
    };


    /// Finish function definition. The task of this class is to add undefined higher order derivatives if undefined.
    template <class F, bool hasVariables>
    struct FinalizeImpl : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template <class... Args>
      FinalizeImpl(Args&&... args) : F(std::forward<Args>(args)...)
      {}

      decltype(auto) operator()() const
      {
        return F::operator()();
      }

      template < int id , class Arg >
      ReturnType d1(const Arg& dx) const
      {
        static_assert( Checks::hasVariableId< F , id >() , "You are trying to compute the first derivative with respect to a variable that is not present" );
        static_assert( Checks::checkArgument<F,Arg,id>() , "Incompatible argument in computation of first derivative." );
        static_assert( Checks::hasConsistentFirstDerivative< F >() , "Inconsistent functional definition encountered.");

        return FinalizeD1< id , ReturnType , Checks::HasMemFn_d1< F , IndexedType<Arg,id> >::value>()(static_cast<const F&>(*this),dx);
      }

      template < int id >
      ReturnType d1() const
      {
        using Arg = Variable_t<F,id>;
        static_assert( Checks::hasVariableId< F , id >() , "You are trying to compute the first derivative with respect to a variable that is not present" );
        static_assert( is_arithmetic<Arg>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d1)." );
        static_assert( Checks::hasConsistentFirstDerivative< F >() , "Inconsistent functional definition encountered.");

        return FinalizeD1< id , ReturnType , Checks::HasMemFn_d1< F , IndexedType<Arg,id> >::value>()(static_cast<const F&>(*this),Arg(1));
      }

      template < int idx , int idy , class ArgX , class ArgY >
      ReturnType d2(const ArgX& dx, const ArgY& dy) const
      {
        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>(),
                       "You are trying to compute the second derivative with respect to at least one variable that is not present" );
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of second derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of second derivative." );
        static_assert( Checks::hasConsistentSecondDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >(), "Inconsistent functional definition encountered." );

        return FinalizeD2< idx , idy , ReturnType , Checks::HasMemFn_d2< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::value>()(static_cast<const F&>(*this),dx,dy);
      }

      template < int idx , int idy >
      ReturnType d2() const
      {
        using ArgX = Variable_t<F,idx>;
        using ArgY = Variable_t<F,idy>;

        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>(),
                       "You are trying to compute the second derivative with respect to at least one variable that is not present" );
        static_assert( is_arithmetic<ArgX>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d2, first argument)." );
        static_assert( is_arithmetic<ArgY>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d2, second argument)." );
        static_assert( Checks::hasConsistentSecondDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >(), "Inconsistent functional definition encountered." );

        return FinalizeD2< idx , idy , ReturnType , Checks::HasMemFn_d2< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::value>()(static_cast<const F&>(*this),ArgX(1),ArgY(1));
      }

      template < int idx , int idy , int idz , class ArgX , class ArgY , class ArgZ >
      ReturnType d3(const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>() && Checks::hasVariableId< F , idz >(),
                      "You are trying to compute the third derivative with respect to at least one variable that is not present" );
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgZ,idz>() , "Incompatible third argument in computation of third derivative." );
        static_assert(Checks::hasConsistentThirdDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >(), "Inconsistent functional definition encountered.");

        return FinalizeD3< idx , idy , idz , ReturnType ,
                           Checks::HasMemFn_d3< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::value>()(static_cast<const F&>(*this),dx,dy,dz);
      }

      template < int idx , int idy , int idz >
      ReturnType d3() const
      {
        using ArgX = Variable_t<F,idx>;
        using ArgY = Variable_t<F,idy>;
        using ArgZ = Variable_t<F,idz>;

        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>() && Checks::hasVariableId< F , idz >(),
                      "You are trying to compute the third derivative with respect to at least one variable that is not present" );
        static_assert( is_arithmetic<ArgX>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d3, first argument)." );
        static_assert( is_arithmetic<ArgY>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d3, second argument)." );
        static_assert( is_arithmetic<ArgZ>::value , "For non-scalar variables you have to provide a direction for which the derivative is computed (d3, third argument)." );
        static_assert(Checks::hasConsistentThirdDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >(), "Inconsistent functional definition encountered.");

        return FinalizeD3< idx , idy , idz , ReturnType ,
                           Checks::HasMemFn_d3< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::value>()(static_cast<const F&>(*this),ArgX(1),ArgY(1),ArgZ(1));
      }
    };

    template <class F>
    struct FinalizeImpl<F,false> : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template < class... Args >
      FinalizeImpl(Args&&... args) : F(std::forward<Args>(args)...)
      {}

      decltype(auto) operator()() const
      {
        return F::operator()();
      }

      template < class Arg >
      decltype(auto) operator()(Arg&& x) const
      {
        F::update(std::forward<Arg>(x));
        return F::operator()();
      }

      template < class Arg >
      ReturnType d1(const Arg& dx) const
      {
        static_assert( Checks::hasConsistentFirstDerivative< F >(), "Inconsistent functional definition encountered." );
        return FinalizeD1< 0 , ReturnType , Checks::HasMemFn_d1< F , IndexedType<Arg,0> >::value >()(static_cast<const F&>(*this),dx);
      }

      template < class ArgX , class ArgY >
      ReturnType d2(const ArgX& dx, const ArgY& dy) const
      {
        static_assert( Checks::hasConsistentSecondDerivative< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> >(), "Inconsistent functional definition encountered." );
        return FinalizeD2< 0 , 0 , ReturnType , Checks::HasMemFn_d2< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> >::value >()(static_cast<const F&>(*this),dx,dy);
      }

      template < class ArgX , class ArgY , class ArgZ >
      ReturnType d3(const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        static_assert( Checks::hasConsistentThirdDerivative< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> , IndexedType<ArgZ,0> >(), "Inconsistent functional definition encountered." );
        return FinalizeD3< 0 , 0 , 0 , ReturnType ,
                           Checks::HasMemFn_d3< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> , IndexedType<ArgZ,0> >::value >()(static_cast<const F&>(*this),dx,dy,dz);
      }
    };
  }
  /// @endcond

  /**
   * \brief Finish function definition.
   *
   * Adds the definition of possibly undefined vanishing higher order derivatives.
   * If the template class Variable is not used, then no ids must be provided for the update-function and derivatives.
   */
  template <class F>
  auto finalize(F&& f)
  {
    return Detail::FinalizeImpl< std::decay_t<F> , Checks::hasVariable< std::decay_t<F> >() >( std::forward<F>(f) );
  }
}

#endif // FUNG_FINALIZE_HH
