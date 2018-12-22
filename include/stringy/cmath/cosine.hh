// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <stringy/util/string.hh>

#include <cmath>

namespace stringy
{
    /*!
      @ingroup StringyCMathGroup
     */
    struct Cos : FunG::Chainer< Cos >
    {
        explicit Cos( std::string x = "x" )
        {
            update( x );
        }

        /// Set point of evaluation.
        void update( const std::string& x )
        {
            this->x = addScope( x );
        }

        /// Function value.
        std::string d0() const noexcept
        {
            return std::string( "cos" ).append( x );
        }

        /// First (directional) derivative.
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "-sin" ).append( x ).append( multiplyIfNotEmpty( dx ) );
        }

        /// Second (directional) derivative.
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-cos" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
        }

        /// Third (directional) derivative.
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "sin" ).append( x ).append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto cos( const Function& f )
    {
        return Cos()( f );
    }
}
