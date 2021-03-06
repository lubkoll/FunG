#pragma once

#include <fung/util/mathop_traits.hh>

#include <string>
#include <type_traits>
#include <utility>
#include <ostream>
#include <regex>

namespace texy
{
    inline std::string forceAddScope( const std::string& str )
    {
        return std::string( "(" ).append( str ).append( ")" );
    }

    inline std::string addScope( std::string str )
    {
        static std::regex notSimple{".*[a-zA-Z0-9]+\\s*[+|-].*"};
        static std::regex isSimplePower{"[a-zA-Z0-9]+\\^"};
        if ( std::regex_match( str, notSimple ) && !std::regex_match( str, isSimplePower ) )
        {
            return forceAddScope( str );
        }
        return str;
    }

    inline std::string addStrictScope( const std::string& str )
    {
        static std::regex notSimple{".+[+|-|\\*|/|\\(|\\^].*"};
        if ( std::regex_match( str, notSimple ) )
        {
            return forceAddScope( str );
        }
        return str;
    }

    inline std::string addTexScope( std::string str )
    {
        return std::string( "{" ).append( std::move( str ) ).append( "}" );
    }

    inline std::string addAllScopes( std::string str )
    {
        return addTexScope( addScope( std::move( str ) ) );
    }

    inline std::string multiplyIfNotEmpty( const std::string& dx )
    {
        return dx.empty() ? dx : std::string( "*" ).append( dx );
    }

    inline std::string multiplyIfNotEmpty( const std::string& dx, const std::string& dy )
    {
        return multiplyIfNotEmpty( dx ).append( multiplyIfNotEmpty( dy ) );
    }

    inline std::string multiplyIfNotEmpty( const std::string& dx, const std::string& dy,
                                           const std::string& dz )
    {
        return multiplyIfNotEmpty( dx, dy ).append( multiplyIfNotEmpty( dz ) );
    }
}

namespace FunG
{
    template <>
    struct MathOpTraits< std::string, void >
    {

        static auto multiply( std::string lhs, std::string rhs )
        {
            using texy::addScope;
            lhs = addScope( lhs );
            rhs = addScope( rhs );
            return lhs.append( "*" ).append( rhs );
        }

        template < class S, std::enable_if_t< std::is_arithmetic< S >::value >* = nullptr >
        static auto multiply( S lhs, const std::string& rhs )
        {
            using texy::addScope;
            return std::to_string( lhs ).append( "*" ).append( addScope( rhs ) );
        }

        template < class S, std::enable_if_t< std::is_arithmetic< S >::value >* = nullptr >
        static auto multiply( const std::string& lhs, S rhs )
        {
            using texy::addScope;
            return addScope( lhs ).append( "*" ).append( std::to_string( rhs ) );
        }

        static auto add( const std::string& lhs, const std::string& rhs )
        {
            return std::string{lhs + " + " + rhs};
        }
    };
}
