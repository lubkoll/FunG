#include "../../FunG/FunG/CMath/pow.hh"
#include "../../FunG/FunG/generate.hh"
#include "../../FunG/FunG/finalize.hh"
#include "../../FunG/FunG/variable.hh"

#include <gtest/gtest.h>

TEST(ScaleTest,Update)
{
  auto fun = 2*FunG::CMath::Pow<3,1>(2.);
  EXPECT_NO_THROW( fun.update(-1) );
  EXPECT_DOUBLE_EQ( fun.d0() , -2. );
}

TEST(ScaleTest,UpdateVariable)
{
  auto x = FunG::variable<0>(2.);
  auto fun = ( 2*FunG::CMath::Pow<3,1>() ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
  EXPECT_NO_THROW( fun.updateVariable<0>(-1.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , -2. );
}

TEST(ScaleTest,D0)
{
  using FunG::CMath::Pow;
  auto fun = 2*Pow<3,1>(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(ScaleTest,D1)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar ( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 24. );
}

TEST(ScaleTest,D2)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 24. );
}

TEST(ScaleTest,D3)
{
  using FunG::CMath::Pow;
  auto a = 0;
  auto fun = FunG::finalize_scalar( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 12. );
}


