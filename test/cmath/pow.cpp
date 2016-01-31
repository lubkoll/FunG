// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#define FUNG_ENABLE_EXCEPTIONS
#include <gtest/gtest.h>
#include "fung/cmath/pow.hh"

#include <limits>

namespace
{
  auto x0()
  {
    return 1;
  }

  auto x1()
  {
    return 2;
  }

  auto condition()
  {
    return 1e1;
  }

  auto eps()
  {
    return std::numeric_limits<double>::epsilon();
  }

  auto generateTestSquared()
  {
    return FunG::Pow<2>( x0() );
  }

  auto generateTestCubic()
  {
    return FunG::Pow<3>( x0() );
  }

  auto generateTestInverse()
  {
    return FunG::Pow<-1>( x0() );
  }

  auto generateTestSqrt()
  {
    return FunG::Sqrt( x1() );
  }

  auto generateTestCbrt()
  {
    return FunG::Cbrt( x0() );
  }

  auto generateTestCbrt2()
  {
    return FunG::Cbrt2( x0() );
  }

  auto generateTestOverCbrt()
  {
    return FunG::Pow<-1,3>( x1() );
  }

  auto generateTestOverCbrt2()
  {
    return FunG::Pow<-2,3>( x1() );
  }
}

TEST(PowSquaredTest,D0)
{
  auto fun = generateTestSquared();
  EXPECT_DOUBLE_EQ( fun.d0() , x0()*x0() );
}

TEST(PowSquaredTest,D1)
{
  auto fun = generateTestSquared();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 2.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 2*dx );
}

TEST(PowSquaredTest, D1DifferentialQuotient)
{
  auto fun = generateTestSquared();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x0() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(PowSquaredTest,D2)
{
  auto fun = generateTestSquared();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 2.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 2*dx*dy );
}


///
TEST(PowCubicTest,D0)
{
  auto fun = generateTestCubic();
  EXPECT_DOUBLE_EQ( fun.d0() , x0()*x0()*x0() );
}

TEST(PowCubicTest,D1)
{
  auto fun = generateTestCubic();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 3.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 3*dx );
}

TEST(PowCubicTest, D1DifferentialQuotient)
{
  auto fun = generateTestCubic();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x0() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(PowCubicTest,D2)
{
  auto fun = generateTestCubic();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 6.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 6*dx*dy );
}

TEST(PowCubicTest,D3)
{
  auto fun = generateTestCubic();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 6.         );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 6*dx*dy*dz );
}


///
TEST(PowInverseTest,Update)
{
  auto fun = generateTestInverse();
  EXPECT_THROW(fun.update(0), FunG::OutOfDomainException);
}

TEST(PowInverseTest,D0)
{
  auto fun = generateTestInverse();
  EXPECT_DOUBLE_EQ( fun.d0() , 1/x0() );
}

TEST(PowInverseTest,D1)
{
  auto fun = generateTestInverse();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , -1.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -1*dx );
}

TEST(PowInverseTest, D1DifferentialQuotient)
{
  auto fun = generateTestInverse();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x0() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}


TEST(PowInverseTest,D2)
{
  auto fun = generateTestInverse();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 2.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 2*dx*dy );
}

TEST(PowInverseTest,D3)
{
  auto fun = generateTestInverse();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , -6.         );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -6*dx*dy*dz );
}


///
TEST(PowSqrtTest,Update)
{
  auto fun = generateTestSqrt();
  EXPECT_THROW(fun.update(-1), FunG::OutOfDomainException);
}

TEST(PowSqrtTest,D0)
{
  auto fun = generateTestSqrt();
  EXPECT_DOUBLE_EQ( fun.d0() , sqrt( x1() ) );
}

TEST(PowSqrtTest,D1)
{
  auto fun = generateTestSqrt();
  auto dx = 2.;
  auto s = sqrt( x1() );
  EXPECT_DOUBLE_EQ( fun.d1()   , .5/s    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , .5/s*dx );
}

