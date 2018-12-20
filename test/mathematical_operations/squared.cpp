#include <fung/cmath/pow.hh>
#include <fung/cmath/stringify/pow.hh>
#include <fung/finalize.hh>
#include <fung/generate.hh>
#include <fung/variable.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;

auto StrEq = []( const auto& str ) { return Eq( str ); };

TEST( SquaredTest, Update )
{
    auto fun = squared( FunG::Pow< 2 >( 2. ) );
    EXPECT_NO_THROW( fun.update( -2 ) );
    EXPECT_DOUBLE_EQ( fun.d0(), 16. );
}

TEST( SquaredTest, UpdateVariable )
{
    auto x = FunG::variable< 0 >( 2. );
    auto fun = squared( FunG::Pow< 2, 1 >()( x ) );
    EXPECT_DOUBLE_EQ( fun.d0(), 16. );
    EXPECT_NO_THROW( fun.update< 0 >( -1. ) );
    EXPECT_NO_THROW( fun.update( 0 ); );
    EXPECT_DOUBLE_EQ( fun.d0(), 1. );
}

TEST( SquaredTest, D0 )
{
    using FunG::Pow;
    auto fun = squared( Pow< 2 >( 2. ) );
    EXPECT_DOUBLE_EQ( fun.d0(), 16. );
}

TEST( StringifySquaredTest, D0 )
{
    using FunG::stringify::Pow;
    auto f = squared( Pow< 2 >() );
    EXPECT_THAT( f.d0(), StrEq( "(x^2)*(x^2)" ) );
}

TEST( SquaredTest, D1 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( squared( Pow< 2 >( 2. ) ) );
    EXPECT_DOUBLE_EQ( fun.d1( 1 ), 32. );
}

TEST( StringifySquaredTest, D1 )
{
    using FunG::stringify::Pow;
    auto f = FunG::finalize( squared( Pow< 2 >() ) );
    EXPECT_THAT( f.d1( "" ), StrEq( "(2*(x^2))*2x" ) );
}

TEST( SquaredTest, D2 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( squared( Pow< 2 >( 2. ) ) );
    EXPECT_DOUBLE_EQ( fun.d2( 1, 1 ), 48. );
}

TEST( StringifySquaredTest, D2 )
{
    using FunG::stringify::Pow;
    auto f = FunG::finalize( squared( Pow< 2 >() ) );
    EXPECT_THAT( f.d2( "", "" ), StrEq( "2*((x^2)*2 + 2x*2x)" ) );
}

TEST( SquaredTest, D3 )
{
    using FunG::Pow;
    auto fun = FunG::finalize( squared( Pow< 2 >( 2. ) ) );
    EXPECT_DOUBLE_EQ( fun.d3( 1, 1, 1 ), 48. );
}

TEST( StringifySquaredTest, D3 )
{
    using FunG::stringify::Pow;
    auto f = FunG::finalize( squared( Pow< 2 >() ) );
    EXPECT_THAT( f.d3( "", "", "" ), StrEq( "2*(2x*2 + 2x*2 + 2*2x)" ) );
}
