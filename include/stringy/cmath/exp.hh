// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <stringy/util/string.hh>

namespace stringy
{
    /** @addtogroup std::stringifyCMathGroup
     *  @{ */
    namespace stringify
    {
        struct Exp : FunG::Chainer< Exp >
        {
            //! @copydoc Cos::d0()
            explicit Exp( const std::string& x = "x" )
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
                return std::string( "(e^" ).append( x ).append( ")" );
            }

            //! @copydoc Cos::d0()
            std::string d1( const std::string& dx = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d0()
            std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d0()
            std::string d3( const std::string& dx = "", const std::string& dy = "",
                            const std::string& dz = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            std::string x;
        };

        struct Exp2 : FunG::Chainer< Exp2 >
        {
            //! @copydoc Cos::Cos()
            explicit Exp2( const std::string& x = "x" )
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
                return addScope( std::string( "2^" ).append( x ) );
            }

            //! @copydoc Cos::d1()
            std::string d1( const std::string& dx = "" ) const
            {
                return addScope( std::string( "ln(2)*(2^" )
                                     .append( x )
                                     .append( ")" )
                                     .append( multiplyIfNotEmpty( dx ) ) );
            }

            //! @copydoc Cos::d2()
            std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
            {
                return addScope( std::string( "(ln(2)^2)*(2^" )
                                     .append( x )
                                     .append( ")" )
                                     .append( multiplyIfNotEmpty( dx, dy ) ) );
            }

            //! @copydoc Cos::d3()
            std::string d3( const std::string& dx = "", const std::string& dy = "",
                            const std::string& dz = "" ) const
            {
                return addScope( std::string( "(ln(2)^3)*(2^" )
                                     .append( x )
                                     .append( ")" )
                                     .append( multiplyIfNotEmpty( dx, dy, dz ) ) );
            }

        private:
            std::string x;
            std::string ln2{"ln(2)"};
        };

        template < class Function,
                   class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
        auto exp( const Function& f )
        {
            return Exp()( f );
        }

        template < class Function,
                   class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
        auto exp2( const Function& f )
        {
            return Exp2()( f );
        }
        /** @} */
    }
