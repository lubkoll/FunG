#include "../../src/CMath/pow.hh"

#include <gtest/gtest.h>

TEST(PowSquaredTest,D0)
{
  const RFFGen::CMath::Pow<2,1> fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 1. );
}

TEST(PowSquaredTest,D1)
{
  const RFFGen::CMath::Pow<2,1> fun(1.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 2.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 2*dx );
}

TEST(PowSquaredTest,D2)
{
  const RFFGen::CMath::Pow<2,1> fun(1.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 2.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 2*dx*dy );
}


///
TEST(PowCubedTest,D0)
{
  const RFFGen::CMath::Pow<3,1> fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 1. );
}

TEST(PowCubedTest,D1)
{
  const RFFGen::CMath::Pow<3,1> fun(1.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 3.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 3*dx );
}

TEST(PowCubedTest,D2)
{
  const RFFGen::CMath::Pow<3,1> fun(1.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 6.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 6*dx*dy );
}

TEST(PowCubedTest,D3)
{
  const RFFGen::CMath::Pow<3,1> fun(1.);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 6.         );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 6*dx*dy*dz );
}


///
TEST(PowInverseTest,Update)
{
  RFFGen::CMath::Pow<-1> fun(1.);
  EXPECT_THROW(fun.update(0), RFFGen::OutOfDomainException);
}

TEST(PowInverseTest,D0)
{
  const RFFGen::CMath::Pow<-1> fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 1. );
}

TEST(PowInverseTest,D1)
{
  const RFFGen::CMath::Pow<-1> fun(1.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , -1.   );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -1*dx );
}

TEST(PowInverseTest,D2)
{
  const RFFGen::CMath::Pow<-1> fun(1.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 2.      );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 2*dx*dy );
}

TEST(PowInverseTest,D3)
{
  const RFFGen::CMath::Pow<-1> fun(1.);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , -6.         );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -6*dx*dy*dz );
}


///
TEST(PowSqrtTest,Update)
{
  RFFGen::CMath::Pow<1,2> fun(1.);
  EXPECT_THROW(fun.update(-1), RFFGen::OutOfDomainException);
}

TEST(PowSqrtTest,D0)
{
  const RFFGen::CMath::Pow<1,2> fun(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , sqrt(2) );
}

TEST(PowSqrtTest,D1)
{
  const RFFGen::CMath::Pow<1,2> fun(2.);
  const double dx = 2.;
  const double s = sqrt(2.);
  EXPECT_DOUBLE_EQ( fun.d1()   , .5/s    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , .5/s*dx );
}

TEST(PowSqrtTest,D2)
{
  const RFFGen::CMath::Pow<1,2> fun(2.);
  const double dx = 2., dy = 3.;
  const double s = sqrt(2);
  EXPECT_DOUBLE_EQ( fun.d2()      , -0.25/(s*s*s)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -0.25/(s*s*s)*dx*dy );
}

TEST(PowSqrtTest,D3)
{
  const RFFGen::CMath::Pow<1,2> fun(2.);
  const double dx = 2., dy = 3., dz = 4.;
  const double s = sqrt(2);
  EXPECT_DOUBLE_EQ( fun.d3()         , 0.375/(s*s*s*s*s)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 0.375/(s*s*s*s*s)*dx*dy*dz );
}


///
TEST(PowOverThirdRootTest,Update)
{
  RFFGen::CMath::Pow<-1,3> fun(1.);
  EXPECT_THROW(fun.update(-0.1), RFFGen::OutOfDomainException);
}

TEST(PowOverThirdRootTest,D0)
{
  const RFFGen::CMath::Pow<-1,3> fun(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , 1/cbrt(2) );
}

TEST(PowOverThirdRootTest,D1)
{
  const RFFGen::CMath::Pow<-1,3> fun(2.);
  const double dx = 2.;
  const double s = 1/cbrt(2.);
  EXPECT_DOUBLE_EQ( fun.d1()   , -1./3*pow(s,4)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -1./3*pow(s,4)*dx );
}

TEST(PowOverThirdRootTest,D2)
{
  const RFFGen::CMath::Pow<-1,3> fun(2.);
  const double dx = 2., dy = 3.;
  const double s = 1/cbrt(2);
  EXPECT_DOUBLE_EQ( fun.d2()      , 4./9*pow(s,7)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 4./9*pow(s,7)*dx*dy );
}

TEST(PowOverThirdRootTest,D3)
{
  const RFFGen::CMath::Pow<-1,3> fun(2.);
  const double dx = 2., dy = 3., dz = 4.;
  const double s = 1/(8*cbrt(2));
  EXPECT_DOUBLE_EQ( fun.d3()         , -28./27*s          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -28./27*s*dx*dy*dz );
}


///
TEST(PowOverThirdRootSquaredTest,Update)
{
  RFFGen::CMath::Pow<-2,3> fun(1.);
  EXPECT_THROW(fun.update(-0.1), RFFGen::OutOfDomainException);
}

TEST(PowOverThirdRootSquaredTest,D0)
{
  const RFFGen::CMath::Pow<-2,3> fun(2.);
  const double val = 1/cbrt(2);
  EXPECT_DOUBLE_EQ( fun.d0() , val*val );
}

TEST(PowOverThirdRootSquaredTest,D1)
{
  const double x = 2;
  const RFFGen::CMath::Pow<-2,3> fun(x);
  const double dx = 2.;
  double val = 1/( x*cbrt(2)*cbrt(2) );
  EXPECT_DOUBLE_EQ( fun.d1()   , -2./3*val    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -2./3*val*dx );
}

TEST(PowOverThirdRootSquaredTest,D2)
{
  const double x = 2;
  const RFFGen::CMath::Pow<-2,3> fun(x);
  const double dx = 2., dy = 3.;
  double val = 1/(x*x*cbrt(2)*cbrt(2));
  EXPECT_DOUBLE_EQ( fun.d2()      , 10./9*val       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 10./9*val*dx*dy );
}

TEST(PowOverThirdRootSquaredTest,D3)
{
  const double x = 2;
  const RFFGen::CMath::Pow<-2,3> fun(x);
  const double dx = 2., dy = 3., dz = 4.;
  const double val = 1/(x*x*x*cbrt(2)*cbrt(2));
  EXPECT_DOUBLE_EQ( fun.d3()         , -80./27*val          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , -80./27*val*dx*dy*dz );
}


///
TEST(PowDefaultTest,Update)
{
  RFFGen::CMath::Pow<3,2> fun(1.);
  EXPECT_THROW(fun.update(-0.1), RFFGen::OutOfDomainException);
}

TEST(PowDefaultTest,D0)
{
  const RFFGen::CMath::Pow<5,2> fun(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , pow(2,2.5) );
}

TEST(PowDefaultTest,D1)
{
  const RFFGen::CMath::Pow<5,2> fun(2.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 2.5*pow(2,1.5)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 2.5*pow(2,1.5)*dx );
}

TEST(PowDefaultTest,D2)
{
  const RFFGen::CMath::Pow<5,2> fun(2.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , 1.5*2.5*pow(2,.5)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , 1.5*2.5*pow(2,.5)*dx*dy );
}

TEST(PowDefaultTest,D3)
{
  const RFFGen::CMath::Pow<5,2> fun(2.);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  0.5*1.5*2.5*pow(2,-.5)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  0.5*1.5*2.5*pow(2,-.5)*dx*dy*dz );
}


