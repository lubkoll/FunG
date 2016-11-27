#include <gtest/gtest.h>

#include <Eigen/Dense>

#include <fung/generate.hh>
#include <fung/finalize.hh>

const constexpr int dim = 2;

namespace
{
    auto get_ones()
    {
        auto v = Eigen::VectorXd(dim);
        for(auto i=0; i<dim; ++i)
            v(i) = 1;
        return v;
    }
}

TEST(DotTest_DynamicEigen,ConstantVectors)
{
    using namespace FunG;
    auto v = constant(get_ones());
    auto w = constant(get_ones());

    auto f = finalize(dot(v,w));

    EXPECT_DOUBLE_EQ( f(), 2.0 );
    EXPECT_EQ( f.d1(get_ones()), 0.0 );
}

TEST(DotTest_DynamicEigen,FirstVariable_SecondConstant)
{
    using namespace FunG;
    auto v = variable<0>(get_ones());
    auto w = constant(get_ones());

    auto f = finalize(dot(v,w));

    EXPECT_DOUBLE_EQ( f(), 2.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    auto value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value , 0.0 );

    Eigen::VectorXd v1 = 2*get_ones();
    f.update<0>(v1);

    EXPECT_DOUBLE_EQ( f(), 4.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );
}

TEST(DotTest_DynamicEigen,FirstConstant_SecondVariable)
{
    using namespace FunG;
    auto v = variable<0>(get_ones());
    auto w = constant(get_ones());

    auto f = finalize(dot(w,v));

    EXPECT_DOUBLE_EQ( f(), 2.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    auto value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );

    Eigen::VectorXd v1 = 2*get_ones();
    f.update<0>(v1);

    EXPECT_DOUBLE_EQ( f(), 4.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );
}

TEST(DotTest_DynamicEigen,FirstVariable_SecondVariable)
{
    using namespace FunG;
    auto v = variable<0>(get_ones());
    auto w = variable<1>(get_ones());

    auto f = finalize(dot(v,w));

    EXPECT_DOUBLE_EQ( f(), 2.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    auto value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );
    value = f.d2<0,1>(get_ones(), get_ones());
    EXPECT_EQ( value, 2.0 );

    Eigen::VectorXd v1 = 2*get_ones();
    f.update<0>(v1);

    EXPECT_DOUBLE_EQ( f(), 4.0 );
    EXPECT_EQ( f.d1<0>(get_ones()), 2.0 );
    value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );
    value = f.d2<0,1>(get_ones(), get_ones());
    EXPECT_EQ( value, 2.0 );

    Eigen::VectorXd w1 = 2*get_ones();
    f.update<1>(w1);

    EXPECT_DOUBLE_EQ( f(), 8.0 );
    EXPECT_EQ( f.d1<1>(get_ones()), 4.0 );
    value = f.d2<0,0>(get_ones(), get_ones());
    EXPECT_EQ( value, 0.0 );
    value = f.d2<0,1>(get_ones(), get_ones());
    EXPECT_EQ( value, 2.0 );
}


