// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <fung/texify/cmath/tan.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using FunG::texify::Tan;

TEST( TexifyTanTest, D0 )
{
    const auto fun = Tan();
    EXPECT_THAT( fun.d0(), Eq( "\\tan(x)" ) );
}

TEST( TexifyTanTest, D1 )
{
    const auto fun = Tan();
    EXPECT_THAT( fun.d1( "dx" ), Eq( "(1+\\tan^2(x))*dx" ) );
}

TEST( TexifyTanTest, D2 )
{
    const auto fun = Tan();
    EXPECT_THAT( fun.d2( "dx", "dy" ), Eq( "2*\\tan(x)*(1+\\tan^2(x))*dx*dy" ) );
}

TEST( TexifyTanTest, D3 )
{
    const auto fun = Tan();
    EXPECT_THAT( fun.d3( "dx", "dy", "dz" ), Eq( "2*(1+\\tan^2(x))*(1 + 3*\\tan^2(x))*dx*dy*dz" ) );
}
