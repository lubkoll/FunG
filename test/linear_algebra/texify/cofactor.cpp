// Copyright (C) 2016 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <texy/linear_algebra/cofactor.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using texy::LinearAlgebra::cof;

TEST( TexifyCofactorTest, D0 )
{
    const auto f = cof( "A" );
    EXPECT_THAT( f(), Eq( "\\mathrm{cof}(A)" ) );
}

TEST( TexifyCofactorTest, D1 )
{
    const auto f = cof( "A" );
    EXPECT_THAT( f.d1( "dA" ), Eq( "\\mathrm{cof}^{(1)}(A)(dA)" ) );
}

TEST( TexifyCofactorTest, D2 )
{
    const auto f = cof( "A" );
    EXPECT_THAT( f.d2( "dA", "dB" ), Eq( "\\mathrm{cof}^{(2)}(A)(dA,dB)" ) );
}
