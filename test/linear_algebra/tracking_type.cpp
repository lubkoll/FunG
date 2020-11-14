#include <fung/cmath/pow.hh>
#include <fung/cmath/sine.hh>
#include <fung/finalize.hh>
#include <fung/generate.hh>
#include <fung/variable.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Eigen/Eigen>

TEST( CheckArgumentTestEigen, FinalizeTrackingType )
{
    using namespace FunG;
    using Arg = Eigen::MatrixXd;
    auto y = Arg( 1, 1 );
    y << 1.0;
    auto y_ref = Arg( 1, 1 );
    y_ref << 1.0;
    auto u = Arg( 1, 1 );
    u << 1.0;
    using F = decltype( finalize( squared( variable< 0 >( y ) - variable< 4 >( y_ref ) ) +
                                  2.0 * squared( variable< 1 >( u ) ) ) );
    const auto value = Checks::CheckArgument< F, Arg, 1 >::value;
    EXPECT_TRUE( value );
}
