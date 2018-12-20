// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

#include <cmath>

namespace FunG
{
    /*!
      @ingroup StringifyCMathGroup

      @brief Cosine function including first three derivatives (based on cos(double) in \<cmath\>).

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
     */
    namespace stringify
    {
        struct Cos : Chainer< Cos >
        {
            /**
             * @brief Constructor.
             * @param x point of evaluation
             */
            explicit Cos( String x = "x" )
            {
                update( x );
            }

            /// Set point of evaluation.
            void update( const String& x )
            {
                this->x = addScope( x );
            }

            /// Function value.
            String d0() const noexcept
            {
                return String( "cos" ).append( x );
            }

            /// First (directional) derivative.
            String d1( const String& dx = "" ) const
            {
                return String( "-sin" ).append( x ).append( multiplyIfNotEmpty( dx ) );
            }

            /// Second (directional) derivative.
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-cos" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
            }

            /// Third (directional) derivative.
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "sin" ).append( x ).append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        /*!
          @ingroup StringifyCMathGroup
          @brief Generate \f$ \cos\circ f \f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Cosc,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto cos( const Function& f )
        {
            return Cos()( f );
        }
    }
}
