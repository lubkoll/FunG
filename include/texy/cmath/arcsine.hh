// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <texy/util/string.hh>

/*!
  @ingroup TexifyCMathGroup

  @brief Arc sine function including first three derivatives (based on asin(std::string) in
  \<cmath\>).

  For scalar functions directional derivatives are less interesting. Incorporating this function
  as building block for more complex functions requires directional derivatives. These occur
  during applications of the chain rule.
 */
namespace texy
{
    struct ASin : FunG::Chainer< ASin >
    {
        //! @copydoc Cos::Cos()
        explicit ASin( const std::string& x = "x" )
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
            return std::string( "\\arcsin" ).append( addTexScope( forceAddScope( x ) ) );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "\\frac{1}{\\sqrt{1-" )
                .append( addScope( x ) )
                .append( "^2}}" )
                .append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return addScope( x )
                .append( "*(1" )
                .append( "-" )
                .append( addScope( x ) )
                .append( "^2)^{-3/2}" )
                .append( multiplyIfNotEmpty( dx ) )
                .append( multiplyIfNotEmpty( dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            auto first =
                std::string( "\\frac{1}{(1-" ).append( addScope( x ) ).append( "^2)^{3/2}}*" );
            const auto second = std::string( "(1 + \\frac{3*" )
                                    .append( addScope( x ) )
                                    .append( "^2" )
                                    .append( "}{1-x^2})" );
            return first.append( second )
                .append( multiplyIfNotEmpty( dx ) )
                .append( multiplyIfNotEmpty( dy ) )
                .append( multiplyIfNotEmpty( dz ) );
        }

    private:
        std::string d1p3() const
        {
            return std::string( "(1-" ).append( addScope( x ) ).append( "^2" ).append( "^(-3/2)" );
        }

        std::string x;
    };

    /*!
      @ingroup TexifyCMathGroup
      @brief Generate \f$ \arcsin\circ f \f$.
      @param f function mapping into a scalar space
      @return object of type MathematicalOperations::Chain<ASin,Function>
     */
    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto asin( const Function& f )
    {
        return ASin()( f );
    }
}
