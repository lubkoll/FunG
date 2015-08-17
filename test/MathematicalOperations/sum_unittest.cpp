#include "../../FunG/FunG/CMath/pow.hh"
#include "../../FunG/FunG/generate.hh"
#include "../../FunG/FunG/finalize.hh"

#include <gtest/gtest.h>

TEST(SumTest,UpdateVariable)
{
  auto x = FunG::variable<0>(1.);
  using FunG::CMath::Pow;
  auto fun = ( Pow<3,1>(2.) + Pow<3,2>(1.) ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
  EXPECT_NO_THROW( fun.updateVariable<0>(4.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 72. );
}

TEST(SumTest,D0)
{
  using FunG::CMath::Pow;
  auto fun = Pow<3,1>(2.) + Pow<3,2>(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 9. );
  fun.update(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
}

TEST(SumTest,D1)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 13.5 );
}

TEST(SumTest,D2)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 12.75 );
}

TEST(SumTest,D3)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 5.625 );
}

