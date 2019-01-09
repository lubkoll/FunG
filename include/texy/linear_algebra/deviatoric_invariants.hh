// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include "deviator.hh"
#include "frobenius_norm.hh"

namespace texy
{
    namespace LinearAlgebra
    {
        /**
         * @ingroup InvariantGroup
         * @brief Second deviatoric invariant \f$
         * j_2(\sigma)=\sqrt{\bar\sigma\negthinspace:\negthinspace\bar\sigma} \f$ with
         * \f$\bar\sigma = \sigma - \frac{\mathrm{tr}(\sigma)}{n}I\f$ and
         * \f$\sigma\in\mathbb{R}^{n,n}\f$.
         */
        template < int n >
        auto j2( const std::string& A )
        {
            return frobeniusNorm( A )( deviator< n >( A ) );
        }
    }
}