TEST(PowSqrtTest, D1DifferentialQuotient)
{
  auto fun = generateTestSqrt();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x1() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(PowSqrtTest,D2)
{
  auto fun = generateTestSqrt();
  auto dx = 2., dy = 3.;
  auto s = sqrt( x1() );
  EXPECT_DOUBLE_EQ( fun.d2()      , -0.25/(s*s*s)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -0.25/(s*s*s)*dx*dy );
}

TEST(PowSqrtTest,D3)
{
  auto fun = generateTestSqrt();
  auto dx = 2., dy = 3., dz = 4.;
  auto s = sqrt( x1() );
  EXPECT_DOUBLE_EQ( fun.d3()         , 0.375/(s*s*s*s*s)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 0.375/(s*s*s*s*s)*dx*dy*dz );
}


///
TEST(PowOverThirdRootTest,Update)
{
  auto fun = generateTestOverCbrt();
  EXPECT_THROW(fun.update(-0.1), FunG::OutOfDomainException);
}

TEST(PowOverThirdRootTest,D0)
{
  auto fun = generateTestOverCbrt();
  EXPECT_DOUBLE_EQ( fun.d0() , 1/cbrt( x1() ) );
}

TEST(PowOverThirdRootTest,D1)
{
  auto fun = generateTestOverCbrt();
  auto dx = 2.;
  auto s = 1/cbrt( x1() );
  EXPECT_DOUBLE_EQ( fun.d1()   , -1./3*pow(s,4)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -1./3*pow(s,4)*dx );
}

TEST(PowOverThirdRootTest, D1DifferentialQuotient)
{
  auto fun = generateTestOverCbrt();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x1() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(PowOverThirdRootTest,D2)
{
  auto fun = generateTestOverCbrt();
  auto dx = 2., dy = 3.;
  auto s = 1/cbrt( x1() );
  EXPECT_NEAR( fun.d2()      , 4./9*pow(s,7)       , eps()*condition() );
  EXPECT_NEAR( fun.d2(dx,dy) , 4./9*pow(s,7)*dx*dy , eps()*condition() );
}

TEST(PowOverThirdRootTest,D3)
{
  auto fun = generateTestOverCbrt();
  auto dx = 2., dy = 3., dz = 4.;
  auto s = 1/(8*cbrt( x1() ));
  EXPECT_DOUBLE_EQ( fun.d3()         , -28./27*s          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -28./27*s*dx*dy*dz );
}


///
TEST(PowOverThirdRootSquaredTest,Update)
{
  auto fun = generateTestOverCbrt2();
  EXPECT_THROW(fun.update(-0.1), FunG::OutOfDomainException);
}

TEST(PowOverThirdRootSquaredTest,D0)
{
  auto fun = generateTestOverCbrt2();
  auto val = 1/cbrt( x1() );
  EXPECT_DOUBLE_EQ( fun.d0() , val*val );
}

TEST(PowOverThirdRootSquaredTest,D1)
{
  auto fun = generateTestOverCbrt2();
  auto dx = 2.;
  double val = 1/( x1()*cbrt( x1() )*cbrt( x1() ) );
  EXPECT_DOUBLE_EQ( fun.d1()   , -2./3*val    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -2./3*val*dx );
}

TEST(PowOverThirdRootSquaredTest, D1DifferentialQuotient)
{
  auto fun = generateTestOverCbrt2();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update( x1() + dx );
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(PowOverThirdRootSquaredTest,D2)
{
  auto fun = generateTestOverCbrt2();
  auto dx = 2., dy = 3.;
  double val = 1/( x1()*x1()*cbrt( x1() )*cbrt( x1() ) );
  EXPECT_DOUBLE_EQ( fun.d2()      , 10./9*val       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 10./9*val*dx*dy );
}

TEST(PowOverThirdRootSquaredTest,D3)
{
  auto fun = generateTestOverCbrt2();
  auto dx = 2., dy = 3., dz = 4.;
  auto val = 1/( x1()*x1()*x1()*cbrt( x1() )*cbrt( x1() ) );
  EXPECT_DOUBLE_EQ( fun.d3()         , -80./27*val          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -80./27*val*dx*dy*dz );
}


///
TEST(PowDefaultTest,Update)
{
  FunG::Pow<3,2> fun( x0() );
  EXPECT_THROW(fun.update(0), FunG::OutOfDomainException);
}

TEST(PowDefaultTest,D0)
{
  const FunG::Pow<5,2> fun( x1() );
  EXPECT_DOUBLE_EQ( fun.d0() , pow( x1(), 2.5 ) );
}

TEST(PowDefaultTest,D1)
{
  const FunG::Pow<5,2> fun( x1() );
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 2.5*pow( x1(), 1.5 )    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 2.5*pow( x1(), 1.5 )*dx );
}

TEST(PowDefaultTest,D2)
{
  const FunG::Pow<5,2> fun( x1() );
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 1.5*2.5*pow( x1(), 0.5 )       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 1.5*2.5*pow( x1(), 0.5 )*dx*dy );
}

TEST(PowDefaultTest,D3)
{
  const FunG::Pow<5,2> fun( x1() );
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  0.5*1.5*2.5*pow( x1(), -0.5 )          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  0.5*1.5*2.5*pow( x1(), -0.5 )*dx*dy*dz );
}


