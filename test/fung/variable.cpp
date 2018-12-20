#include <fung/fung.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Eq;

auto generateTestFunction()
{
    using namespace FunG;
    auto f = ( variable< 0 >( 1. ) + variable< 1 >( 2. ) ) * pow< 2 >( variable< 2 >( 3. ) );
    return finalize( f );
}

TEST( HasVariableTest, Sum )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) + 1 );
    EXPECT_TRUE( Checks::Has::variable< decltype( f ) >() );
    const auto g = finalize( 1 + variable< 0 >( 1. ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( g ) >() );
    const auto h = finalize( variable< 0 >( 1. ) + variable< 1 >( 2 ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( h ) >() );
    const auto k = Pow< 2, 1 >() + variable< 1 >( 1 );
    EXPECT_TRUE( Checks::Has::variable< decltype( k ) >() );
}

TEST( HasVariableTest, Scale )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) * 2 );
    EXPECT_TRUE( Checks::Has::variable< decltype( f ) >() );
    const auto g = finalize( 2 * variable< 0 >( 1. ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( g ) >() );
}

TEST( HasVariableTest, Multiply )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) * variable< 1 >( 2. ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( f ) >() );
    const auto g = finalize( FunG::Sin( 1 ) * variable< 0 >( 1. ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( g ) >() );
}

TEST( HasVariableTest, Squared )
{
    using namespace FunG;
    auto f = finalize( squared( variable< 0 >( 1. ) ) );
    EXPECT_TRUE( Checks::Has::variable< decltype( f ) >() );
}

TEST( VariableTypeTest, IF_applied_to_variable_THEN_RETURNS_its_type )
{
    using G = FunG::Variable< double, 1 >;
    const auto result = std::is_same< FunG::Variable_t< G, 1 >, double >::value;
    EXPECT_TRUE( result );
}

TEST( VariableTypeTest, IF_applied_to_const_variable_THEN_RETURNS_its_type )
{
    using G = const FunG::Variable< double, 1 >;
    const auto result = std::is_same< FunG::Variable_t< G, 1 >, double >::value;
    EXPECT_TRUE( result );
}

TEST( HasVariableIdTest, Sum )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) + 1 );
    auto result = Checks::Has::variableId< decltype( f ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( f ), 1 >();
    EXPECT_FALSE( result );
    const auto g = finalize( 1 + variable< 0 >( 1. ) );
    result = Checks::Has::variableId< decltype( g ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( g ), 1 >();
    EXPECT_FALSE( result );
    const auto h = finalize( variable< 0 >( 1. ) + variable< 1 >( 2 ) );
    result = Checks::Has::variableId< decltype( h ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( h ), 1 >();
    EXPECT_TRUE( result );
    const auto k = Pow< 2, 1 >() + variable< 1 >( 1 );
    result = Checks::Has::variableId< decltype( k ), 0 >();
    EXPECT_FALSE( result );
    result = Checks::Has::variableId< decltype( k ), 1 >();
    EXPECT_TRUE( result );
}

TEST( HasVariableIdTest, Scale )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) * 2 );
    auto result = Checks::Has::variableId< decltype( f ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( f ), 1 >();
    EXPECT_FALSE( result );
    const auto g = finalize( 2 * variable< 0 >( 1. ) );
    result = Checks::Has::variableId< decltype( g ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( g ), 1 >();
    EXPECT_FALSE( result );
}

TEST( HasVariableIdTest, Multiply )
{
    using namespace FunG;
    const auto f = finalize( variable< 0 >( 1. ) * variable< 1 >( 2. ) );
    auto result = Checks::Has::variableId< decltype( f ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( f ), 1 >();
    EXPECT_TRUE( result );
    const auto g = finalize( FunG::Sin( 1 ) * variable< 0 >( 1. ) );
    result = Checks::Has::variableId< decltype( g ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( g ), 1 >();
    EXPECT_FALSE( result );
}

TEST( HasVariableIdTest, Squared )
{
    using namespace FunG;
    const auto f = finalize( squared( variable< 0 >( 1. ) ) );
    auto result = Checks::Has::variableId< decltype( f ), 0 >();
    EXPECT_TRUE( result );
    result = Checks::Has::variableId< decltype( f ), 1 >();
    EXPECT_FALSE( result );
}

TEST( VariableTest, D0 )
{
    auto fun = generateTestFunction();
    EXPECT_DOUBLE_EQ( fun.d0(), 27. );
}

TEST( VariableTest, D1 )
{
    auto fun = generateTestFunction();
    EXPECT_DOUBLE_EQ( fun.d1< 0 >(), 9. );
    EXPECT_DOUBLE_EQ( fun.d1< 1 >(), 9. );
    EXPECT_DOUBLE_EQ( fun.d1< 2 >(), 18. );
}

TEST( VariableTest, D2 )
{
    auto fun = generateTestFunction();
    auto val = fun.d2< 0, 0 >();
    EXPECT_DOUBLE_EQ( val, 0. );
    val = fun.d2< 1, 1 >();
    EXPECT_DOUBLE_EQ( val, 0. );
    val = fun.d2< 2, 2 >();
    EXPECT_DOUBLE_EQ( val, 6. );
    val = fun.d2< 0, 1 >();
    EXPECT_DOUBLE_EQ( val, 0. );
    auto val2 = fun.d2< 1, 0 >();
    EXPECT_DOUBLE_EQ( val, val2 );
    val = fun.d2< 0, 2 >();
    EXPECT_DOUBLE_EQ( val, 6. );
    val2 = fun.d2< 2, 0 >();
    EXPECT_DOUBLE_EQ( val, val2 );
    val = fun.d2< 1, 2 >();
    EXPECT_DOUBLE_EQ( val, 6. );
    val2 = fun.d2< 2, 1 >();
    EXPECT_DOUBLE_EQ( val, val2 );
}

TEST( MaxVariableIdTest, IF_applied_to_variable_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MaxVariableIdTest, IF_applied_to_const_variable_THEN_RETURNS_its_id )
{
    using namespace FunG;
    const auto f = variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MinVariableIdTest, IF_applied_to_variable_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MinVariableIdTest, IF_applied_to_const_variable_THEN_RETURNS_its_id )
{
    using namespace FunG;
    const auto f = variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MaxVariableIdTest, IF_noVariablesInSum_THEN_RETURNS_lowest_int )
{
    using namespace FunG;
    auto f = finalize( Pow< 1, 2 >() + Pow< 2, 1 >() );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value,
                 Eq( std::numeric_limits< int >::lowest() ) );
}

TEST( MinVariableIdTest, IF_noVariablesInSum_THEN_RETURNS_max_int )
{
    using namespace FunG;
    auto f = finalize( Pow< 1, 2 >() + Pow< 2, 1 >() );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value,
                 Eq( std::numeric_limits< int >::max() ) );
}

TEST( MaxVariableIdTest, IF_oneVariableInSum_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = finalize( Pow< 2, 1 >() + variable< 1 >( 1 ) );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MinVariableIdTest, IF_oneVariableInSum_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = Pow< 2, 1 >() + variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MaxVariableIdTest, IF_twoVariablesInSum_THEN_RETURNS_the_bigger_id )
{
    using namespace FunG;
    auto f = variable< -3 >( 1 ) + variable< 2 >( 1 );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value, Eq( 2 ) );
}

