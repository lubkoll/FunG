// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

#include <type_traits>

namespace FunG
{
    /** @addtogroup StringifyCMathGroup
     *  @{ */

    /*!
      @brief Power function with rational exponent \f$ k = \frac{dividend}{divisor} \f$ including
      first three derivatives.

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
      For the cases \f$k=-1\f$ and \f$k=2\f$ specializations are used that avoid the use of
      std::pow.
     */
    namespace stringify
    {
        template < int dividend, int divisor = 1 >
        struct Pow : Chainer< Pow< dividend, divisor > >
        {
            //! @copydoc Cos::Cos()
            explicit Pow( String x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( String x )
            {
                this->x = addScope( x );
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return xtp( k );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return k * xtp( k1 ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return kk1 * xtp( k2 ).append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return kk1k2 * xtp( k3 ).append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String xtp( const String& k ) const
            {
                return String( x ).append( "^" ).append( addScope( k ) );
            }

            String x;
            const String k = toString( dividend ) / toString( divisor );
            const String k1 = toString( dividend - divisor ) / toString( divisor );
            const String kk1 =
                toString( dividend * ( dividend - divisor ) ) / toString( divisor * divisor );
            const String k2 = toString( dividend - 2 * divisor ) / toString( divisor );
            const String kk1k2 =
                toString( dividend * ( dividend - divisor ) * ( dividend - 2 * divisor ) ) /
                toString( divisor * divisor * divisor );
            const String k3 = toString( dividend - 3 * divisor ) / toString( divisor );
        };

        /// @cond
        template <>
        struct Pow< 1, 1 > : Chainer< Pow< 1, 1 > >
        {
            //! @copydoc Cos::Cos()
            explicit Pow( String x = "x" )
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
                return x;
            }

            //! @copydoc Cos::d1()
            String d1( String dx = "" ) const
            {
                return String( "1" ).append( multiplyIfNotEmpty( dx ) );
            }

        private:
            String x;
        };

        /// @cond
        template <>
        struct Pow< 2, 1 > : Chainer< Pow< 2, 1 > >
        {
            //! @copydoc Cos::Cos()
            explicit Pow( String x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = addScope( x );
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return String( x ).append( "^2" );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "2" ).append( x ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "2" ).append( multiplyIfNotEmpty( dx, dy ) );
            }

        private:
            String x;
        };

        /// @cond
        template <>
        struct Pow< 3, 1 > : Chainer< Pow< 3, 1 > >
        {
            //! @copydoc Cos::Cos()
            explicit Pow( const String& x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = addScope( x );
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return String( x ).append( "^3" );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "3" ).append( x ).append( "^2" ).append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "6" ).append( x ).append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return String( "6" ).append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        template < int dividend >
        struct Pow< dividend, 1 > : Chainer< Pow< dividend, 1 > >
        {
            //! @copydoc Cos::Cos()
            explicit Pow( const String& x = "x" )
            {
                update( x );
            }

            //! @copydoc Cos::update()
            void update( const String& x )
            {
                this->x = addScope( x );
            }

            //! @copydoc Cos::d0()
            String d0() const noexcept
            {
                return String( x ).append( "^" ).append( toString( dividend ) );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return toString( dividend )
                    .append( x )
                    .append( "^" )
                    .append( toString( dividend - 1 ) )
                    .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return toString( dividend * ( dividend - 1 ) )
                    .append( x )
                    .append( "^" )
                    .append( toString( dividend - 2 ) )
                    .append( multiplyIfNotEmpty( dx, dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                return toString( dividend * ( dividend - 1 ) * ( dividend - 2 ) )
                    .append( x )
                    .append( "^" )
                    .append( toString( dividend - 3 ) )
                    .append( multiplyIfNotEmpty( dx, dy, dz ) );
            }

        private:
            String x;
        };

        /*!
          @brief Generate \f$ \sqrt{f} \f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Sqrt,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto sqrt( const Function& f )
        {
            return Pow< 1, 2 >()( f );
        }

        /*!
          @brief Generate \f$ \sqrt[3]{f} \f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Cbrt,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto cbrt( const Function& f )
        {
            return Pow< 1, 3 >()( f );
        }

        /*!
          @brief Generate \f$ \sqrt[3]{f^2}\f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Cbrt2,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto cbrt2( const Function& f )
        {
            return Pow< 2, 3 >()( f );
        }

        /*!
          @brief Generate \f$ f^{k/l} \f$.
          @param f function mapping into a scalar space
          @tparam k dividend
          @tparam l divisor
          @return object of type MathematicalOperations::Chain< Pow<dividend,divisor> , Function >
         */
        template < int k, int l, class Function,
                   class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto pow( const Function& f )
        {
            return Pow< k, l >()( f );
        }

        /*!
          @brief Generate \f$ f^k,\ k\in\mathbb{N}\f$.
          @param f function mapping into a scalar space
          @tparam k exponent
          @return object of type MathematicalOperations::Chain< Pow<dividend,divisor> , Function >
         */
        template < int k, class Function,
                   class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto pow( const Function& f )
        {
            return Pow< k >()( f );
        }
    }
    /** @} */
}
