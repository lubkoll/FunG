// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <fung/texify/cmath/cosine.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using FunG::texify::Cos;

TEST( TexifyCosineTest, D0 )
{
    const auto fun = Cos();
    EXPECT_THAT( fun.d0(), Eq( R"(\cos{(x)})" ) );
}

TEST( TexifyCosineTest, D1 )
{
    const auto fun = Cos();
    EXPECT_THAT( fun.d1( "dx" ), Eq( R"(-\sin{(x)}*dx)" ) );
}

TEST( TexifyCosineTest, D2 )
{
    const auto fun = Cos();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( R"(-\cos{(x)}*dx*dy)" ) );
}

TEST( TexifyCosineTest, D3 )
{
    const auto fun = Cos();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( R"(\sin{(x)}*dx*dy*dz)" ) );
}
