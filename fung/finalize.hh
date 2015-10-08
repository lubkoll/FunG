// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later./

#ifndef FUNG_FINALIZE_HH
#define FUNG_FINALIZE_HH

#include <type_traits>

#include "fung/util/consistency_check.hh"
#include "fung/util/evaluate_if_present.hh"
#include "fung/util/indexed_type.hh"
#include "fung/util/zero.hh"
#include "fung/variable.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATION_DETAILS
   */
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
        return f.template d1<id>(dx);
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
        return f.template d2<idx,idy>(dx,dy);
      }
    };


    template <int idx, int idy, int idz, class ReturnType, bool present>
    struct FinalizeD3 : FillDefault<ReturnType> {};

    template <int idx, int idy, int idz, class ReturnType>
    struct FinalizeD3<idx,idy,idz,ReturnType,true>
    {
      template <class F, class Arg>
      __attribute__((always_inline)) ReturnType operator()(const F& f, const Arg& dx, const Arg& dy, const Arg& dz) const { return f.template d3<idx,idy,idz>(dx,dy,dz); }
    };


    /**
   * Finish function definition. The task of this class is to add undefined higher order derivatives if undefined.
   */
    template <class F, bool arithmeticReturnType, bool hasVariables>
    struct FinalizeImpl : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template <class... Args>
      FinalizeImpl(const Args&... args) : F(args...)
      {}

      ReturnType operator()() const
      {
        return F::d0();
      }

      template < int id , class Arg >
      ReturnType d1(const Arg& dx) const
      {
        static_assert( Checks::hasVariableId< F , id >() , "You are trying to compute the first derivative with respect to a variable that is not present" );
        static_assert( Checks::checkArgument<F,Arg,id>() , "Incompatible argument in computation of first derivative." );
        static_assert( hasConsistentFirstDerivative< F >() , "Inconsistent functional definition encountered.");

        return FinalizeD1< id , ReturnType , HasD1MemberFunction< F , IndexedType<Arg,id> >::value>()(static_cast<const F&>(*this),dx);
      }

      template < int idx , int idy , class ArgX , class ArgY >
      ReturnType d2(const ArgX& dx, const ArgY& dy) const
      {
        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>(),
                       "You are trying to compute the second derivative with respect to at least one variable that is not present" );
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of second derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of second derivative." );
        static_assert( hasConsistentSecondDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >(), "Inconsistent functional definition encountered." );

        return FinalizeD2< idx , idy , ReturnType , HasD2MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::value>()(static_cast<const F&>(*this),dx,dy);
      }

      template < int idx , int idy , int idz , class ArgX , class ArgY , class ArgZ >
      ReturnType d3(const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        static_assert( Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>() && Checks::hasVariableId< F , idz >(),
                      "You are trying to compute the third derivative with respect to at least one variable that is not present" );
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgZ,idz>() , "Incompatible third argument in computation of third derivative." );
        static_assert(hasConsistentThirdDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >(), "Inconsistent functional definition encountered.");

        return FinalizeD3< idx , idy , idz , ReturnType ,
                           HasD3MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::value>()(static_cast<const F&>(*this),dx,dy,dz);
      }
    };

    template <class F>
    struct FinalizeImpl<F,true,true> : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template <class... Args>
      FinalizeImpl(const Args&... args) : F(args...)
      {}

      ReturnType operator()() const
      {
        return F::d0();
      }

      template < int id , class Arg = double >
      ReturnType d1(Arg dx=1) const
      {
        static_assert( Checks::hasVariableId< F , id >(), "You are trying to compute the first derivative with respect to a variable that is not present" );
        static_assert( Checks::checkArgument<F,Arg,id>() , "Incompatible argument in computation of first derivative." );
        static_assert( hasConsistentFirstDerivative< F >(), "Inconsistent functional definition encountered." );
        return FinalizeD1< id , ReturnType , HasD1MemberFunction< F , IndexedType<Arg,id> >::value >()(static_cast<const F&>(*this),dx);
      }

      template < int idx , int idy , class ArgX = double , class ArgY = double >
      ReturnType d2(ArgX dx=1, ArgY dy=1) const
      {
        static_assert(Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>(),
                      "You are trying to compute the second derivative with respect to at least one variable that is not present");
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of second derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of second derivative." );
        static_assert(hasConsistentSecondDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >(), "Inconsistent functional definition encountered." );

        return FinalizeD2< idx , idy , ReturnType , HasD2MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::value >()(static_cast<const F&>(*this),dx,dy);
      }

      template < int idx , int idy , int idz , class ArgX = double , class ArgY = double , class ArgZ = double>
      ReturnType d3(ArgX dx=1, ArgY dy=1, ArgZ dz=1) const
      {
        static_assert(Checks::hasVariableId< F , idx >() && Checks::hasVariableId< F , idy>() && Checks::hasVariableId< F , idz >(),
                      "You are trying to compute the third derivative with respect to at least one variable that is not present");
        static_assert( Checks::checkArgument<F,ArgX,idx>() , "Incompatible first argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgY,idy>() , "Incompatible second argument in computation of third derivative." );
        static_assert( Checks::checkArgument<F,ArgZ,idz>() , "Incompatible third argument in computation of third derivative." );
        static_assert(hasConsistentThirdDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >(), "Inconsistent functional definition encountered.");

        return FinalizeD3< idx , idy , idy , ReturnType ,
                           HasD3MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::value >()(static_cast<const F&>(*this),dx,dy,dz);
      }
    };

    template <class F>
    struct FinalizeImpl<F,true,false> : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template <class... Args>
      FinalizeImpl(const Args&... args) : F(args...)
      {}

      template <class Arg>
      ReturnType operator()(const Arg& x)
      {
        update_if_present(*this,x);
        return F::d0();
      }


      ReturnType operator()() const
      {
        return F::d0();
      }

      template < class Arg = double >
      ReturnType d1(Arg dx=1) const
      {
        static_assert( hasConsistentFirstDerivative< F >(), "Inconsistent functional definition encountered." );
        return FinalizeD1< 0 , ReturnType , HasD1MemberFunction< F , IndexedType<Arg,0> >::value >()(static_cast<const F&>(*this),dx);
      }

      template < class ArgX = double , class ArgY = double >
      ReturnType d2(ArgX dx=1, ArgY dy=1) const
      {
        static_assert( hasConsistentSecondDerivative< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> >(), "Inconsistent functional definition encountered." );
        return FinalizeD2< 0 , 0 , ReturnType , HasD2MemberFunction< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> >::value >()(static_cast<const F&>(*this),dx,dy);
      }

      template < class ArgX = double , class ArgY = double , class ArgZ = double>
      ReturnType d3(ArgX dx=1, ArgY dy=1, ArgZ dz=1) const
      {
        static_assert( hasConsistentThirdDerivative< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> , IndexedType<ArgZ,0> >(), "Inconsistent functional definition encountered." );
        return FinalizeD3< 0 , 0 , 0 , ReturnType ,
                           HasD3MemberFunction< F , IndexedType<ArgX,0> , IndexedType<ArgY,0> , IndexedType<ArgZ,0> >::value >()(static_cast<const F&>(*this),dx,dy,dz);
      }
    };

    template <class F>
    struct FinalizeImpl<F,false,false> : F
    {
      using ReturnType = std::decay_t<decltype(std::declval<F>().d0())>;

      template <class... Args>
      FinalizeImpl(const Args&... args) : F(args...)
      {}

      ReturnType operator()() const
      {
        return F::d0();
      }

      template < int id = 0 , class Arg = double >
      ReturnType d1(const Arg& dx) const
      {
        static_assert( hasConsistentFirstDerivative< F >(), "Inconsistent functional definition encountered." );
        return FinalizeD1< id , ReturnType , HasD1MemberFunction< F , IndexedType<Arg,id> >::value >()(static_cast<const F&>(*this),dx);
      }

      template < int idx = 0 , int idy = 0 , class ArgX = double , class ArgY = double >
      ReturnType d2(const ArgX& dx, const ArgY& dy) const
      {
        static_assert( hasConsistentSecondDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >(), "Inconsistent functional definition encountered." );
        return FinalizeD2< idx , idy , ReturnType , HasD2MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> >::value >()(static_cast<const F&>(*this),dx,dy);
      }

      template < int idx = 0 , int idy = 0 , int idz = 0 , class ArgX = double , class ArgY = double , class ArgZ = double>
      ReturnType d3(const ArgX& dx, const ArgY& dy, const ArgZ& dz) const
      {
        static_assert( hasConsistentThirdDerivative< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >(), "Inconsistent functional definition encountered." );
        return FinalizeD3< idx , idy , idy , ReturnType ,
                           HasD3MemberFunction< F , IndexedType<ArgX,idx> , IndexedType<ArgY,idy> , IndexedType<ArgZ,idz> >::value >()(static_cast<const F&>(*this),dx,dy,dz);
      }
    };
  }
  /**
   * \endcond
   */

  /**
   * \brief Finish function definition.
   *
   * Adds the definition of possibly undefined vanishing higher order derivatives.
   */
  template <class F, bool arithmeticArgument = false>
  using Finalize = Detail::FinalizeImpl< F , arithmeticArgument , Checks::hasVariable<F>() >;

  /**
   * \brief Finish function definition.
   *
   * Adds the definition of possibly undefined vanishing higher order derivatives.
   */
  template <class F>
  auto finalize(const F& f)
  {
    return Finalize<F>(f);
  }

  /**
   * \brief Finish function definition.
   *
   * Adds the definition of possibly undefined vanishing higher order derivatives.
   */
  template <class F>
  auto finalize_scalar(const F& f)
  {
    return Finalize<F,true>(f);
  }
}

#endif // FUNG_FINALIZE_HH
