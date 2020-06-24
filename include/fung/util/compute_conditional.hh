#pragma once

#include "mathop_traits.hh"
#include "type_traits.hh"

#include <algorithm>
#include <utility>

namespace FunG
{
    /// @cond
    namespace Detail
    {
    template < class X, class Y, bool = X::present, bool = Y::present >
    struct ComputeConditionalImpl
    {
        static constexpr bool present = false;
        ComputeConditionalImpl( const X&, const Y&, bool )
        {
        }
    };

    template < class X, class Y >
    struct ComputeConditionalImpl< X, Y, true, true >
    {
        static constexpr bool present = true;

        ComputeConditionalImpl( const X& x, const Y& y, bool choose_x )
            : value( choose_x ? x() : y() )
        {
        }

        decltype( auto ) operator()() const
        {
            return value;
        }

        remove_reference_t< decltype(std::declval< X >()() ) > value;
    };

    template < class X, class Y >
    struct ComputeConditionalImpl< X, Y, true, false >
    {
        static constexpr bool present = true;

        ComputeConditionalImpl( const X& x, const Y&, bool choose_x ) : value( choose_x ? x() : 0.0 )
        {
        }

        decltype( auto ) operator()() const
        {
            return value;
        }

        decltype( std::declval< X >()() ) value;
    };

    template < class X, class Y >
    struct ComputeConditionalImpl< X, Y, false, true >
    {
        static constexpr bool present = true;

        ComputeConditionalImpl( const X&, const Y& y, bool choose_x ) : value( choose_x ? 0.0 : y() )
        {
        }

        decltype( auto ) operator()() const
        {
            return value;
        }

        decltype( std::declval< Y >()() ) value;
    };
    }

    template < class... >
    struct ComputeConditional;

    template < class X, class Y >
    struct ComputeConditional< X, Y > : public Detail::ComputeConditionalImpl< X, Y >
    {
        ComputeConditional( const X& x, const Y& y, bool choose_x )
            : Detail::ComputeConditionalImpl< X, Y, X::present, Y::present >( x, y, choose_x )
        {
        }

        template < class F, class G, class... Args >
        ComputeConditional( F const& f, G const& g, bool choose_x, Args&&... args )
            : ComputeConditional( X( f, args... ), Y( g, args... ), choose_x )
        {
        }
    };
    /// @endcond
}
