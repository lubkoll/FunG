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

      @brief Sine function including first three derivatives (based on sin(double) in \<cmath\>).

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
     */
    namespace stringify
    {
        struct Sin : Chainer< Sin >
        {
            //! @copydoc Cos::Cos()
            explicit Sin( String x = "x" )
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
                return String( "sin" ).append( x );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "cos" ).append( x ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-sin" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "-cos" ).append( x ).append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        /*!
          @ingroup StringifyCMathGroup
          @brief Generate \f$ \sin\circ f \f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Sin,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto sin( const Function& f )
        {
            return Sin()( f );
        }
    }
}
