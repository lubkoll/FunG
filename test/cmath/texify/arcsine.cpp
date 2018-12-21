// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <fung/texify/cmath/arcsine.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using FunG::texify::ASin;

TEST( TexifyArcsineTest, D0 )
{
    const auto fun = ASin();
    EXPECT_THAT( fun.d0(), Eq( "\\arcsin{(x)}" ) );
}

TEST( TexifyArcsineTest, D1 )
{
    const auto fun = ASin();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "\\frac{1}{\\sqrt{1-x^2}}*dx" ) );
}

TEST( TexifyArcsineTest, D2 )
{
    const auto fun = ASin();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "x*(1-x^2)^{-3/2}*dx*dy" ) );
}

TEST( TexifyArcsineTest, D3 )
{
    const auto fun = ASin();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ),
                 Eq( "\\frac{1}{(1-x^2)^{3/2}}*(1 + \\frac{3*x^2}{1-x^2})*dx*dy*dz" ) );
}
