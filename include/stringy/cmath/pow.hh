// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/mathop_traits.hh>
#include <fung/util/static_checks.hh>
#include <stringy/util/string.hh>

#include <type_traits>

namespace stringy
{
    /** @addtogroup StringyCMathGroup
     *  @{ */
    template < int dividend, int divisor = 1 >
    struct Pow : FunG::Chainer< Pow< dividend, divisor > >
    {
        //! @copydoc Cos::Cos()
        explicit Pow( std::string x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( std::string x )
        {
            this->x = addStrictScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return xtp( k );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return FunG::multiply_via_traits( k, xtp( k1 ).append( multiplyIfNotEmpty( dx ) ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return FunG::multiply_via_traits( kk1,
                                              xtp( k2 ).append( multiplyIfNotEmpty( dx, dy ) ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return FunG::multiply_via_traits(
                kk1k2, xtp( k3 ).append( multiplyIfNotEmpty( dx, dy, dz ) ) );
        }

    private:
        std::string xtp( const std::string& k ) const
        {
            return std::string( x ).append( "^" ).append( addStrictScope( k ) );
        }

        std::string x;
        const std::string k =
            std::to_string( dividend ).append( "/" ).append( std::to_string( divisor ) );
        const std::string k1 =
            std::to_string( dividend - divisor ).append( "/" ).append( std::to_string( divisor ) );
        const std::string kk1 = std::to_string( dividend * ( dividend - divisor ) )
                                    .append( "/" )
                                    .append( std::to_string( divisor * divisor ) );
        const std::string k2 = std::to_string( dividend - 2 * divisor )
                                   .append( "/" )
                                   .append( std::to_string( divisor ) );
        const std::string kk1k2 =
            std::to_string( dividend * ( dividend - divisor ) * ( dividend - 2 * divisor ) )
                .append( "/" )
                .append( std::to_string( divisor * divisor * divisor ) );
        const std::string k3 = std::to_string( dividend - 3 * divisor )
                                   .append( "/" )
                                   .append( std::to_string( divisor ) );
    };

    /// @cond
    template <>
    struct Pow< 1, 1 > : FunG::Chainer< Pow< 1, 1 > >
    {
        //! @copydoc Cos::Cos()
        explicit Pow( std::string x = "x" )
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
            return x;
        }

        //! @copydoc Cos::d1()
        std::string d1( std::string dx = "" ) const
        {
            return std::string( "1" ).append( multiplyIfNotEmpty( dx ) );
        }

    private:
        std::string x;
    };

    /// @cond
    template <>
    struct Pow< 2, 1 > : FunG::Chainer< Pow< 2, 1 > >
    {
        //! @copydoc Cos::Cos()
        explicit Pow( std::string x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addStrictScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( x ).append( "^2" );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "2" ).append( x ).append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "2" ).append( multiplyIfNotEmpty( dx, dy ) );
        }

    private:
        std::string x;
    };

    /// @cond
    template <>
    struct Pow< 3, 1 > : FunG::Chainer< Pow< 3, 1 > >
    {
        //! @copydoc Cos::Cos()
        explicit Pow( const std::string& x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addStrictScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( x ).append( "^3" );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "3" ).append( x ).append( "^2" ).append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "6" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "6" ).append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    template < int dividend >
    struct Pow< dividend, 1 > : FunG::Chainer< Pow< dividend, 1 > >
    {
        //! @copydoc Cos::Cos()
        explicit Pow( const std::string& x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addStrictScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( x ).append( "^" ).append( std::to_string( dividend ) );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::to_string( dividend )
                .append( x )
                .append( "^" )
                .append( std::to_string( dividend - 1 ) )
                .append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::to_string( dividend * ( dividend - 1 ) )
                .append( x )
                .append( "^" )
                .append( std::to_string( dividend - 2 ) )
                .append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::to_string( dividend * ( dividend - 1 ) * ( dividend - 2 ) )
                .append( x )
                .append( "^" )
                .append( std::to_string( dividend - 3 ) )
                .append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto sqrt( const Function& f )
    {
        return Pow< 1, 2 >()( f );
    }

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto cbrt( const Function& f )
    {
        return Pow< 1, 3 >()( f );
    }

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto cbrt2( const Function& f )
    {
        return Pow< 2, 3 >()( f );
    }

    template < int k, int l, class Function,
               class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto pow( const Function& f )
    {
        return Pow< k, l >()( f );
    }

    template < int k, class Function,
               class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto pow( const Function& f )
    {
        return Pow< k >()( f );
    }
    /** @} */
}
