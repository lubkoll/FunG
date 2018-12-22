#pragma once

#include <fung/concept_check.hh>
#include <fung/util/chainer.hh>
#include <fung/util/type_traits.hh>
#include <texy/util/string.hh>

#include <type_traits>
#include <utility>

namespace texy
{
    namespace LinearAlgebra
    {
        /** @addtogroup TexifyLinearAlgebraGroup
         *  @{ */
        /// %Trace of a matrix (sum of diagonal elements).
        struct Trace : FunG::Chainer< Trace >
        {
            Trace() = default;

            explicit Trace( const std::string& A = "A" )
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
                return d1( A );
            }

            /// First directional derivative.
            std::string d1( const std::string& dA ) const
            {
                return std::string( "\\mathrm{tr}" ).append( forceAddScope( dA ) );
            }

        private:
            std::string A;
        };

        inline auto trace( const std::string& A )
        {
            return Trace( A );
        }

        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto trace( const F& f )
        {
            return Trace( f() )( f );
        }
        /** @} */
    }
}
