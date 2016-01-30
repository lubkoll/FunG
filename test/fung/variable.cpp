#include <gtest/gtest.h>
#include "fung/fung.hh"

auto generateTestFunction()
{
  using namespace FunG;
  auto f = ( variable<0>(1.) + variable<1>(2.) ) * pow<2>(variable<2>(3.));
  return finalize(f);
}

TEST(VariableTest,D0)
{
  auto fun = generateTestFunction();
  EXPECT_DOUBLE_EQ( fun.d0() ,  27. );
}

TEST(VariableTest,D1)
{
  auto fun = generateTestFunction();
  EXPECT_DOUBLE_EQ( fun.d1<0>() , 9. );
  EXPECT_DOUBLE_EQ( fun.d1<1>() , 9. );
  EXPECT_DOUBLE_EQ( fun.d1<2>() , 18. );
}

TEST(VariableTest,D2)
{
  auto fun = generateTestFunction();
  auto val = fun.d2<0,0>();
  EXPECT_DOUBLE_EQ( val , 0. );
  val = fun.d2<1,1>();
  EXPECT_DOUBLE_EQ( val , 0. );
  val = fun.d2<2,2>();
  EXPECT_DOUBLE_EQ( val , 6. );
  val = fun.d2<0,1>();
  EXPECT_DOUBLE_EQ( val , 0. );
  auto val2 = fun.d2<1,0>();
  EXPECT_DOUBLE_EQ( val, val2 );
  val = fun.d2<0,2>();
  EXPECT_DOUBLE_EQ( val , 6. );
  val2 = fun.d2<2,0>();
  EXPECT_DOUBLE_EQ( val , val2 );
  val = fun.d2<1,2>();
  EXPECT_DOUBLE_EQ( val , 6. );
  val2 = fun.d2<2,1>();
  EXPECT_DOUBLE_EQ( val , val2 );
}

//TEST(VariableTest,D3)
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
