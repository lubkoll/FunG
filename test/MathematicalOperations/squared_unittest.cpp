#include "../../RFFGen/CMath/pow.hh"
#include "../../RFFGen/generate.hh"
#include "../../RFFGen/finalize.hh"
#include "../../RFFGen/variable.hh"

#include <gtest/gtest.h>

TEST(SquaredTest,Update)
{
  auto fun = RFFGen::CMath::Pow<2>(2.)^2;
  EXPECT_NO_THROW( fun.update(-2) );
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(SquaredTest,UpdateVariable)
{
  auto x = RFFGen::variable<0>(2.);
  auto fun = RFFGen::CMath::Pow<2,1>()(x)^2;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
  EXPECT_NO_THROW( fun.updateVariable<0>(-1.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 1. );
}

TEST(SquaredTest,D0)
{
  using RFFGen::CMath::Pow;
  auto fun = Pow<2>(2.)^2;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(SquaredTest,D1)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar ( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d1() , 32. );
}

TEST(SquaredTest,D2)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d2() , 48. );
}

TEST(SquaredTest,D3)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( Pow<2>(2.)^2 );
  EXPECT_DOUBLE_EQ( fun.d3() , 48. );
}



