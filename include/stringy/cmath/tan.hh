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

    /*!
      @brief Tangent function including first three derivatives.

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
     */
    struct Tan : FunG::Chainer< Tan >
    {
        //! @copydoc Cos::Cos()
        explicit Tan( const std::string& x = "x" )
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
            return std::string( "tan" ).append( x );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "(1 +" ).append( "tan^2" ).append( x ).append( ")" ).append(
                multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "2" )
                .append( d0() )
                .append( "*" )
                .append( d1( dx ) )
                .append( multiplyIfNotEmpty( dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "2*" )
                .append( d1( "" ) )
                .append( "*" )
                .append( "(1 + 3*tan^2" )
                .append( x )
                .append( ")" )
                .append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto tan( const Function& f )
    {
        return Tan()( f );
    }
    /** @} */
}
