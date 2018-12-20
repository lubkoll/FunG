// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

namespace FunG
{
    /** @addtogroup StringifyCMathGroup
   *  @{ */

    /*!
    @brief Tangent function including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function
    as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
    namespace stringify
    {
        struct Tan : Chainer< Tan >
        {
            //! @copydoc Cos::Cos()
            explicit Tan( const String& x = 0. )
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
                return String( "tan" ).append( x );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "(1 +" ).append( "tan^2" ).append( x ).append( ")" ).append(
                    multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return ( String( "2" ).append( d0() ) * d1( dx ) )
                    .append( multiplyIfNotEmpty( dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return (toString(2) * d1("") * String("(1 + 3*tan^2").append(x).append(")").append(multiplyIfNotEmpty(dx,dy,dz));
            }

        private:
            String x;
        };

        /*!
    @brief Generate \f$ \tan\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Tan,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto tan( const Function& f )
        {
            return Tan()( f );
        }
    }
    /** @} */
}
