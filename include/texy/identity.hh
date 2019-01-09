// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <texy/util/chainer.hh>

#include <string>

namespace texy
{
    /// %Identity mapping \f$ f(x)=x \f$.
    struct Identity : Chainer< Identity >
    {
        /// Default constructor.
        Identity() = default;

        /**
         * @brief Constructor.
         * @param x point of evaluation.
         */
        Identity( const std::string& x )
        {
            update( x );
        }

        /// Reset point of evaluation
        void update( const std::string& x )
        {
            x_ = x;
        }

        /// Function value.
        const std::string& d0() const noexcept
        {
            return x_;
        }

        /// First directional derivative.
        template < int >
        const std::string& d1( const std::string& dx ) const noexcept
        {
            return dx;
        }

    private:
        std::string x_;
    };

    /// @return Identity(x).
    auto identity( const std::string& x )
    {
        return Identity( x );
    }
}
