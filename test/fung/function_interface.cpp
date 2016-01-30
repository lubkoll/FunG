#include <gtest/gtest.h>
#include "fung/cmath/pow.hh"
#include "fung/finalize.hh"
#include "fung/generate.hh"

inline auto functionInterfaceValue()
{
  return 3.;
}

inline auto functionInterfaceValueD1()
{
  return 5.;
}

inline auto functionInterfaceValueD2()
{
  return 8.;
}

inline auto functionInterfaceValueD3()
{
  return 13.;
}

struct ConstantExample
{
  double operator()() const { return functionInterfaceValue(); }
};

struct LinearExample
{
  double operator()() const { return functionInterfaceValue(); }

  double d1(double dx) const { return functionInterfaceValueD1()*dx; }
};

struct QuadraticExample
{
  double operator()() const { return functionInterfaceValue(); }

  double d1(double dx) const { return functionInterfaceValueD1()*dx; }

  double d2(double dx, double dy) const { return functionInterfaceValueD2()*dx*dy; }
};

struct CubicExample
{
  double operator()() const { return functionInterfaceValue(); }

  double d1(double dx) const { return functionInterfaceValueD1()*dx; }

  double d2(double dx, double dy) const { return functionInterfaceValueD2()*dx*dy; }

  double d3(double dx, double dy, double dz) const { return functionInterfaceValueD3()*dx*dy*dz; }
};

inline double constantExampleFunction() { return functionInterfaceValue(); }


TEST(FunctionInterface_Sum,ConstantExampleFunctor)
{
  using namespace FunG;
  auto summand1 = ConstantExample{};
  auto summand2 = Pow<3>(1.);
  auto f = finalize( summand1 + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , 3. );
  ASSERT_EQ( f.d2(1,1) , 6. );
  ASSERT_EQ( f.d3(1,1,1) , 6. );
}

TEST(FunctionInterface_Sum,ConstantExampleFunctionPointer)
{
  using namespace FunG;
  auto summand2 = Pow<3>(1.);
  auto f = finalize( &constantExampleFunction + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , 3. );
  ASSERT_EQ( f.d2(1,1) , 6. );
  ASSERT_EQ( f.d3(1,1,1) , 6. );
}

TEST(FunctionInterface_Sum,ConstantExampleLambda)
{
  using namespace FunG;
  auto summand2 = Pow<3>(1.);
  auto f = finalize( [](){ return functionInterfaceValue(); } + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , 3. );
  ASSERT_EQ( f.d2(1,1) , 6. );
  ASSERT_EQ( f.d3(1,1,1) , 6. );
}

TEST(FunctionInterface_Sum,LinearExampleFunctor)
{
  using namespace FunG;
  auto summand1 = LinearExample{};
  auto summand2 = Pow<3>(1.);
  auto f = finalize( summand1 + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , functionInterfaceValueD1() + 3. );
  ASSERT_EQ( f.d2(1,1) , 6. );
  ASSERT_EQ( f.d3(1,1,1) , 6. );
}

TEST(FunctionInterface_Sum,QuadraticExampleFunctor)
{
  using namespace FunG;
  auto summand1 = QuadraticExample{};
  auto summand2 = Pow<3>(1.);
  auto f = finalize( summand1 + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , functionInterfaceValueD1() + 3. );
  ASSERT_EQ( f.d2(1,1) , functionInterfaceValueD2() + 6. );
  ASSERT_EQ( f.d3(1,1,1) , 6. );
}

TEST(FunctionInterface_Sum,CubicExampleFunctor)
{
  using namespace FunG;
  auto summand1 = CubicExample{};
  auto summand2 = Pow<3>(1.);
  auto f = finalize( summand1 + summand2 );

  ASSERT_EQ( f() , functionInterfaceValue() + 1 );
  ASSERT_EQ( f.d1(1) , functionInterfaceValueD1() + 3. );
  ASSERT_EQ( f.d2(1,1) , functionInterfaceValueD2() + 6. );
  ASSERT_EQ( f.d3(1,1,1) , functionInterfaceValueD3() + 6. );
}

TEST(FunctionInterface_Chain,ConstantExampleFunctor)
{
  using namespace FunG;
  auto f = finalize( pow<2>( ConstantExample{} ) );

  ASSERT_EQ( f() , pow(functionInterfaceValue(),2) );
  ASSERT_EQ( f.d1(1) , 0. );
  ASSERT_EQ( f.d2(1,1) , 0. );
  ASSERT_EQ( f.d3(1,1,1) , 0. );
}

TEST(FunctionInterface_Chain,ConstantExampleFunctionPointer)
{
  using namespace FunG;
  auto f = finalize( pow<2>( &constantExampleFunction ) );

  ASSERT_EQ( f() , pow(functionInterfaceValue(),2) );
  ASSERT_EQ( f.d1(1) , 0. );
  ASSERT_EQ( f.d2(1,1) , 0. );
  ASSERT_EQ( f.d3(1,1,1) , 0. );
}

TEST(FunctionInterface_Chain,ConstantExampleLambda)
{
  using namespace FunG;
  auto f = finalize( pow<2>( [](){ return functionInterfaceValue(); } ) );

  ASSERT_EQ( f() , pow(functionInterfaceValue(),2) );
  ASSERT_EQ( f.d1(1) , 0. );
  ASSERT_EQ( f.d2(1,1) , 0. );
  ASSERT_EQ( f.d3(1,1,1) , 0. );
}

TEST(FunctionInterface_Chain,LinearExampleFunctor)
{
  using namespace FunG;
  auto f = finalize( pow<2>( LinearExample{} ) );

  ASSERT_EQ( f() , pow(functionInterfaceValue(),2) );
  ASSERT_EQ( f.d1(1) , 2*functionInterfaceValue()*functionInterfaceValueD1() );
  ASSERT_EQ( f.d2(1,1) , 2*functionInterfaceValueD1()*functionInterfaceValueD1() );
  ASSERT_EQ( f.d3(1,1,1) , 0. );
}

TEST(FunctionInterface_Chain,QuadraticExampleFunctor)
{
  using namespace FunG;
  auto f = finalize( pow<2>( QuadraticExample{} ) );

  ASSERT_EQ( f() , pow(functionInterfaceValue(),2) );
  ASSERT_EQ( f.d1(1) , 2*functionInterfaceValue()*functionInterfaceValueD1());
  ASSERT_EQ( f.d2(1,1) , 2*( functionInterfaceValue()*functionInterfaceValueD2() + functionInterfaceValueD1()*functionInterfaceValueD1() ) );
  ASSERT_EQ( f.d3(1,1,1) , 6*functionInterfaceValueD1()*functionInterfaceValueD2() );
}

