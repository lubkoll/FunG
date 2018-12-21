// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

namespace FunG
{
    /*!
    @ingroup std::stringifyCMathGroup

    @brief Arc sine function including first three derivatives (based on asin(std::string) in
    \<cmath\>).

    For scalar functions directional derivatives are less interesting. Incorporating this function
    as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
    namespace stringify
    {
        struct ASin : Chainer< ASin >
        {
            //! @copydoc Cos::Cos()
            explicit ASin( const std::string& x = "x" )
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
                return std::string( "asin" ).append( x );
            }

            //! @copydoc Cos::d1()
            std::string d1( const std::string& dx = "" ) const
            {
                return std::to_string( 1 )
                    .append( "/" )
                    .append( "sqrt(1-" )
                    .append( x )
                    .append( "^2)" )
                    .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
            {
                return std::string( x ).append( "*" ).append( d1p3() ).append(
                    multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            std::string d3( const std::string& dx = "", const std::string& dy = "",
                            const std::string& dz = "" ) const
            {
                return d1p3()
                    .append( "(1 + 3*" )
                    .append( x )
                    .append( "^2/(" )
                    .append( d1( "" ) )
                    .append( "^2))" )
                    .append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            std::string d1p3() const
            {
                return std::string( "(1-" ).append( x ).append( "^2" ).append( "^(-3/2)" );
            }

            std::string x;
        };

        /*!
    @ingroup std::stringifyCMathGroup
    @brief Generate \f$ \arcsin\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<ASin,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto asin( const Function& f )
        {
            return ASin()( f );
        }
    }
}