TEST( MinVariableIdTest, IF_twoVariablesInSum_THEN_RETURNS_the_smaller_id )
{
    using namespace FunG;
    auto f = variable< -3 >( 1 ) + variable< 2 >( 1 );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value, Eq( -3 ) );
}

TEST( MaxVariableIdTest, IF_noVariablesInScale_THEN_RETURNS_lowest_int )
{
    using namespace FunG;
    auto f = finalize( 2 * Pow< 2, 1 >() );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value,
                 Eq( std::numeric_limits< int >::lowest() ) );
}

TEST( MinVariableIdTest, IF_noVariablesInScale_THEN_RETURNS_max_int )
{
    using namespace FunG;
    auto f = finalize( 2 * Pow< 2, 1 >() );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value,
                 Eq( std::numeric_limits< int >::max() ) );
}

TEST( MaxVariableIdTest, IF_oneVariableInScale_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = 2 * variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MaxVariableId< decltype( f ) >::value, Eq( 1 ) );
}

TEST( MinVariableIdTest, IF_oneVariableInScale_THEN_RETURNS_its_id )
{
    using namespace FunG;
    auto f = 2 * variable< 1 >( 1 );
    EXPECT_THAT( VariableDetail::MinVariableId< decltype( f ) >::value, Eq( 1 ) );
}

// TEST(VariableTest,D3)
//{
//  auto fun = generateTestFunction();
//  auto val = fun.d3<0,0,0>();
//  EXPECT_DOUBLE_EQ( val , 0. );
//  val = fun.d3<1,1,1>();
//  EXPECT_DOUBLE_EQ( val , 0. );
//  val = fun.d3<2,2,2>();
//  EXPECT_DOUBLE_EQ( val , 0. );
//  val = fun.d3<0,2,2>();
//  EXPECT_DOUBLE_EQ( val , 2. );
//  auto val2 = fun.d3<2,0,2>();
//  EXPECT_DOUBLE_EQ( val , val2 );
//  val2 = fun.d3<2,2,0>();
//  EXPECT_DOUBLE_EQ( val , val2 );
//  val = fun.d3<0,1,2>();
//  EXPECT_DOUBLE_EQ( val , 0. );
//  val2 = fun.d3<2,1,0>();
//  EXPECT_DOUBLE_EQ( val , val2 );
//  val = fun.d3<0,2,1>();
//  EXPECT_DOUBLE_EQ( val , 0. );
//  val2 = fun.d3<1,2,0>();
//  EXPECT_DOUBLE_EQ( val , val2 );
//  val = fun.d3<2,1,0>();
//  EXPECT_DOUBLE_EQ( val , val2 );
//}
