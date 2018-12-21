#include <fung/cmath/pow.hh>
#include <fung/cmath/stringify/pow.hh>
#include <fung/cmath/texify/pow.hh>
#include <fung/finalize.hh>
#include <fung/generate.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;

auto StrEq = []( const auto& str ) { return Eq( str ); };

TEST( SumTest, UpdateVariable )
{
    auto x = FunG::variable< 0 >( 1. );
    using FunG::Pow;
    auto fun = ( Pow< 3, 1 >( 2. ) + Pow< 3, 2 >( 1. ) ) << x;
    EXPECT_DOUBLE_EQ( fun.d0(), 2. );
    EXPECT_NO_THROW( fun.update< 0 >( 4. ) );
    EXPECT_NO_THROW( fun.update( 0 ); );
    EXPECT_DOUBLE_EQ( fun.d0(), 72. );
}

TEST( SumTest, D0 )
{
    using FunG::Pow;
    auto fun = Pow< 3, 1 >( 2. ) + Pow< 3, 2 >( 1. );
    EXPECT_DOUBLE_EQ( fun.d0(), 9. );
    fun.update( 1. );
    EXPECT_DOUBLE_EQ( fun.d0(), 2. );
}

TEST( StringifySumTest, D0 )
{
    using FunG::stringify::Pow;
    auto fun = Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" );
    EXPECT_THAT( fun.d0(), StrEq( "x^3 + y^(3/2)" ) );
}

TEST( TexifySumTest, D0 )
{
    using FunG::texify::Pow;
    auto fun = Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" );
    EXPECT_THAT( fun.d0(), StrEq( "x^3 + y^{3/2}" ) );
}

TEST( SumTest, D1 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( 2. ) + Pow< 3, 2 >( 1. ) );
    EXPECT_DOUBLE_EQ( fun.d1( 1 ), 13.5 );
}

TEST( StringifySumTest, D1 )
{
    using FunG::stringify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d1( "" ), StrEq( "3x^2 + 3/2*y^(1/2)" ) );
}

TEST( TexifySumTest, D1 )
{
    using FunG::texify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d1( "" ), StrEq( "3x^2 + 3/2*y^{1/2}" ) );
}

TEST( SumTest, D2 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( 2. ) + Pow< 3, 2 >( 1. ) );
    EXPECT_DOUBLE_EQ( fun.d2( 1, 1 ), 12.75 );
}

TEST( StringifySumTest, D2 )
{
    using FunG::stringify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d2( "", "" ), StrEq( "6x + 3/4*y^(-1/2)" ) );
}

TEST( TexifySumTest, D2 )
{
    using FunG::texify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d2( "", "" ), StrEq( "6x + 3/4*y^{-1/2}" ) );
}

TEST( SumTest, D3 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( 2. ) + Pow< 3, 2 >( 1. ) );
    EXPECT_DOUBLE_EQ( fun.d3( 1, 1, 1 ), 5.625 );
}

TEST( StringifySumTest, D3 )
{
    using FunG::stringify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d3( "", "", "" ), StrEq( "6 + -3/8*y^(-3/2)" ) );
}

TEST( TexifySumTest, D3 )
{
    using FunG::texify::Pow;
    auto fun = FunG::finalize( Pow< 3, 1 >( "x" ) + Pow< 3, 2 >( "y" ) );
    EXPECT_THAT( fun.d3( "", "", "" ), StrEq( "6 + -3/8*y^{-3/2}" ) );
}
