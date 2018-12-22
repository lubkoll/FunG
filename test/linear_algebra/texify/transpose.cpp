#include <texy/linear_algebra/transpose.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;
using texy::LinearAlgebra::Transpose;

TEST( TexifyTransposeTest, D0 )
{
    const auto f = Transpose( "B" );
    EXPECT_THAT( f(), Eq( "B^T" ) );
}

TEST( TexifyTransposeTest, D1 )
{
    const auto f = Transpose( "B" );
    EXPECT_THAT( f.d1( "dC" ), Eq( "dC^T" ) );
}
