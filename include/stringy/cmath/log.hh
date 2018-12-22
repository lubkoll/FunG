// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <stringy/util/string.hh>

namespace stringy
{
    struct LN : FunG::Chainer< LN >
    {
        //! @copydoc Cos::Cos()
        explicit LN( const std::string& x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = x;
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( "ln(" ).append( x ).append( ")" );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "(x^-1)" ).append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-(x^-2)" ).append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "2(x^-3)" ).append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    struct Log10 : FunG::Chainer< Log10 >
    {
        //! @copydoc Cos::Cos()
        explicit Log10( const std::string& x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = x;
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( "log_10(" ).append( x ).append( ")" );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( ln10 ).append( "x" ).append( "^(-1)" ).append(
                multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-" )
                .append( ln10 )
                .append( "^(-1)" )
                .append( x )
                .append( "^(-2)" )
                .append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "2" )
                .append( ln10 )
                .append( "^(-1)" )
                .append( x )
                .append( "^(-3)" )
                .append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
        std::string ln10{"ln(10)"};
    };

    struct Log2 : FunG::Chainer< Log2 >
    {
        //! @copydoc Cos::Cos()
        explicit Log2( const std::string& x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( "log_2" ).append( x );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "(" )
                .append( ln2 )
                .append( "*" )
                .append( x )
                .append( ")^(-1)" )
                .append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-" )
                .append( ln2 )
                .append( "^(-1)" )
                .append( x )
                .append( "^(-2)" )
                .append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "2" )
                .append( ln2 )
                .append( "^(-1)" )
                .append( x )
                .append( "^(-3)" )
                .append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
        std::string ln2{"ln(2)"};
    };

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto ln( const Function& f )
    {
        return LN()( f );
    }

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto log10( const Function& f )
    {
        return Log10()( f );
    }

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto log2( const Function& f )
    {
        return Log2()( f );
    }
}
