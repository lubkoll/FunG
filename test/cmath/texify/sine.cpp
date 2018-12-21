// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <fung/texify/cmath/sine.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using FunG::texify::Sin;

TEST( TexifySineTest, D0 )
{
    const auto fun = Sin();
    EXPECT_THAT( fun.d0(), Eq( R"(\sin{(x)})" ) );
}

TEST( TexifySineTest, D1 )
{
    const auto fun = Sin();
    EXPECT_THAT( fun.d1( "dx" ), Eq( R"(\cos{(x)}*dx)" ) );
}

TEST( TexifySineTest, D2 )
{
    const auto fun = Sin();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( R"(-\sin{(x)}*dx*dy)" ) );
}

TEST( TexifySineTest, D3 )
{
    const auto fun = Sin();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( R"(-\cos{(x)}*dx*dy*dz)" ) );
}
