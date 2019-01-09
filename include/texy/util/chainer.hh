// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <texy/mathematical_operations/chain.hh>
#include <fung/util/static_checks.hh>

#include <type_traits>

namespace texy
{
    /// @cond
    template < class Function >
    struct Chainer
    {
        decltype( auto ) operator()() const noexcept
        {
            return static_cast< const Function* >( this )->d0();
        }

        template < class Arg, class = std::enable_if_t< !FunG::Checks::isFunction< Arg >() > >
        decltype( auto ) operator()( const Arg& x )
        {
            static_cast< Function* >( this )->update( x );
            return static_cast< const Function* >( this )->d0();
        }

        template < class OtherFunction,
                   class = std::enable_if_t< FunG::Checks::isFunction< OtherFunction >() > >
        auto operator()( const OtherFunction& g )
        {
            return MathematicalOperations::Chain< Function, OtherFunction >(
                *static_cast< const Function* >( this ), g );
        }
    };
    /// @endcond
}
