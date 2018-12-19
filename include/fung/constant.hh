#pragma once

#include <fung/util/chainer.hh>

namespace FunG
{
    /// @cond
    template < class >
    struct ArithmeticConceptCheck;
    /// @endcond

    /// Wrap a constant.
    template < class Type, class = ArithmeticConceptCheck< Type > >
    struct Constant : Chainer< Constant< Type, ArithmeticConceptCheck< Type > > >
    {
        constexpr Constant() = default;

        /// Construct constant from copy.
        constexpr Constant( Type t_ ) : t( t_ )
        {
        }

        /// Function value.
        constexpr const Type& d0() const noexcept
        {
            return t;
        }

    private:
        Type t;
    };

    /**
     * \brief Generate a constant function that stores its argument as constant reference.
     *
     * This admits to use variable constant arguments, i.e. parameters that we want to study.
     */
    template < class Arg >
    auto constRef( const Arg& x )
    {
        return Constant< const Arg& >( x );
    }

    /**
   * \brief Wrap a constant.
   * \return Constant<Arg>(x)
   */
    template < class Arg >
    constexpr auto constant( Arg x )
    {
        return Constant< Arg >( x );
    }
}
