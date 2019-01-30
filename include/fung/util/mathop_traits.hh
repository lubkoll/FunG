#pragma once

#include <type_traits>

namespace FunG
{
    template < class T, class = void >
    struct MathOpTraits
    {
        template < class S >
        static constexpr auto multiply( const T& lhs, const S& rhs )
        {
            return lhs * rhs;
        }

        static constexpr auto add( const T& lhs, const T& rhs )
        {
            return lhs + rhs;
        }
    };

    template < class T >
    struct MathOpTraits< T, std::enable_if_t< std::is_arithmetic< T >::value > >
    {
        static constexpr auto multiply( T lhs, T rhs ) noexcept
        {
            return lhs * rhs;
        }

        static constexpr auto add( T lhs, T rhs ) noexcept
        {
            return lhs + rhs;
        }
    };

    template < class T, class S,
               std::enable_if_t< std::is_same< std::decay_t< T >, std::decay_t< S > >::value &&
                                 !std::is_arithmetic< std::decay_t< T > >::value >* = nullptr >
    auto multiply_via_traits( T&& lhs, S&& rhs )
    {
        return MathOpTraits< std::decay_t< T > >::multiply( std::forward< T >( lhs ),
                                                            std::forward< S >( rhs ) );
    }

    template < class T, class S,
               std::enable_if_t< !std::is_same< std::decay_t< T >, std::decay_t< S > >::value &&
                                 !std::is_arithmetic< std::decay_t< T > >::value &&
                                 !std::is_arithmetic< std::decay_t< S > >::value >* = nullptr >
    auto multiply_via_traits( T&& lhs, S&& rhs )
    {
        return MathOpTraits< std::decay_t< T > >::multiply( std::forward< T >( lhs ),
                                                            std::forward< S >( rhs ) );
    }

    template < class T, class S, std::enable_if_t< std::is_arithmetic< T >::value &&
                                                   std::is_arithmetic< S >::value >* = nullptr >
    auto multiply_via_traits( T lhs, S rhs )
    {
        return MathOpTraits< std::common_type_t< T, S > >::multiply( lhs, rhs );
    }

    template < class T, class S, std::enable_if_t< std::is_arithmetic< T >::value >* = nullptr,
               std::enable_if_t< !std::is_arithmetic< std::decay_t< S > >::value >* = nullptr >
    auto multiply_via_traits( T lhs, S&& rhs )
    {
        return MathOpTraits< std::decay_t< S > >::multiply( lhs, std::forward< S >( rhs ) );
    }

    template < class T, class S, std::enable_if_t< std::is_arithmetic< S >::value >* = nullptr,
               std::enable_if_t< !std::is_arithmetic< std::decay_t< T > >::value >* = nullptr >
    auto multiply_via_traits( T&& lhs, S rhs )
    {
        return MathOpTraits< std::decay_t< T > >::multiply( std::forward< T >( lhs ), rhs );
    }

    template < class T, class S,
               std::enable_if_t< std::is_same< std::decay_t< T >, std::decay_t< S > >::value &&
                                 !std::is_arithmetic< std::decay_t< T > >::value >* = nullptr >
    auto add_via_traits( T&& lhs, S&& rhs )
    {
        return MathOpTraits< std::decay_t< T > >::add( std::forward< T >( lhs ),
                                                       std::forward< S >( rhs ) );
    }

    template < class T, class S, std::enable_if_t< std::is_arithmetic< T >::value &&
                                                   std::is_arithmetic< S >::value >* = nullptr >
    auto add_via_traits( T lhs, S rhs )
    {
        return MathOpTraits< std::common_type_t< T, S > >::add( lhs, rhs );
    }
}
