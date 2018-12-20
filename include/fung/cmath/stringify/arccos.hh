#pragma once

#include <fung/cmath/arccos.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

#include <type_traits>

#include <string>

namespace FunG
{
    namespace stringify
    {
        struct ACos : Chainer< ACos >
        {
            //! @copydoc Cos::Cos()
            explicit ACos( String x = "x" )
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
                return String( "acos" ).append( x );
            }

            //! @copydoc Cos::d1()
            String d1( const String& dx = "" ) const
            {
                return String( "-1/sqrt(1-" )
                    .append( x )
                    .append( "^2)" )
                    .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            String d2( const String& dx = "", const String& dy = "" ) const
            {
                return String( "-" ).append( x ).append( "*(1" ) -
                       String( x )
                           .append( "^2)^-3/2" )
                           .append( multiplyIfNotEmpty( dx ) )
                           .append( multiplyIfNotEmpty( dy ) );
            }

            //! @copydoc Cos::d3()
            String d3( const String& dx = "", const String& dy = "", const String& dz = "" ) const
            {
                auto first = String( "-1/(1-" ).append( x ).append( "^2)^(3/2) * " );
                const auto second =
                    String( "(1 + 3*" ).append( x ).append( "^2" ).append( "/(1-x^2))" );
                return first.append( second )
                    .append( multiplyIfNotEmpty( dx ) )
                    .append( multiplyIfNotEmpty( dy ) )
                    .append( multiplyIfNotEmpty( dz ) );
            }

        private:
            String x;
        };

        /*!
          @ingroup StringifyCMathGroup
          @brief Generate printable version of \f$ \arccos\circ f \f$.
          @param f function mapping into a scalar space
          @return object of type MathematicalOperations::Chain<Printable<ACos>,Function>
         */
        template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
        auto acos( const Function& f )
        {
            return ACos()( f );
        }
    }
}
