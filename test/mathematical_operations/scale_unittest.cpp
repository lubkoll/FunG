#include <gtest/gtest.h>

#include "fung/cmath/pow.hh"
#include "fung/generate.hh"
#include "fung/finalize.hh"
#include "fung/variable.hh"

TEST(ScaleTest,Update)
{
  auto fun = 2*FunG::Pow<3,1>(2.);
  EXPECT_NO_THROW( fun.update(-1) );
  EXPECT_DOUBLE_EQ( fun.d0() , -2. );
}

TEST(ScaleTest,UpdateVariable)
{
  auto x = FunG::variable<0>(2.);
  auto fun = ( 2*FunG::Pow<3,1>() ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
  EXPECT_NO_THROW( fun.update<0>(-1.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , -2. );
}

TEST(ScaleTest,D0)
{
  using FunG::Pow;
  auto fun = 2*Pow<3,1>(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(ScaleTest,D1)
{
  using FunG::Pow;
  auto fun = FunG::finalize ( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d1(1) , 24. );
}

TEST(ScaleTest,D2)
{
  using FunG::Pow;
  auto fun = FunG::finalize( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d2(1,1) , 24. );
}

TEST(ScaleTest,D3)
{
  using FunG::Pow;
  auto a = 0;
  auto fun = FunG::finalize( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d3(1,1,1) , 12. );
}


