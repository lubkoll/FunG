// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <fung/constant.hh>
#include <fung/mathematical_operations/scale.hh>
#include <fung/util/chainer.hh>

namespace funcy
{
    constexpr bool isProductPositive( int a, int b )
    {
        return a * b > 0;
    }

    template < int dividend, int divisor = 1,
               bool isPositive = isProductPositive( dividend, divisor ) >
    struct Pow;

    template <>
    struct Pow< 1, 1, true > : FunG::Chainer< Pow< 1, 1, true > >
    {
        constexpr explicit Pow( double x ) : x( x )
        {
        }

        void update( double x ) noexcept
        {
            this->x = x;
        }

        constexpr double operator()() const noexcept
        {
            return x;
        }

        constexpr auto d1( double dx ) const noexcept
        {
            return FunG::constant( dx );
        }

    private:
        double x;
    };

    template < int k >
    struct Pow< k, 1, true > : FunG::Chainer< Pow< k, 1, true > >
    {
        constexpr explicit Pow( double x ) : x( x )
        {
        }

        void update( double x ) noexcept
        {
            this->x = x;
        }

        constexpr double d0( double x ) const noexcept
        {
            return Pow< 1 >()( x ) * Pow< k - 1 >( x )();
        }

        constexpr auto d1( double dx ) const noexcept
        {
            return FunG::MathematicalOperations::Scale< int, Pow< k - 1 > >( k, x ) *
                   FunG::constant( dx );
        }

    private:
        double x;
    };
}
