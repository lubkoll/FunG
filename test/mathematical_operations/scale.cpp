#include <fung/cmath/pow.hh>
#include <fung/finalize.hh>
#include <fung/generate.hh>
#include <fung/variable.hh>
#include <stringy/cmath/pow.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;

auto StrEq = []( const auto& str ) { return Eq( str ); };

TEST( ScaleTest, Update )
{
    auto fun = 2 * FunG::Pow< 3, 1 >( 2. );
    EXPECT_NO_THROW( fun.update( -1 ) );
    EXPECT_DOUBLE_EQ( fun.d0(), -2. );
}

TEST( ScaleTest, UpdateVariable )
{
    auto x = FunG::variable< 0 >( 2. );
    auto fun = ( 2 * FunG::Pow< 3, 1 >() ) << x;
    EXPECT_DOUBLE_EQ( fun.d0(), 16. );
    EXPECT_NO_THROW( fun.update< 0 >( -1. ) );
    EXPECT_NO_THROW( fun.update( 0 ); );
    EXPECT_DOUBLE_EQ( fun.d0(), -2. );
}

TEST( ScaleTest, D0 )
{
    using FunG::Pow;
    const auto fun = 2 * Pow< 3, 1 >( 2. );
    EXPECT_DOUBLE_EQ( fun.d0(), 16. );
}

TEST( StringifyScaleTest, D0 )
{
    using stringy::Pow;
    const auto fun = 2 * Pow< 3, 1 >();
    EXPECT_THAT( fun.d0(), StrEq( "2*x^3" ) );
}

TEST( ScaleTest, D1 )
{
    using FunG::Pow;
    const auto fun = FunG::finalize( 2 * Pow< 3, 1 >( 2. ) );
    EXPECT_DOUBLE_EQ( fun.d1( 1 ), 24. );
}

TEST( StringifyScaleTest, D1 )
{
    using stringy::Pow;
    const auto fun = FunG::finalize( 2 * Pow< 3, 1 >() );
    EXPECT_THAT( fun.d1( "" ), StrEq( "2*3x^2" ) );
}

TEST( ScaleTest, D2 )
{
    using FunG::Pow;
    const auto fun = FunG::finalize( 2 * Pow< 3, 1 >( 2. ) );
    EXPECT_DOUBLE_EQ( fun.d2( 1, 1 ), 24. );
}

TEST( StringifyScaleTest, D2 )
{
    using stringy::Pow;
    const auto fun = FunG::finalize( 2 * Pow< 3, 1 >() );
    EXPECT_THAT( fun.d2( "", "" ), StrEq( "2*6x" ) );
}

TEST( ScaleTest, D3 )
{
    using FunG::Pow;
    const auto fun = FunG::finalize( 2 * Pow< 3, 1 >( 2. ) );
    EXPECT_DOUBLE_EQ( fun.d3( 1, 1, 1 ), 12. );
}

TEST( StringifyScaleTest, D3 )
{
    using stringy::Pow;
    auto fun = FunG::finalize( 2 * Pow< 3, 1 >() );
    EXPECT_THAT( fun.d3( "", "", "" ), StrEq( "2*6" ) );
}
