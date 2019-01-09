// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <texy/util/chainer.hh>
#include <texy/util/string.hh>
#include <fung/util/static_checks.hh>

#include <cmath>

/*!
  @ingroup TexifyCMathGroup

  @brief Cosine function including first three derivatives (based on cos(double) in \<cmath\>).

  For scalar functions directional derivatives are less interesting. Incorporating this function
  as building block for more complex functions requires directional derivatives. These occur
  during applications of the chain rule.
 */
namespace texy
{
    struct Cos : Chainer< Cos >
    {
        /**
         * @brief Constructor.
         * @param x point of evaluation
         */
        explicit Cos( std::string x = "x" )
        {
            update( x );
        }

        /// Set point of evaluation.
        void update( const std::string& x )
        {
            this->x = addTexScope( forceAddScope( x ) );
        }

        /// Function value.
        std::string d0() const noexcept
        {
            return std::string( "\\cos" ).append( x );
        }

        /// First (directional) derivative.
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "-\\sin" ).append( x ).append( multiplyIfNotEmpty( dx ) );
        }

        /// Second (directional) derivative.
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-\\cos" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
        }

        /// Third (directional) derivative.
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( "\\sin" ).append( x ).append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    /*!
      @ingroup TexifyCMathGroup
      @brief Generate \f$ \cos\circ f \f$.
      @param f function mapping into a scalar space
      @return object of type MathematicalOperations::Chain<Cosc,Function>
     */
    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto cos( const Function& f )
    {
        return Cos()( f );
    }
}
