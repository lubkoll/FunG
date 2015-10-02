#include <gtest/gtest.h>

#include "fung/cmath/pow.hh"
#include "fung/generate.hh"
#include "fung/finalize.hh"

TEST(SumTest,UpdateVariable)
{
  auto x = FunG::variable<0>(1.);
  using FunG::Pow;
  auto fun = ( Pow<3,1>(2.) + Pow<3,2>(1.) ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
  EXPECT_NO_THROW( fun.updateVariable<0>(4.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 72. );
}

TEST(SumTest,D0)
{
  using FunG::Pow;
  auto fun = Pow<3,1>(2.) + Pow<3,2>(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 9. );
  fun.update(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
}

TEST(SumTest,D1)
{
  using FunG::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 13.5 );
}

TEST(SumTest,D2)
{
  using FunG::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 12.75 );
}

TEST(SumTest,D3)
{
  using FunG::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 5.625 );
}

