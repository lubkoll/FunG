// Copyright (C) 2019 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include "trace.hh"
#include <texy/constant.hh>
#include <texy/generate.hh>
#include <texy/identity.hh>
#include <fung/util/static_checks.hh>

#include <type_traits>

namespace texy
{
    /// @cond
    namespace Concepts
    {
        template < class >
        struct SquareMatrixConceptChecks;
    }
    /// @endcond

    namespace LinearAlgebra
    {
        /** @addtogroup TexifyLinearAlgebraGroup
         *  @{ */

        /// Generate %deviator \f$ \mathrm{dev}(A) = A - \frac{\mathrm{tr}(A)}{n}I \f$ of a matrix
        /// \f$ A\in\mathbb{R}^{n,n} \f$.
        template < int n >
        auto deviator( const std::string& A )
        {
            return identity( A ) + ( -1. / n ) * ( trace( A ) * constant( "I" ) );
        }

        /// Generate %deviator \f$ \mathrm{dev}\circ f\f$.
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto deviator( const F& f )
        {
            return deviator( f() )( f );
        }

        /** @} */
    }
}
