#pragma once

#include <fung/cmath/arccos.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/string.hh>

#include <type_traits>

#include <string>

namespace FunG
{
    namespace texify
    {
        struct ACos : Chainer< ACos >
        {
            //! @copydoc Cos::Cos()
            explicit ACos( std::string x = "x" )
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
                return std::string( "\\arccos" ).append( addTexScope( forceAddScope( x ) ) );
            }

            //! @copydoc Cos::d1()
            std::string d1( const std::string& dx = "" ) const
            {
                return std::string( "\\frac{-1}{\\sqrt{1-" )
                    .append( addScope( x ) )
                    .append( "^2}}" )
                    .append( multiplyIfNotEmpty( dx ) );
            }

            //! @copydoc Cos::d2()
            std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
            {
                return std::string( "-" )
                    .append( addScope( x ) )
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
                    std::string( "\\frac{-1}{(1-" ).append( addScope( x ) ).append( "^2)^{3/2}}*" );
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
            std::string x;
        };

        /*!
          @ingroup TexifyCMathGroup
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
