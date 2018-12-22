// Copyright (C) 2016 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <texy/linear_algebra/determinant.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using texy::LinearAlgebra::det;

TEST( TexifyDeterminantTest, D0 )
{
    const auto f = det( "A" );
    EXPECT_THAT( f(), Eq( "\\det(A)" ) );
}

TEST( TexifyDeterminantTest, D1 )
{
    const auto f = det( "A" );
    EXPECT_THAT( f.d1( "dA" ), Eq( "\\mathrm{tr}((\\mathrm{cof}(A))^T*dA)" ) );
}

TEST( TexifyDeterminantTest, D2 )
{
    const auto f = det( "A" );
    EXPECT_THAT( f.d2( "dA", "dB" ), Eq( "\\mathrm{tr}((\\mathrm{cof}^{(1)}(A)(dB))^T*dA)" ) );
}

TEST( TexifyDeterminantTest, D3 )
{
    const auto f = det( "A" );
    EXPECT_THAT( f.d3( "dA", "dB", "dC" ),
                 Eq( "\\mathrm{tr}((\\mathrm{cof}^{(2)}(A)(dB,dC))^T*dA)" ) );
}
