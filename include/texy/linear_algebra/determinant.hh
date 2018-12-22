// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include "cofactor.hh"
#include "trace.hh"
#include "transpose.hh"

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/type_traits.hh>
#include <texy/constant.hh>
#include <texy/util/string.hh>

#include <type_traits>
#include <utility>

namespace texy
{
    namespace LinearAlgebra
    {
        /** @addtogroup TexifyLinearAlgebraGroup
         *  @{ */
        class Determinant : public FunG::Chainer< Determinant >
        {
        public:
            Determinant() = default;

            /// Constructor.
            Determinant( const std::string& A = "A" ) : A( A )
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
                return std::string( "\\det" ).append( forceAddScope( A ) );
            }

            /// First (directional) derivative.
            std::string d1( const std::string& dA ) const
            {
                return trace( transpose( cof( A ) )().append( "*" ).append( dA ) )();
            }

            /// Second (directional) derivative.
            std::string d2( const std::string& dA, const std::string& dB ) const
            {
                return trace( transpose( cof( A ).d1( dB ) )().append( "*" ).append( dA ) )();
            }

            /// Third (directional) derivative.
            std::string d3( const std::string& dA, const std::string& dB,
                            const std::string& dC ) const
            {
                return trace( transpose( cof( A ).d2( dB, dC ) )().append( "*" ).append( dA ) )();
            }

        private:
            std::string A;
        };

        /**
         * @brief Generate \f$\det(A)\f$.
         * @param A square matrix
         * @return Determinant(A)
         */
        inline auto det( const std::string& A )
        {
            return Determinant( A );
        }

        /**
         * @brief Generate \f$\det\circ f\f$.
         * @param f function mapping into a space of square matrices
         * @return Determinant< std::decay_t<decltype(f.d0())> >(f.d0())(f)
         */
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto det( const F& f )
        {
            return Determinant( f.d0() )( f );
        }
        /** @} */
    }
}
