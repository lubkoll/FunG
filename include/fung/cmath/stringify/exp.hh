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
    @brief Exponential function including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function
    as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
    namespace stringify
    {
        struct Exp : Chainer< Exp >
        {
            //! @copydoc Cos::d0()
            explicit Exp( const String& x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = x;
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return String( "(e^" ).append( x ).append( ")" );
            }

            //! @copydoc Cos::d0()
            String d1( const String& dx = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d0()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d0()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return d0().append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        /*!
    @brief Function \f$2^x\f$ including first three derivatives.

    For scalar functions directional derivatives are less interesting. Incorporating this function
    as building block for more complex functions requires directional derivatives. These occur
    during applications of the chain rule.
   */
        struct Exp2 : Chainer< Exp2 >
        {
            //! @copydoc Cos::Cos()
            explicit Exp2( const String& x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = x;
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return IN_PARENS( String( "2^" ).append( x ) );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return IN_PARENS( String( "ln(2)*(2^" )
                                      .append( x )
                                      .append( ")" )
                                      .append( multiplyIfNotEmpty( dx ) ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return IN_PARENS( String( "(ln(2)^2)*(2^" )
                                      .append( x )
                                      .append( ")" )
                                      .append( multiplyIfNotEmpty( dx, dy ) ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return IN_PARENS( String( "(ln(2)^3)*(2^" )
                                      .append( x )
                                      .append( ")" )
                                      .append( multiplyIfNotEmpty( dx, dy, dz ) ) );
            }

        private:
            String x;
            String ln2{"ln(2)"};
        };

        /*!
    @brief Generate \f$ \exp(f) \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Exp,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto exp( const Function& f )
        {
            return Exp()( f );
        }

        /*!
    @brief Generate \f$2^f\f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Exp2,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto exp2( const Function& f )
        {
            return Exp2()( f );
        }
    }
    /** @} */
}
