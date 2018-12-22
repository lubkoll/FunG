// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <texy/cmath/pow.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;

using ::texy::Pow;

TEST( TexifyPowSquaredTest, D0 )
{
    auto fun = Pow< 2 >();
    EXPECT_THAT( fun.d0(), Eq( "x^2" ) );
}

TEST( TexifyPowSquaredTest, D1 )
{
    auto fun = Pow< 2 >();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "2x*dx" ) );
}
TEST( TexifyPowSquaredTest, D2 )
{
    auto fun = Pow< 2 >();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "2*dx*dy" ) );
}

///
TEST( TexifyPowCubicTest, D0 )
{
    auto fun = Pow< 3 >();
    EXPECT_THAT( fun.d0(), Eq( "x^3" ) );
}

TEST( TexifyPowCubicTest, D1 )
{
    auto fun = Pow< 3 >();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "3x^2*dx" ) );
}

TEST( TexifyPowCubicTest, D2 )
{
    auto fun = Pow< 3 >();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "6x*dx*dy" ) );
}

TEST( TexifyPowCubicTest, D3 )
{
    auto fun = Pow< 3 >();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( "6*dx*dy*dz" ) );
}

TEST( TexifyPow4Test, D0 )
{
    auto fun = Pow< 4 >();
    EXPECT_THAT( fun.d0(), Eq( "x^4" ) );
}

TEST( TexifyPow4Test, D1 )
{
    auto fun = Pow< 4 >();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "4x^3*dx" ) );
}
TEST( TexifyPow4Test, D2 )
{
    auto fun = Pow< 4 >();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "12x^2*dx*dy" ) );
}

TEST( TexifyPow4Test, D3 )
{
    auto fun = Pow< 4 >();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( "24x*dx*dy*dz" ) );
}

TEST( TexifySqrtTest, D0 )
{
    auto fun = Pow< 1, 2 >();
    EXPECT_THAT( fun.d0(), Eq( "x^{1/2}" ) );
}

TEST( TexifySqrtTest, D1 )
{
    auto fun = Pow< 1, 2 >();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "1/2*x^{-1/2}*dx" ) );
}
TEST( TexifySqrtTest, D2 )
{
    auto fun = Pow< 1, 2 >();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "-1/4*x^{-3/2}*dx*dy" ) );
}

TEST( TexifySqrtTest, D3 )
{
    auto fun = Pow< 1, 2 >();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( "3/8*x^{-5/2}*dx*dy*dz" ) );
}
