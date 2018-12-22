#pragma once

#include <fung/cmath/arccos.hh>
#include <fung/util/static_checks.hh>
#include <stringy/util/string.hh>

#include <type_traits>

#include <string>

namespace stringy
{
    struct ACos : FunG::Chainer< ACos >
    {
        //! @copydoc Cos::Cos()
        explicit ACos( std::string x = "x" )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( const std::string& x )
        {
            this->x = addScope( x );
        }

        //! @copydoc Cos::d0()
        std::string d0() const noexcept
        {
            return std::string( "acos" ).append( x );
        }

        //! @copydoc Cos::d1()
        std::string d1( const std::string& dx = "" ) const
        {
            return std::string( "-1/sqrt(1-" )
                .append( x )
                .append( "^2)" )
                .append( multiplyIfNotEmpty( dx ) );
        }

        //! @copydoc Cos::d2()
        std::string d2( const std::string& dx = "", const std::string& dy = "" ) const
        {
            return std::string( "-" )
                .append( x )
                .append( "*(1" )
                .append( "-" )
                .append( x )
                .append( "^2)^(-3/2)" )
                .append( multiplyIfNotEmpty( dx ) )
                .append( multiplyIfNotEmpty( dy ) );
        }

        //! @copydoc Cos::d3()
        std::string d3( const std::string& dx = "", const std::string& dy = "",
                        const std::string& dz = "" ) const
        {
            auto first = std::string( "-1/(1-" ).append( x ).append( "^2)^(3/2) * " );
            const auto second =
                std::string( "(1 + 3*" ).append( x ).append( "^2" ).append( "/(1-x^2))" );
            return first.append( second )
                .append( multiplyIfNotEmpty( dx ) )
                .append( multiplyIfNotEmpty( dy ) )
                .append( multiplyIfNotEmpty( dz ) );
        }

    private:
        std::string x;
    };

    template < class Function, class = std::enable_if_t< FunG::Checks::isFunction< Function >() > >
    auto acos( const Function& f )
    {
        return ACos()( f );
    }
}
