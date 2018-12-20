// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

namespace FunG
{
    /*!
    @ingroup StringifyCMathGroup

    @brief Arc sine function including first three derivatives (based on asin(String) in \<cmath\>).

    For scalar functions directional derivatives are less interesting. Incorporating this function
    as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
    namespace stringify
    {
        struct ASin : Chainer< ASin >
        {
            //! @copydoc Cos::Cos()
            explicit ASin( const String& x = 0. )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = IN_PARENS( x );
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return String( "asin" ).append( x );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return toString( 1 ) /
                       String( "sqrt(1-" )
                           .append( x )
                           .append( "^2)" )
                           .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( x ).append( "*" ).append( d1p3() ).append(
                    multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
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
            String d1p3() const
            {
                return String( "(1-" ).append( x ).append( "^2" ).append( "^(-3/2)" );
            }

            String x;
        };

        /*!
    @ingroup StringifyCMathGroup
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
