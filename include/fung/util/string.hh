#pragma once

#include "chainer.hh"
#include "indexed_type.hh"

#include <string>
#include <type_traits>
#include <utility>
#include <ostream>
#include <regex>

#define IN_PARENS( ARG ) ::FunG::addScope( ARG )

namespace FunG
{
    struct String
    {
        template < class... Args,
                   std::enable_if_t< !std::is_constructible< String, Args... >::value >* = nullptr >
        String( Args&&... args ) : str( std::forward< Args >( args )... )
        {
        }

        template < class... Args,
                   std::enable_if_t< !std::is_assignable< String, Args... >::value >* = nullptr >
        String& operator=( Args&&... args )
        {
            str = std::string( std::forward< Args >( args )... );
            return *this;
        }

        String& append( String&& other )
        {
            str.append( std::move( other.str ) );
            return *this;
        }

        String& append( const String& other )
        {
            str.append( other.str );
            return *this;
        }

        bool empty() const noexcept
        {
            return str.empty();
        }

        std::string str;
    };

    template < class T >
    String toString( T&& t )
    {
        return String{std::to_string( std::forward< T >( t ) )};
    }

    inline String addScope( String str )
    {
        static std::regex notSimple{".*[+|-|\\*|/|\\(|\\^].*"};
        if ( std::regex_match( str.str, notSimple ) )
        {
            return ::FunG::String( "(" ).append( std::move( str ) ).append( ")" );
        }
        return str;
    }

    inline String multiplyIfNotEmpty( const String& dx )
    {
        return dx.empty() ? dx : String( "*" ).append( dx );
    }

    inline String multiplyIfNotEmpty( const String& dx, const String& dy )
    {
        return multiplyIfNotEmpty( dx ).append( multiplyIfNotEmpty( dy ) );
    }

    inline String multiplyIfNotEmpty( const String& dx, const String& dy, const String& dz )
    {
        return multiplyIfNotEmpty( dx, dy ).append( multiplyIfNotEmpty( dz ) );
    }

    inline String operator+( const String& lhs, const String& rhs )
    {
        return String{lhs.str + " + " + rhs.str};
    }

    inline String operator-( const String& lhs, const String& rhs )
    {
        return String{lhs.str + " - " + rhs.str};
    }

    inline String operator*( const String& lhs, const String& rhs )
    {
        return String{addScope( lhs ).str + "*" + addScope( rhs ).str};
    }

    template < class T, std::enable_if_t< std::is_arithmetic< T >::value >* = nullptr >
    String operator*( const T& lhs, const String& rhs )
    {
        return toString( lhs ).append( "*" ).append( addScope( rhs ) );
    }

    template < class T, std::enable_if_t< std::is_arithmetic< T >::value >* = nullptr >
    String operator*( String lhs, const T& rhs )
    {
        return addScope( lhs ).append( "*" ).append( toString( rhs ) );
    }

    inline String operator/( const String& lhs, const String& rhs )
    {
        return String{addScope( lhs ).str + "/" + addScope( rhs ).str};
    }

    inline std::ostream& operator<<( std::ostream& os, const String& str )
    {
        return os << str.str;
    }

    inline bool operator==( const String& lhs, const String& rhs )
    {
        return lhs.str == rhs.str;
    }
}
