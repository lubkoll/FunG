// Copyright (C) 2019 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <texy/util/chainer.hh>
#include <texy/util/string.hh>
#include <fung/util/static_checks.hh>

/*!
  @ingroup TexifyCMathGroup

  @brief Sine function including first three derivatives (based on sin(double) in \<cmath\>).

  For scalar functions directional derivatives are less interesting. Incorporating this function
  as building block for more complex functions requires directional derivatives. These occur
  during applications of the chain rule.
 */
namespace texy
{
    struct Sin : Chainer< Sin >
    {
        //! @copydoc Cos::Cos()
        explicit Sin( std::string x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addTexScope( forceAddScope( x ) );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( R"(\sin)" ).append( x );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( R"(\cos)" ).append( x ).append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( R"(-\sin)" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            return std::string( R"(-\cos)" ).append( x ).append( multiplyIfNotEmpty( dx, dy, dz ) );
        }

    private:
        std::string x;
    };

    /*!
      @ingroup TexifyCMathGroup
      @brief Generate \f$ \sin\circ f \f$.
      @param f function mapping into a scalar space
      @return object of type MathematicalOperations::Chain<Sin,Function>
     */
    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto sin( const Function& f )
    {
        return Sin()( f );
    }
}
