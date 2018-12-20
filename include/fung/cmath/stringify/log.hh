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

    /**
   * @brief Natural logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function
   * as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
    namespace stringify
    {
        struct LN : Chainer< LN >
        {
            //! @copydoc Cos::Cos()
            explicit LN( const String& x = "x" )
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
                return String( "ln(" ).append( x ).append( ")" );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "(x^-1)" ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-(x^-2)" ).append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "2(x^-3)" ).append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        /**
   * @brief Common (base 10) logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function
   * as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
        struct Log10 : Chainer< Log10 >
        {
            //! @copydoc Cos::Cos()
            explicit Log10( const String& x = "x" )
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
                return String( "log_10(" ).append( x ).append( ")" );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return ( ln10 * "x" ).append( "^(-1)" ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-" )
                    .append( ln10 )
                    .append( "^(-1)" )
                    .append( x )
                    .append( "^(-2)" )
                    .append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "2" )
                    .append( ln10 )
                    .append( "^(-1)" )
                    .append( x )
                    .append( "^(-3)" )
                    .append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
            String ln10{"ln(10)"};
        };

        /**
   * @brief %Base 2 logarithm including first three derivatives.
   *
   * For scalar functions directional derivatives are less interesting. Incorporating this function
   * as building block for more complex functions requires directional derivatives. These occur
   * during applications of the chain rule.
   */
        struct Log2 : Chainer< Log2 >
        {
            //! @copydoc Cos::Cos()
            explicit Log2( const String& x = 1. )
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
                return String( "log_2" ).append( x );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "(" )
                    .append( ln2 )
                    .append( "*" )
                    .append( x )
                    .append( ")^(-1)" )
                    .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-" )
                    .append( ln2 )
                    .append( "^(-1)" )
                    .append( x )
                    .append( "^(-2)" )
                    .append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "2" )
                    .append( ln2 )
                    .append( "^(-1)" )
                    .append( x )
                    .append( "^(-3)" )
                    .append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
            String ln2{"ln(2)"};
        };

        /*!
    @brief Generate \f$ \mathrm{ln}\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Log,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto ln( const Function& f )
        {
            return LN()( f );
        }

        /*!
    @brief Generate \f$ \mathrm{log}_{10}\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Log10,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto log10( const Function& f )
        {
            return Log10()( f );
        }

        /*!
    @brief Generate \f$ \mathrm{log}_{2}\circ f \f$.
    @param f function mapping into a scalar space
    @return object of type MathematicalOperations::Chain<Log2,Function>
   */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto log2( const Function& f )
        {
            return Log2()( f );
        }
    }
    /** @} */
}
