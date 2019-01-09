// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <texy/util/chainer.hh>
#include <texy/util/string.hh>
#include <fung/util/static_checks.hh>

namespace texy
{
    namespace LinearAlgebra
    {
        /** @addtogroup TexifyLinearAlgebraGroup
         *  @{ */
        class Cofactor : public Chainer< Cofactor >
        {
        public:
            Cofactor() = default;

            /// Constructor.
            Cofactor( const std::string& A = "A" ) : A( A )
            {
                update( A );
            }

            /// Reset point of evaluation.
            void update( const std::string& A )
            {
                this->A = A;
            }

            /// Function value.
            std::string d0() const
            {
                return std::string( "\\mathrm{cof}" ).append( forceAddScope( A ) );
            }

            /// First (directional) derivative.
            std::string d1( const std::string& dA ) const
            {
                return std::string( "\\mathrm{cof}^{(1)}" )
                    .append( forceAddScope( A ) )
                    .append( forceAddScope( dA ) );
            }

            /// Second (directional) derivative.
            auto d2( const std::string& dA, const std::string& dB ) const
            {
                return std::string( "\\mathrm{cof}^{(2)}" )
                    .append( forceAddScope( A ) )
                    .append( forceAddScope( std::string( dA ).append( "," ).append( dB ) ) );
            }

            /// Third (directional) derivative.
            auto d3( const std::string& dA, const std::string& dB, const std::string& dC ) const
            {
                return std::string( "\\mathrm{cof}^{(3)}" )
                    .append( forceAddScope( A ) )
                    .append( forceAddScope(
                        std::string( dA ).append( "," ).append( dB ).append( "," ).append( dC ) ) );
            }

        private:
            std::string A;
        };

        inline auto cof( const std::string& A )
        {
            return Cofactor( A );
        }
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto cof( const F& f )
        {
            return Cofactor( f.d0() )( f );
        }
        /** @} */
    }
}
