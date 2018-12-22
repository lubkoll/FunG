// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/concept_check.hh>
#include <fung/util/chainer.hh>
#include <texy/util/string.hh>

#include <type_traits>

namespace texy
{
    namespace LinearAlgebra
    {
        /** @addtogroup TexifyLinearAlgebraGroup
         *  @{ */
        /// Represents transposition matrices.
        class Transpose : public FunG::Chainer< Transpose >
        {
        public:
            explicit Transpose( const std::string& A ) : A( A )
            {
            }

            void update( const std::string& A )
            {
                this->A = A;
            }

            std::string d0() const noexcept
            {
                return addStrictScope( A ).append( "^T" );
            }

            std::string d1( const std::string& dA ) const
            {
                return addStrictScope( dA ).append( "^T" );
            }

        private:
            std::string A;
        };

        inline auto transpose( const std::string& A )
        {
            return Transpose( A );
        }

        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto transpose( const F& f )
        {
            return Transpose( f() )( f );
        }
        /** @} */
    }
}
