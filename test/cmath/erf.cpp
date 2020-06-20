#include <fung/cmath/erf.hh>

#include <gtest/gtest.h>

using FunG::Erf;

namespace
{
  auto x0()
  {
    return 1.0;
  }

  auto generateTestErf()
  {
    return FunG::Erf( x0() );
  }
}

const auto scale = 2/std::sqrt(M_PI);

TEST(ErfTest,D0)
{
  auto fun = generateTestErf();
  EXPECT_DOUBLE_EQ( fun.d0() ,  std::erf(x0()) );
}

TEST(ErfTest,D1)
{
  auto fun = generateTestErf();
  auto dx = 2.;
  EXPECT_NEAR( fun.d1()   , scale*std::exp(-x0()*x0())   , 1e-12 );
  EXPECT_NEAR( fun.d1(dx) , scale*std::exp(-x0()*x0())*dx, 1e-12*dx );
}

TEST(ErfTest, D1DifferentialQuotient)
{
  auto fun = generateTestErf();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update(x0() + dx);
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx );
}

TEST(ErfTest,D2)
{
  const auto fun = generateTestErf();
  const auto dx = 2.;
  const auto dy = 3.;
  EXPECT_NEAR( fun.d2()       , -scale*2*x0()*std::exp(-x0()*x0())   , 1e-12 );
  EXPECT_NEAR( fun.d2(dx, dy) , -scale*2*x0()*std::exp(-x0()*x0())*dx*dy, 1e-12*dx*dy );
}

TEST(ErfTest, D2DifferentialQuotient)
{
  auto fun = generateTestErf();
  const auto dx = 1e-6;
  const auto f0 = fun();
  fun.update(x0() + dx);
  const auto f1 = fun();
  fun.update(x0() - dx);
  const auto f2 = fun();
  EXPECT_NEAR( fun.d2() , ( f1 + f2 - 2*f0 )/(dx*dx) , 1e-5);
}

TEST(ErfTest,D3)
{
  auto fun = generateTestErf();
  auto dx = 2., dy = 3., dz = 4.;
  EXPECT_NEAR( fun.d3()         ,  scale*2*( 2*x0()*x0()*std::exp(-x0()*x0()) - std::exp(-x0()*x0()))   , 1e-12 );
//  EXPECT_NEAR( fun.d3(dx,dy,dz) ,  std::erf(x0())*dx*dy*dz );
}
