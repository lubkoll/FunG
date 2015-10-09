#include <gtest/gtest.h>

#include "fung/cmath/pow.hh"
#include "fung/generate.hh"
#include "fung/finalize.hh"
#include "fung/variable.hh"

TEST(SquaredTest,Update)
{
  auto fun = FunG::Pow<2>(2.)^2;
  EXPECT_NO_THROW( fun.update(-2) );
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(SquaredTest,UpdateVariable)
{
  auto x = FunG::variable<0>(2.);
  auto fun = FunG::Pow<2,1>()(x)^2;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
  EXPECT_NO_THROW( fun.update<0>(-1.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 1. );
}

TEST(SquaredTest,D0)
{
  using FunG::Pow;
  auto fun = Pow<2>(2.)^2;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(SquaredTest,D1)
{
  using FunG::Pow;
  auto fun = FunG::finalize ( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d1(1) , 32. );
}

TEST(SquaredTest,D2)
{
  using FunG::Pow;
  auto fun = FunG::finalize( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d2(1,1) , 48. );
}

TEST(SquaredTest,D3)
{
  using FunG::Pow;
  auto fun = FunG::finalize( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d3(1,1,1) , 48. );
}



